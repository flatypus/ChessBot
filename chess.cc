#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "color.h"
#include "maps.h"
#include "utils.h"

class Piece;
using Row = std::vector<std::shared_ptr<Piece>>;

void _err(std::string msg)
{
    std::cout << "Invalid move: " << msg << std::endl;
}

class Piece : public Player
{
private:
    int color;
    Symbol symbol;
    int x;
    int y;
    bool moved = false;

public:
    Piece() {}
    Piece(int color, Symbol symbol, int x, int y) : color(color),
                                                    symbol(symbol),
                                                    x(x),
                                                    y(y) {}
    Symbol getSymbol()
    {
        return symbol;
    }
    int getColor()
    {
        return color;
    }
    void setX(int x)
    {
        this->x = x;
    }
    void setY(int y)
    {
        this->y = y;
    }
    void setMoved()
    {
        this->moved = true;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    bool getMoved()
    {
        return moved;
    }
    virtual bool validMove(int x, int y, bool capture = false)
    {
        return true;
    }
    void getInfo()
    {
        cout << "Piece: " << (char)(symbol) << " " << color << " " << x << " " << y << std::endl;
    }
};

class Pawn : public Piece
{
public:
    Pawn(int color, int x, int y) : Piece(color, Symbol::PAWN, x, y) {}
    bool validMove(int x, int y, bool capture = false)
    {
        bool valid = false;
        if (capture)
        {
            switch (this->getColor())
            {
            case Piece::WHITE:
                valid = this->getY() + 1 == y && (this->getX() + 1 == x || this->getX() - 1 == x);
                break;
            case Piece::BLACK:
                valid = this->getY() - 1 == y && (this->getX() + 1 == x || this->getX() - 1 == x);
                break;
            }
        }
        else
        {
            switch (this->getColor())
            {
            case Piece::WHITE:
                valid = (this->getY() == 1 && y == 3 || this->getY() + 1 == y) && this->getX() == x && !capture;
                break;
            case Piece::BLACK:
                valid = (this->getY() == 6 && y == 4 || this->getY() - 1 == y) && this->getX() == x && !capture;
                break;
            }
        }
        return valid;
    };
};

class Rook : public Piece
{
public:
    Rook(int color, int x, int y) : Piece(color, Symbol::ROOK, x, y) {}
    bool validMove(int x, int y, bool capture = false)
    {
        cout << "Checking rook" << std::endl;
        cout << this->getX() << " " << this->getY() << " " << x << " " << y << std::endl;
        return this->getX() == x || this->getY() == y;
    }
};

class Knight : public Piece
{
public:
    Knight(int color, int x, int y) : Piece(color, Symbol::KNIGHT, x, y) {}
    bool validMove(int x, int y, bool capture = false)
    {
        cout << "Checking knight" << std::endl;
        cout << this->getX() << " " << this->getY() << " " << x << " " << y << std::endl;
        return abs(this->getX() - x) == 2 && abs(this->getY() - y) == 1 || abs(this->getX() - x) == 1 && abs(this->getY() - y) == 2;
    }
};

class Bishop : public Piece
{
public:
    Bishop(int color, int x, int y) : Piece(color, Symbol::BISHOP, x, y) {}
    bool validMove(int x, int y, bool capture = false)
    {
        return abs(this->getX() - x) == abs(this->getY() - y);
    }
};

class Queen : public Piece
{
public:
    Queen(int color, int x, int y) : Piece(color, Symbol::QUEEN, x, y) {}
    bool validMove(int x, int y, bool capture = false)
    {
        return abs(this->getX() - x) == abs(this->getY() - y) || this->getX() == x || this->getY() == y;
    }
};

class King : public Piece
{
public:
    King(int color, int x, int y) : Piece(color, Symbol::KING, x, y) {}
    bool validMove(int x, int y, bool capture = false)
    {
        return abs(this->getX() - x) <= 1 && abs(this->getY() - y) <= 1;
    }
};

class Board
{
    mutable std::vector<Row> board;
    mutable Row white_pieces;
    mutable Row black_pieces;
    mutable Row white_capture;
    mutable Row black_capture;
    mutable int turn = Piece::WHITE;

public:
    Board()
    {
        setupBoard();
        while (true)
        {
            displayBoard();
            requestMove();
            switchTurn();
        }
    }

private:
    void switchTurn()
    {
        turn = turn == Piece::WHITE ? Piece::BLACK : Piece::WHITE;
    }
    bool inRange(char file, char rank)
    {
        return !(letter_map.find(file) == letter_map.end() || num_map.find(rank) == num_map.end());
    }

    bool checkOverlap(int start_x, int start_y, int end_x, int end_y)
    {
        // return true if no overlap
        cout << "Checking overlap" << std::endl;
        int x_diff = end_x - start_x;
        int y_diff = end_y - start_y;
        int x_dir = x_diff == 0 ? 0 : x_diff / abs(x_diff);
        int y_dir = y_diff == 0 ? 0 : y_diff / abs(y_diff);
        for (int i = 1; i < max(abs(x_diff), abs(y_diff)); i++)
        {
            if (board[start_y + i * y_dir][start_x + i * x_dir]->getSymbol() != Symbol::EMPTY)
            {
                return false;
            }
        }
        return true;
    }

    void short_castle(int color)
    {
        int rank = color == Piece::WHITE ? 0 : 7;
        std::shared_ptr<Piece> king = board[rank][4];
        std::shared_ptr<Piece> rook = board[rank][7];

        if (!checkOverlap(king->getX(), king->getY(), 6, rank) || !checkOverlap(rook->getX(), rook->getY(), 5, rank))
        {
            _err("can't short castle; pieces in the way");
            return;
        }
        if (king->getSymbol() != Symbol::KING || rook->getSymbol() != Symbol::ROOK)
        {
            _err("you can't short castle here!");
            return;
        }

        if (king->getMoved() || rook->getMoved())
        {
            _err("can't short castle; piece has been moved before");
            return;
        }
        makeMove(king, 6, rank);
        makeMove(rook, 5, rank);
    }

    void requestMove()
    {
        std::string input;
        std::cout << "It is " << (turn == Piece::WHITE ? "white" : "black") << "'s turn" << std::endl;
        std::cout << "Enter a move: " << std::endl;
        std::cin >> input;
        if (!parseMove(input))
        {
            displayBoard();
            requestMove();
        }
    }

    std::shared_ptr<Piece> pieceFromSymbol(Symbol symbol, int color, int x, int y)
    {
        switch (symbol)
        {
        case Symbol::PAWN:
            return std::make_shared<Pawn>(color, x, y);
        case Symbol::ROOK:
            return std::make_shared<Rook>(color, x, y);
        case Symbol::KNIGHT:
            return std::make_shared<Knight>(color, x, y);
        case Symbol::BISHOP:
            return std::make_shared<Bishop>(color, x, y);
        case Symbol::QUEEN:
            return std::make_shared<Queen>(color, x, y);
        case Symbol::KING:
            return std::make_shared<King>(color, x, y);
        default:
            return std::make_shared<Piece>(-1, Symbol::EMPTY, x, y);
        }
    }

    void makeMove(std::shared_ptr<Piece> piece, int new_file, int new_rank)
    {
        std::cout << new_file << " " << new_rank << std::endl;
        int old_file = piece->getX();
        int old_rank = piece->getY();

        board[old_rank][old_file] = std::make_shared<Piece>(-1, Symbol::EMPTY, old_file, old_rank);
        piece->setX(new_file);
        piece->setY(new_rank);
        piece->setMoved();
        board[new_rank][new_file] = piece;
    }

    void implementCapture(int color, int x, int y)
    {
        std::cout << "Implementing capture " << x << " " << y << std::endl;
        Row *piece_list = color == Piece::WHITE ? &black_pieces : &white_pieces;
        Row *capture_list = color == Piece::WHITE ? &white_capture : &black_capture;
        for (Row::iterator it = piece_list->begin(); it != piece_list->end(); ++it)
        {
            if ((*it)->getX() == x && (*it)->getY() == y)
            {
                (*it)->getInfo();
                capture_list->push_back(*it);
                piece_list->erase(it);
                break;
            }
        }
    }

    bool parseMove(std::string input)
    {
        if (input == "0-0")
        {
            short_castle(turn);
            return true;
        }

        if (input.length() < 2)
        {
            _err("too short");
            return false;
        }
        char file = input[input.length() - 2];
        char rank = input[input.length() - 1];

        if (!inRange(file, rank))
        {
            _err("out of range");
            return false;
        }

        if (board[num_map[rank]][letter_map[file]]->getColor() == turn)
        {
            _err("cannot capture own piece");
            return false;
        }

        bool capture = board[num_map[rank]][letter_map[file]]->getSymbol() != Symbol::EMPTY;
        char piece = isupper(input[0]) ? input[0] : 'P';
        char disambiguation;

        if (capture && count(input, 'x') != 1)
        {
            _err("proper algebraic notation requires 'x' for captures");
            return false;
        }

        if (piece != 'P' && piece != 'R' && piece != 'N' && piece != 'B' && piece != 'Q' && piece != 'K')
        {
            _err("invalid piece");
            return false;
        }
        if (piece == 'P')
        {
            // if the move is pawn like fxe4, the disambiguator is f
            disambiguation = input.length() == 2 ? ' ' : input[0];
        }
        else
        {
            // otherwise if the move is rook like R6g4, the disambiguator is 6
            disambiguation = input.length() == 3 ? ' ' : input[1];
        }
        disambiguation = disambiguation == 'x' ? ' ' : disambiguation;
        cout << piece << " " << file << " " << rank << " " << capture << " " << disambiguation << std::endl;
        Row valid_pieces = {};
        for (Row row : board)
        {
            for (std::shared_ptr<Piece> p : row)
            {
                if ((char)(p->getSymbol()) != piece || p->getColor() != turn)
                {
                    continue;
                }
                if (disambiguation == ' ' ? true : (disambiguation == coords[p->getX()] || disambiguation == coords[p->getY()]))
                {
                    valid_pieces.push_back(p);
                }
            }
        }
        for (std::shared_ptr<Piece> p : valid_pieces)
        {
            int x = letter_map[file];
            int y = num_map[rank];
            if (p->getX() == x && p->getY() == y)
                continue;
            std::cout << "Checking piece: " << (char)(p->getSymbol()) << " " << p->getX() << " " << p->getY() << std::endl;
            if (p->validMove(letter_map[file], num_map[rank], capture))
            {
                std::cout << "Found a piece to check: " << (char)(p->getSymbol()) << " " << p->getX() << " " << p->getY() << std::endl;
                if (!(piece == 'N') && !checkOverlap(p->getX(), p->getY(), x, y))
                {
                    _err("overlap");
                    continue;
                }
                std::cout << "Valid move" << std::endl;
                if (capture)
                    implementCapture(turn, x, y);
                makeMove(p, x, y);
                return true;
            }
        }
        _err("move doesn't work");
        return false;
    }

    void setupBoard()
    {
        std::vector<Symbol> backRow = {Symbol::ROOK, Symbol::KNIGHT, Symbol::BISHOP, Symbol::QUEEN, Symbol::KING, Symbol::BISHOP, Symbol::KNIGHT, Symbol::ROOK};
        board.resize(8);
        for (int i = 0; i < 8; i++)
        {
            board[0].push_back(pieceFromSymbol(backRow[i], Piece::WHITE, i, 0));
            board[1].push_back(pieceFromSymbol(Symbol::PAWN, Piece::WHITE, i, 1));
            for (int j = 2; j < 6; j++)
            {
                board[j].push_back(pieceFromSymbol(Symbol::EMPTY, Piece::EMPTY, i, j));
            }
            board[6].push_back(pieceFromSymbol(Symbol::PAWN, Piece::BLACK, i, 6));
            board[7].push_back(pieceFromSymbol(backRow[i], Piece::BLACK, i, 7));
            white_pieces.push_back(board[0][i]);
            white_pieces.push_back(board[1][i]);
            black_pieces.push_back(board[6][i]);
            black_pieces.push_back(board[7][i]);
        }
    }

    void printList(Row list)
    {
        Row sorted_list = {};
        for (Symbol symbol : {Symbol::KING, Symbol::QUEEN, Symbol::ROOK, Symbol::BISHOP, Symbol::KNIGHT, Symbol::PAWN})
        {
            for (std::shared_ptr<Piece> p : list)
            {
                if (p->getSymbol() == symbol)
                    sorted_list.push_back(p);
            }
        }
        for (std::shared_ptr<Piece> p : sorted_list)
        {
            cout << Color::to_chess_piece(p->getColor(), p->getSymbol()) << " ";
        }
    }

    void displayBoard()
    {
        for (int y = board.size() - 1; y >= 0; y--)
        {
            std::cout << y << " ";
            std::cout << y + 1 << " ";
            for (int x = 0; x < board[0].size(); x++)
            {
                int color = board[y][x]->getColor();
                Symbol symbol = board[y][x]->getSymbol();
                std::cout << Color::getColoredString(symbol, Foreground::WHITE, (x + y) % 2 == 0 ? Background::BEIGE : Background::GREEN, color);
            }
            switch (y)
            {
            case 7:
                std::cout << "    White pieces: ";
                printList(white_pieces);
                break;
            case 6:
                std::cout << "    Black pieces: ";
                printList(black_pieces);
                break;
            case 5:
                std::cout << "    White captured: ";
                printList(white_capture);
                break;
            case 4:
                std::cout << "    Black captured: ";
                printList(black_capture);
                break;
            }
            std::cout << std::endl;
        }
        std::cout << "    ";
        for (int i = 0; i < 8; i++)
        {
            std::cout << " " << coords[i] << " ";
        }
        std::cout << std::endl
                  << "    ";
        for (int i = 0; i < 8; i++)
        {
            std::cout << " " << i << " ";
        }
        std::cout << std::endl;
    }
};

int main()
{
    Board board = Board();
    return 0;
}
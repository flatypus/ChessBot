#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "color.h"
#include "utils.h"

using namespace std;

void _err(string msg)
{
    cout << "Invalid move: " << msg << endl;
}

vector<char>
    coords = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
map<char, int> letter_map = {
    {'a', 0},
    {'b', 1},
    {'c', 2},
    {'d', 3},
    {'e', 4},
    {'f', 5},
    {'g', 6},
    {'h', 7}};
map<char, int> num_map = {
    {'1', 0},
    {'2', 1},
    {'3', 2},
    {'4', 3},
    {'5', 4},
    {'6', 5},
    {'7', 6},
    {'8', 7}};

class Piece : public Player
{
private:
    int color;
    Symbol symbol;
    int x;
    int y;

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
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    virtual bool validMove(int x, int y, bool capture = false)
    {
        return true;
    }
    // when cout
    void getInfo()
    {
        cout << "Piece: " << (char)(symbol) << " " << color << " " << x << " " << y << endl;
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
        cout << "Checking rook" << endl;
        cout << this->getX() << " " << this->getY() << " " << x << " " << y << endl;
        return this->getX() == x || this->getY() == y;
    }
};

class Knight : public Piece
{
public:
    Knight(int color, int x, int y) : Piece(color, Symbol::KNIGHT, x, y) {}
    bool validMove(int x, int y, bool capture = false)
    {
        return false;
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
        return false;
    }
};

class King : public Piece
{
public:
    King(int color, int x, int y) : Piece(color, Symbol::KING, x, y) {}
    bool validMove(int x, int y, bool capture = false)
    {
        return false;
    }
};

class Board
{
    mutable vector<vector<shared_ptr<Piece>>> board;
    mutable vector<shared_ptr<Piece>> white_pieces;
    mutable vector<shared_ptr<Piece>> black_pieces;
    mutable vector<shared_ptr<Piece>> white_capture;
    mutable vector<shared_ptr<Piece>> black_capture;
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
        cout << "Checking overlap" << endl;
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

    void requestMove()
    {
        string input;
        cout << "It is " << (turn == Piece::WHITE ? "white" : "black") << "'s turn" << endl;
        cout << "Enter a move: " << endl;
        cin >> input;
        if (!parseMove(input))
        {
            displayBoard();
            requestMove();
        }
    }

    shared_ptr<Piece> pieceFromSymbol(Symbol symbol, int color, int x, int y)
    {
        switch (symbol)
        {
        case Symbol::PAWN:
            return make_shared<Pawn>(color, x, y);
        case Symbol::ROOK:
            return make_shared<Rook>(color, x, y);
        case Symbol::KNIGHT:
            return make_shared<Knight>(color, x, y);
        case Symbol::BISHOP:
            return make_shared<Bishop>(color, x, y);
        case Symbol::QUEEN:
            return make_shared<Queen>(color, x, y);
        case Symbol::KING:
            return make_shared<King>(color, x, y);
        default:
            return make_shared<Piece>(-1, Symbol::EMPTY, x, y);
        }
    }

    void makeMove(shared_ptr<Piece> piece, int new_file, int new_rank)
    {
        cout << new_file << " " << new_rank << endl;
        int old_file = piece->getX();
        int old_rank = piece->getY();

        board[old_rank][old_file] = make_shared<Piece>(-1, Symbol::EMPTY, old_file, old_rank);
        piece->setX(new_file);
        piece->setY(new_rank);
        board[new_rank][new_file] = piece;
    }

    void implementCapture(int color, int x, int y)
    {
        cout << "Implementing capture " << x << " " << y << endl;
        vector<shared_ptr<Piece>> *piece_list = color == Piece::WHITE ? &black_pieces : &white_pieces;
        vector<shared_ptr<Piece>> *capture_list = color == Piece::WHITE ? &white_capture : &black_capture;
        for (vector<shared_ptr<Piece>>::iterator it = piece_list->begin(); it != piece_list->end(); ++it)
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

    bool parseMove(string input)
    {
        if (input.length() < 2)
        {
            _err("Too short");
            return false;
        }
        char file = input[input.length() - 2];
        char rank = input[input.length() - 1];

        if (!inRange(file, rank))
        {
            _err("Out of range");
            return false;
        }

        if (board[num_map[rank]][letter_map[file]]->getColor() == turn)
        {
            _err("Cannot capture own piece");
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
        cout << piece << " " << file << " " << rank << " " << capture << " " << disambiguation << endl;
        vector<shared_ptr<Piece>> valid_pieces = {};
        for (vector<shared_ptr<Piece>> row : board)
        {
            for (shared_ptr<Piece> p : row)
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
        for (shared_ptr<Piece> p : valid_pieces)
        {
            int x = letter_map[file];
            int y = num_map[rank];
            if (p->getX() == x && p->getY() == y)
                continue;
            cout << "Checking piece: " << (char)(p->getSymbol()) << " " << p->getX() << " " << p->getY() << endl;
            if (p->validMove(letter_map[file], num_map[rank], capture))
            {
                cout << "Found a piece to check: " << (char)(p->getSymbol()) << " " << p->getX() << " " << p->getY() << endl;
                if (!checkOverlap(p->getX(), p->getY(), x, y))
                {
                    _err("overlap");
                    continue;
                }
                cout << "Valid move" << endl;
                if (capture)
                    implementCapture(turn, x, y);
                makeMove(p, x, y);
                return true;
            }
        }
        _err("move doesn't work");
        return false;
    }

    void
    setupBoard()
    {
        vector<Symbol> backRow = {Symbol::ROOK, Symbol::KNIGHT, Symbol::BISHOP, Symbol::QUEEN, Symbol::KING, Symbol::BISHOP, Symbol::KNIGHT, Symbol::ROOK};
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

    void printList(vector<shared_ptr<Piece>> list)
    {
        vector<shared_ptr<Piece>> sorted_list = {};
        for (Symbol symbol : {Symbol::KING, Symbol::QUEEN, Symbol::ROOK, Symbol::BISHOP, Symbol::KNIGHT, Symbol::PAWN})
        {
            for (shared_ptr<Piece> p : list)
            {
                if (p->getSymbol() == symbol)
                {
                    sorted_list.push_back(p);
                }
            }
        }
        for (shared_ptr<Piece> p : sorted_list)
        {
            cout << Color::to_chess_piece(p->getColor(), p->getSymbol()) << " ";
        }
    }

    void displayBoard()
    {
        for (int y = board.size() - 1; y >= 0; y--)
        {
            cout << y << " ";
            cout << y + 1 << " ";
            for (int x = 0; x < board[0].size(); x++)
            {
                int color = board[y][x]->getColor();
                Symbol symbol = board[y][x]->getSymbol();
                cout << Color::getColoredString(symbol, Foreground::WHITE, (x + y) % 2 == 0 ? Background::BEIGE : Background::GREEN, color);
            }
            switch (y)
            {
            case 7:
                cout << "    White pieces: ";
                printList(white_pieces);
                break;
            case 6:
                cout << "    Black pieces: ";
                printList(black_pieces);
                break;
            case 5:
                cout << "    White captured: ";
                printList(white_capture);
                break;
            case 4:
                cout << "    Black captured: ";
                printList(black_capture);
                break;
            }
            cout << endl;
        }
        cout << "    ";
        for (int i = 0; i < 8; i++)
        {
            cout << " " << coords[i] << " ";
        }
        cout << endl
             << "    ";
        for (int i = 0; i < 8; i++)
        {
            cout << " " << i << " ";
        }
        cout << endl;
    }
};

int main()
{
    Board board = Board();
    return 0;
}
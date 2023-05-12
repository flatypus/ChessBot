#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <map>
#include "color.h"
#include "utils.h"

using namespace std;

vector<char> coords = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
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

class Piece
{
private:
    int color;
    Symbol symbol;
    int x;
    int y;

public:
    const static int WHITE = 0;
    const static int BLACK = 1;
    const static int EMPTY = -1;
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
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    virtual bool validMove(int x, int y)
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
    bool validMove(int x, int y)
    {
        cout << this->getX() << " " << this->getY() << " " << x << " " << y << endl;
        getInfo();
        bool valid = false;
        switch (this->getColor())
        {
        case Piece::WHITE:
            valid = (this->getY() == 1 && y == 3 || this->getY() + 1 == y) && this->getX() == x;
            break;
        case Piece::BLACK:
            valid = (this->getY() == 6 && y == 4 || this->getY() - 1 == y) && this->getX() == x;
            break;
        }
        cout << valid << endl;
        return valid;
    };
};

class Rook : public Piece
{
public:
    Rook(int color, int x, int y) : Piece(color, Symbol::ROOK, x, y) {}
    bool validMove(int x, int y)
    {
        return false;
    }
};

class Knight : public Piece
{
public:
    Knight(int color, int x, int y) : Piece(color, Symbol::KNIGHT, x, y) {}
    bool validMove(int x, int y)
    {
        return false;
    }
};

class Bishop : public Piece
{
public:
    Bishop(int color, int x, int y) : Piece(color, Symbol::BISHOP, x, y) {}
    bool validMove(int x, int y)
    {
        return false;
    }
};

class Queen : public Piece
{
public:
    Queen(int color, int x, int y) : Piece(color, Symbol::QUEEN, x, y) {}
    bool validMove(int x, int y)
    {
        return false;
    }
};

class King : public Piece
{
public:
    King(int color, int x, int y) : Piece(color, Symbol::KING, x, y) {}
    bool validMove(int x, int y)
    {
        return false;
    }
};

class Board
{
    mutable vector<vector<Piece *>> board;
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

    Piece *pieceFromSymbol(Symbol symbol, int color, int x, int y)
    {
        switch (symbol)
        {
        case Symbol::PAWN:
            return new Pawn(color, x, y);
        case Symbol::ROOK:
            return new Rook(color, x, y);
        case Symbol::KNIGHT:
            return new Knight(color, x, y);
        case Symbol::BISHOP:
            return new Bishop(color, x, y);
        case Symbol::QUEEN:
            return new Queen(color, x, y);
        case Symbol::KING:
            return new King(color, x, y);
        default:
            return new Piece(Piece::EMPTY, Symbol::EMPTY, x, y);
        }
    }

    void makeMove(Piece *piece, int new_file, int new_rank)
    {
        cout << new_file << " " << new_rank << endl;
        int old_file = piece->getX();
        int old_rank = piece->getY();
        board[old_rank][old_file] = new Piece(Piece::EMPTY, Symbol::EMPTY, old_file, old_rank);
        board[new_rank][new_file] = piece;
    }

    bool parseMove(string input)
    {
        if (input.length() < 2)
        {
            cout << "Invalid move" << endl;
            return false;
        }
        char piece = input.length() == 2 ? 'P' : input[0];
        char rank = input[input.length() - 1];
        char file = input[input.length() - 2];
        vector<Piece *> valid_pieces = {};
        for (vector<Piece *> row : board)
        {
            for (Piece *p : row)
            {
                if ((char)(p->getSymbol()) == piece && p->getColor() == turn)
                {
                    valid_pieces.push_back(p);
                }
            }
        }
        for (Piece *p : valid_pieces)
        {
            int x = letter_map[file];
            int y = num_map[rank];
            if (p->getX() == x && p->getY() == y)
                continue;
            if (p->validMove(letter_map[file], num_map[rank]))
            {
                cout << "valid move" << endl;
                makeMove(p, x, y);
                return true;
            }
        }
        cout << "Invalid move" << endl;
        return false;
    }

    void setupBoard()
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
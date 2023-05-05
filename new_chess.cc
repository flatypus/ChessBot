#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

// vector<char> coords = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

class Symbol
{
public:
    const static char KING = 'K';
    const static char QUEEN = 'Q';
    const static char ROOK = 'R';
    const static char BISHOP = 'B';
    const static char KNIGHT = 'N';
    const static char PAWN = 'P';
};

class Piece
{
private:
    int color;
    char symbol;
    int x;
    int y;

public:
    const static int BLACK = 0;
    const static int WHITE = 1;
    Piece() {}
    Piece(int color, char symbol, int x, int y) : color(color),
                                                  symbol(symbol),
                                                  x(x),
                                                  y(y) {}
};

class Board
{
    // vector<vector<Piece>> board;

public:
    Board()
    {
        // setupBoard();
        // displayBoard();
    }

private:
    void setupBoard()
    {
        // board.resize(8);
        // for (int i = 0; i < 8; i++)
        // {
        //     board[i].resize(8);
        // }
        // board[0] = {Piece(0, Symbol::ROOK, 0, 0), Piece(0, Symbol::KNIGHT, 1, 0), Piece(0, Symbol::BISHOP, 2, 0),
        //             Piece(0, Symbol::QUEEN, 3, 0), Piece(0, Symbol::KING, 4, 0), Piece(0, Symbol::BISHOP, 5, 0),
        //             Piece(0, Symbol::KNIGHT, 6, 0), Piece(0, Symbol::ROOK, 7, 0)};
        // board[7] = {Piece(1, Symbol::ROOK, 0, 7), Piece(1, Symbol::KNIGHT, 1, 7), Piece(1, Symbol::BISHOP, 2, 7),
        //             Piece(1, Symbol::QUEEN, 3, 7), Piece(1, Symbol::KING, 4, 7), Piece(1, Symbol::BISHOP, 5, 7),
        //             Piece(1, Symbol::KNIGHT, 6, 7), Piece(1, Symbol::ROOK, 7, 7)};
        // for (int i = 0; i < 8; i++)
        // {
        //     board[1].push_back(Piece(0, Symbol::PAWN, i, 1));
        //     board[6].push_back(Piece(1, Symbol::PAWN, i, 6));
        // }
    }
    void displayBoard()
    {
        // for (int y = 0; y < board.size(); y++)
        // {
        //     for (int x = 0; x < board[0].size(); x++)
        //     {
        //         cout << x << " " << y << endl;
        //     }
        // }
    }
};

int main()
{
    // Board board = Board();
    return 0;
}  
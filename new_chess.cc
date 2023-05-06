#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include "color.h"

using namespace std;

vector<char> coords = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

class Piece
{
private:
    int color;
    Symbol symbol;
    int x;
    int y;

public:
    const static int BLACK = 0;
    const static int WHITE = 1;
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
};

class Board
{
    mutable vector<vector<Piece>> board;

public:
    Board()
    {
        setupBoard();
        displayBoard();
    }

private:
    void setupBoard()
    {
        board.resize(8);
        for (int i = 0; i < 8; i++)
        {
            board[i].resize(8);
        }
        vector<Symbol> backRow = {Symbol::ROOK, Symbol::KNIGHT, Symbol::BISHOP,
                                  Symbol::QUEEN, Symbol::KING, Symbol::BISHOP,
                                  Symbol::KNIGHT, Symbol::ROOK};
        for (int i = 0; i < 8; i++)
        {
            board[0][i] = Piece(Piece::BLACK, backRow[i], i, 0);
            board[1][i] = Piece(Piece::BLACK, Symbol::PAWN, i, 1);
            for (int j = 2; j < 6; j++)
            {
                board[j][i] = Piece(Piece::BLACK, Symbol::EMPTY, i, j);
            }
            board[6][i] = Piece(Piece::WHITE, Symbol::PAWN, i, 6);
            board[7][i] = Piece(Piece::WHITE, backRow[i], i, 7);
        }
    }
    void displayBoard()
    {
        for (int y = 0; y < board.size(); y++)
        {
            for (int x = 0; x < board[0].size(); x++)
            {
                int color = board[y][x].getColor();
                Symbol symbol = board[y][x].getSymbol();
                // green and lightbrown/beige
                // alternating color scheme with modulus
                cout << Color::getColoredString(symbol, Foreground::WHITE, (x + y) % 2 == 0 ? Background::BEIGE : Background::GREEN, color);
            }
            cout << endl;
        }
    }
};

int main()
{
    Board board = Board();
    // cout << Color::getColoredString("Hello", Foreground::RED, Background::BRIGHT_WHITE) << endl;
    return 0;
}
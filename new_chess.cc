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
    {'1', 7},
    {'2', 6},
    {'3', 5},
    {'4', 4},
    {'5', 3},
    {'6', 2},
    {'7', 1},
    {'8', 0}};

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
            cout << "Invalid move" << endl;
            requestMove();
        }
    }

    bool makeMove(int file, int rank)
    {
        // find pawn on file
        for (int i = 0; i < board.size(); i++)
        {
            if (board[i][file]->getSymbol() == Symbol::PAWN && board[i][file]->getColor() == turn)
            {
                // found move
                Piece *piece = board[i][file];
                board[i][file] = new Piece(Piece::EMPTY, Symbol::EMPTY, file, i);
                board[rank][file] = piece;
                return true;
            }
        }
        return false;
    }

    bool parseMove(string move)
    {
        bool valid = true;
        vector<string> split_move = split(move, 'x');
        if (split_move.size() == 2)
        {
            // takes
            string piece = split_move[0];
            vector<char> piece_coordinate = to_list(piece);
            vector<char> target_list = to_list(split_move[1]);
            cout << split_move[0] << " " << split_move[1] << endl;
        }
        else
        {
            // just a move
            vector<char> move_list = to_list(split_move[0]);
            switch (move_list.size())
            {
            case 2:
                // pawn move up
                cout << "Move the " << move_list[0] << " pawn to " << move_list[0] << move_list[1] << endl;
                if (!inRange(move_list[0], move_list[1]))
                {
                    valid = false;
                }
                else
                {
                    int file = letter_map[move_list[0]];
                    int rank = num_map[move_list[1]];
                    valid = makeMove(file, rank);
                }
                break;
            case 3:
                // piece is RNBKQ
                cout << "Move the " << move_list[0] << " piece to " << move_list[1] << move_list[2] << endl;
                break;
            case 4:
                // RNBKQ disambiguation
                cout << "Move the " << move_list[0] << " piece on " << move_list[1] << " to " << move_list[2] << move_list[3] << endl;
                break;
            }
        }
        return valid;
    }

    void setupBoard()
    {
        vector<Symbol> backRow = {Symbol::ROOK, Symbol::KNIGHT, Symbol::BISHOP,
                                  Symbol::QUEEN, Symbol::KING, Symbol::BISHOP,
                                  Symbol::KNIGHT, Symbol::ROOK};
        board.resize(8);
        for (int i = 0; i < 8; i++)
        {
            board[7].push_back(new Piece(Piece::WHITE, backRow[i], i, 0));

            board[6].push_back(new Piece(Piece::WHITE, Symbol::PAWN, i, 1));
            for (int j = 2; j < 6; j++)
            {
                board[j].push_back(new Piece(Piece::EMPTY, Symbol::EMPTY, i, j));
            }
            board[1].push_back(new Piece(Piece::BLACK, Symbol::PAWN, i, 6));
            board[0].push_back(new Piece(Piece::BLACK, backRow[i], i, 7));
        }
    }
    void displayBoard()
    {
        for (int y = 0; y < board.size(); y++)
        {
            cout << 8 - y << " ";
            for (int x = 0; x < board[0].size(); x++)
            {
                int color = board[y][x]->getColor();
                Symbol symbol = board[y][x]->getSymbol();
                cout << Color::getColoredString(symbol, Foreground::WHITE, (x + y) % 2 == 0 ? Background::BEIGE : Background::GREEN, color);
            }
            cout << endl;
        }
        cout << "  ";
        for (int i = 0; i < 8; i++)
        {
            cout << " " << coords[i] << " ";
        }
        cout << endl;
    }
};

int main()
{
    Board board = Board();
    return 0;
}
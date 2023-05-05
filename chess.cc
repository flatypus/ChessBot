#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

std::vector<char> coords = {'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};

class Piece
{
public:
    const static int BLACK = 0;
    const static int WHITE = 1;

    Piece() {}
    Piece(char symbol) : symbol(symbol) {}
    Piece(char color, int symbol) : symbol(symbol), color(color) {}
    Piece(char color, int symbol, int x, int y) : symbol(symbol), color(color), x(x), y(y) {}

    int getColor() const
    {
        return color;
    }
    char getSymbol() const
    {
        return symbol;
    }
    int getX() const
    {
        return x;
    }
    int getY() const
    {
        return y;
    }

    void setColor(int color)
    {
        this->color = color;
    }
    void setSymbol(char symbol)
    {
        this->symbol = symbol;
    }
    void setX(int x)
    {
        this->x = x;
    }
    void setY(int y)
    {
        this->y = y;
    }

private:
    int color;
    int x;
    int y;
    char symbol;
};

class ChessBoard
{
public:
    ChessBoard()
    {
        const int SIZE = 8;
        const vector<Piece> FIRST_RANK{
            Piece(Piece::BLACK, 'R', 0, 0), Piece(Piece::BLACK, 'N', 1, 0), Piece(Piece::BLACK, 'B', 2, 0),
            Piece(Piece::BLACK, 'Q', 3, 0), Piece(Piece::BLACK, 'K', 4, 0), Piece(Piece::BLACK, 'B', 5, 0),
            Piece(Piece::BLACK, 'N', 6, 0), Piece(Piece::BLACK, 'R', 7, 0)};
        vector<Piece> SECOND_RANK(SIZE, Piece(Piece::BLACK, 'P', 0, 1));

        for (int x = 0; x < SIZE; x++)
        {
            SECOND_RANK[x].setX(x);
        }

        board.resize(SIZE, vector<Piece>(SIZE, Piece()));
        board[0] = FIRST_RANK;
        board[1] = SECOND_RANK;
        for (int i = 2; i < SIZE - 2; ++i)
        {
            board[i] = vector<Piece>(SIZE, Piece('.'));
        }
        vector<Piece> SEVENTH_RANK(SIZE, Piece(Piece::WHITE, 'P', 0, 6));

        for (int x = 0; x < SIZE; x++)
        {
            SEVENTH_RANK[x].setX(x);
        }

        board[SIZE - 2] = SEVENTH_RANK;
        const vector<Piece> EIGHTH_RANK{
            Piece(Piece::WHITE, 'R', 0, 7), Piece(Piece::WHITE, 'N', 1, 7), Piece(Piece::WHITE, 'B', 2, 7),
            Piece(Piece::WHITE, 'Q', 3, 7), Piece(Piece::WHITE, 'K', 4, 7), Piece(Piece::WHITE, 'B', 5, 7),
            Piece(Piece::WHITE, 'N', 6, 7), Piece(Piece::WHITE, 'R', 7, 7)};
        board[SIZE - 1] = EIGHTH_RANK;
    }

    const vector<vector<Piece>> &getBoard() const
    {
        return board;
    }

    void setPiece(int x, int y, Piece piece)
    {
        board[piece.getY()][piece.getX()].setSymbol('.');
        board[x][y] = piece;
    }

private:
    vector<vector<Piece>> board;
};

void printBoard(const ChessBoard &board)
{
    const auto &b = board.getBoard();
    for (const auto &row : b)
    {
        for (const auto &piece : row)
        {
            cout << piece.getSymbol() << " ";
        }
        cout << endl;
    }
}

bool checkMove(int startX, int startY, int endX, int endY, char symbol)
{
    if (startX == endX && startY == endY)
    {
        cout << "Cannot move to same location";
        return false;
    }
    cout << startX << " " << startY << " " << endX << " " << endY << " " << symbol << endl;
    switch (symbol)
    {
    case 'K':
        if (abs(startX - endX) > 1 || abs(startY - endY) > 1)
        {
            cout << "Invalid move for King";
            return false;
        }
    }
    return true;
}

void doMove(const string &move, ChessBoard &board, int moveNumber)
{ // rn only works for three letter things like ke5...
    // cout<<move[2]<<endl;
    char m = move[0];
    // Piece moved(moveNumber%2, toupper(m));
    Piece moved;
    const auto &b = board.getBoard();
    for (const auto &row : b)
    {
        for (const auto &piece : row)
        {
            if (piece.getSymbol() == toupper(m) && piece.getColor() == moveNumber % 2)
            {
                moved = piece;
            }
        }
    }
    char horizontalCoord = move[1];
    int verticalCoord = move[2] - '0';
    int verticalInt = 0;

    for (int y = 1; y < 8; y++)
    {
        if (y == verticalCoord)
        {
            for (vector<char>::iterator t = coords.begin(); t != coords.end(); ++t)
            {
                if (*t == horizontalCoord)
                {
                    cout << horizontalCoord - 1 << ", " << verticalInt << endl;
                    cout << moved.getX() << ", " << moved.getY() << endl;
                    if (checkMove(moved.getX(), moved.getY(), horizontalCoord, verticalInt, moved.getSymbol()))
                    {
                        board.setPiece(verticalInt, horizontalCoord - 1, moved);
                    }
                    break;
                }
            }
        }
        verticalInt++;
    }
}

int main()
{
    std::string move;
    int moveNumber = 1;
    ChessBoard board;
    printBoard(board);

    while (true)
    {
        std::cout << "Enter move: ";
        std::cin >> move;
        // if(moveNumber%2 == 0)
        doMove(move, board, moveNumber);
        printBoard(board);
        moveNumber++;
    }
    return 0;
}
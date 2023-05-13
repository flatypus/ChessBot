#include <string>
#include <unordered_map>
#include "types.h"

using namespace std;

enum class Foreground
{
    BLACK = 30,
    RED = 31,
    GREEN = 32,
    YELLOW = 33,
    BLUE = 34,
    MAGENTA = 35,
    WHITE = 37,
};

enum class Background
{
    BLACK = 40,
    RED = 41,
    GREEN = 42,
    BLUE = 44,
    MAGENTA = 45,
    WHITE = 47,
    BEIGE = -1
};

class Color
{
public:
    static string to_chess_piece(int color, Symbol symbol)
    {
        unordered_map<Symbol, string> white_piece_map = {
            {Symbol::KING, u8"\u2654"},
            {Symbol::QUEEN, u8"\u2655"},
            {Symbol::ROOK, u8"\u2656"},
            {Symbol::BISHOP, u8"\u2657"},
            {Symbol::KNIGHT, u8"\u2658"},
            {Symbol::PAWN, u8"\u2659"},
            {Symbol::EMPTY, " "}};

        // black piece hashmap
        unordered_map<Symbol, string> black_piece_map = {
            {Symbol::KING, u8"\u265A"},
            {Symbol::QUEEN, u8"\u265B"},
            {Symbol::ROOK, u8"\u265C"},
            {Symbol::BISHOP, u8"\u265D"},
            {Symbol::KNIGHT, u8"\u265E"},
            {Symbol::PAWN, u8"\u265F"},
            {Symbol::EMPTY, " "}};

        switch (color)
        {
        case 0:
            return white_piece_map[symbol];
        case 1:
            return black_piece_map[symbol];
        default:
            return " ";
        }
    }
    static string to_String(Foreground fg)
    {
        return to_string(static_cast<int>(fg));
    }
    static string to_String(Background bg)
    {
        switch (bg)
        {
        case Background::BEIGE:
            return "48;2;255;255;204";
        default:
            break;
        }
        return to_string(static_cast<int>(bg));
    }

    static string getColoredString(Symbol symbol, Foreground fg, Background bg, int color)
    {
        string stringified_symbol = to_string(static_cast<int>(symbol));
        string result = "\033[" + to_String(fg) + ";" + to_String(bg) + ";24m " + to_chess_piece(color, symbol) + " \033[0m";
        return result;
    }

    static string getColoredString(string s, Foreground fg, Background bg, int color)
    {
        string result = "\033[" + to_String(fg) + ";" + to_String(bg) + ";24m " + s + " \033[0m";
        return result;
    }
};

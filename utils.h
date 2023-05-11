#include <string>
#include <vector>

using namespace std;

extern int count(string s, char target)
{
    int a = 0;
    for (char c : s)
    {
        if (target == c)
            a++;
    }
    return a;
}

extern vector<char> to_list(string s)
{
    vector<char> l = {};
    for (char c : s)
    {
        l.push_back(c);
    }
    return l;
}

extern vector<string> split(string s, char target)
{
    vector<string> l = {};
    string running = "";
    s = s + target;
    for (char c : s)
    {
        if (c == target)
        {
            l.push_back(running);
            running = "";
        }
        else
        {
            running = running + c;
        }
    }
    return l;
}

extern bool is_upper(char x)
{
    return x >= 'A' && x <= 'Z';
}
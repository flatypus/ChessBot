#include <string>
#include <vector>

extern int count(std::string s, char target)
{
    int a = 0;
    for (char c : s)
    {
        if (target == c)
            a++;
    }
    return a;
}

extern std::vector<char> to_list(std::string s)
{
    std::vector<char> l = {};
    for (char c : s)
    {
        l.push_back(c);
    }
    return l;
}

extern std::vector<std::string> split(std::string s, char target)
{
    std::vector<std::string> l = {};
    std::string running = "";
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
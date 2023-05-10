#include <string>
#include <vector>

using namespace std;

class Utils
{
private:
    int count(string s, char target)
    {
        int a = 0;
        for (char c : s)
        {
            if (target == c)
                a++;
        }
        return a;
    }
    vector<string> split(string s, char target)
    {
        vector<string> l = {};
        vector<char> running = {};
        for (char c : s)
        {
            
        }
    }
}
#include <string>
#include <iostream>
using namespace std;
void findstr(string word, string sub)
{
    static string::size_type idx = 0;
    static int count = 1;
    static string::size_type temp = 0;
    while (idx < word.length() - sub.length())
    {
        temp = word.find(sub) + 1;
        idx += temp;
        if (temp != string::npos)
        {
            cout << "第" << count << "个位子：" << idx << endl;
        }
        if (idx + 1 < word.length() - sub.length())
        {
            count++;
            findstr(word.substr(idx), sub);
        }
    }
}

int main(int argc, char const *argv[])
{
    string name;
    cin >> name;
    findstr(name, "hello");
    return 0;
}

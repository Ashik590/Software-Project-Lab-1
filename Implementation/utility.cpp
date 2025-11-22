#include <bits/stdc++.h>
#include "../Headers/utility.h"
using namespace std;
#define nl "\n"

// Blues
void print_text_blue(string s)
{
    cout << "\033[34m" << s << "\033[0m";
}
void print_text_blue_bold(string s)
{
    cout << "\033[1;34m" << s << "\033[0m";
}
void print_text_blue_fade(string s)
{
    cout << "\033[2;34m" << s << "\033[0m";
}
void print_text_blue_italic(string s)
{
    cout << "\033[3;34m" << s << "\033[0m";
}
void print_text_blue_underline(string s)
{
    cout << "\033[4;34m" << s << "\033[0m";
}

// Reds
void print_text_red(string s)
{
    cout << "\033[31m" << s << "\033[0m";
}
void print_text_red_bold(string s)
{
    cout << "\033[1;31m" << s << "\033[0m";
}
void print_text_red_fade(string s)
{
    cout << "\033[2;31m" << s << "\033[0m";
}
void print_text_red_italic(string s)
{
    cout << "\033[3;31m" << s << "\033[0m";
}
void print_text_red_underline(string s)
{
    cout << "\033[4;31m" << s << "\033[0m";
}

// Yellows
void print_text_yellow(string s)
{
    cout << "\033[33m" << s << "\033[0m";
}
void print_text_yellow_bold(string s)
{
    cout << "\033[1;33m" << s << "\033[0m";
}
void print_text_yellow_fade(string s)
{
    cout << "\033[2;33m" << s << "\033[0m";
}
void print_text_yellow_italic(string s)
{
    cout << "\033[3;33m" << s << "\033[0m";
}
void print_text_yellow_underline(string s)
{
    cout << "\033[4;33m" << s << "\033[0m";
}

// Greens
void print_text_green(string s)
{
    cout << "\033[32m" << s << "\033[0m";
}
void print_text_green_bold(string s)
{
    cout << "\033[1;32m" << s << "\033[0m";
}
void print_text_green_fade(string s)
{
    cout << "\033[2;32m" << s << "\033[0m";
}
void print_text_green_italic(string s)
{
    cout << "\033[3;32m" << s << "\033[0m";
}
void print_text_green_underline(string s)
{
    cout << "\033[4;32m" << s << "\033[0m";
}

// Magentas
void print_text_magenta(string s)
{
    cout << "\033[35m" << s << "\033[0m";
}
void print_text_magenta_bold(string s)
{
    cout << "\033[1;35m" << s << "\033[0m";
}
void print_text_magenta_fade(string s)
{
    cout << "\033[2;35m" << s << "\033[0m";
}
void print_text_magenta_italic(string s)
{
    cout << "\033[3;35m" << s << "\033[0m";
}
void print_text_magenta_underline(string s)
{
    cout << "\033[4;35m" << s << "\033[0m";
}

// Cyans
void print_text_cyan(string s)
{
    cout << "\033[36m" << s << "\033[0m";
}
void print_text_cyan_bold(string s)
{
    cout << "\033[1;36m" << s << "\033[0m";
}
void print_text_cyan_fade(string s)
{
    cout << "\033[2;36m" << s << "\033[0m";
}
void print_text_cyan_italic(string s)
{
    cout << "\033[3;36m" << s << "\033[0m";
}
void print_text_cyan_underline(string s)
{
    cout << "\033[4;36m" << s << "\033[0m";
}

string trim(string s)
{
    string newStr;
    int firstChAppear = 0;

    for (auto ch : s)
    {
        if (ch != ' ')
            firstChAppear = 1;

        if (firstChAppear)
            newStr.push_back(ch);
    }

    while (newStr[newStr.size() - 1] == ' ')
    {
        newStr.pop_back();
    }

    return newStr;
}

string to_lower_str(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        s[i] = tolower(s[i]);
    }

    return s;
}

int menu_bar(string head, vector<string> options)
{
    char n;
    while (1)
    {
        cout << nl;
        print_text_yellow_bold("Select " + head + " Options - ");
        cout << nl;

        for (int i = 0; i < options.size(); i++)
        {
            print_text_green("(" + to_string(i + 1) + ") ");
            cout << options[i] << nl;
        }

        print_text_yellow_fade("=> ");
        cin >> n;
        n -= '0';

        if (n > 0 && n <= options.size())
            break;
        else
        {
            print_text_red("Wrong Selection !");
            cout << nl;
        }
    }

    return n;
}

bool findWord(string line, string keyword)
{
    bool read = 1;
    bool condition = 0;
    int ind = 0;
    for (int i = 0; i < line.size(); i++)
    {
        if (!(line[i] >= 65 && line[i] <= 90) && !(line[i] >= 97 && line[i] <= 122))
        {
            if (ind == keyword.size() && read)
                condition = 1;
            read = 1;
            ind = 0;
        }
        else if (read && ind < keyword.size() && line[i] == keyword[ind])
            ind++;
        else
            read = 0;
    }
    if (ind == keyword.size() && read)
        condition = 1;

    return condition;
}
#include <bits/stdc++.h>
#include "../Headers/utility.h"
#include "../Headers/levenshtein_distance.h"
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

bool isLetter(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '-';
}

vector<int> findWord(string line, string keyword)
{
    vector<int> pos;
    bool read = 1;
    int ind = 0;
    for (int i = 0; i < line.size(); i++)
    {
        if (!isLetter(line[i]))
        {
            if (ind == keyword.size() && read)
                pos.push_back(i - ind);

            read = 1;
            ind = 0;
        }
        else if (read && ind < keyword.size() && line[i] == keyword[ind])
            ind++;
        else
            read = 0;
    }

    if (ind == keyword.size() && read)
        pos.push_back(line.size() - ind);

    return pos;
}

vector<int> getLPS(string &pattern)
{
    int n = pattern.size();
    vector<int> LPS(n);

    int i = 1;
    LPS[0] = 0;
    int len = 0;

    while (i < n)
    {
        if (pattern[i] == pattern[len])
        {
            LPS[i++] = ++len;
        }
        else
        {
            if (len == 0)
                LPS[i++] = 0;
            else
                len = LPS[len - 1];
        }
    }

    return LPS;
}

vector<int> find_KMP(string &str, string &pattern)
{
    vector<int> findings;

    int j = 0;
    int i = 0;

    int n = str.size();
    int m = pattern.size();

    vector<int> LPS = getLPS(pattern);

    while (i < n)
    {
        if (pattern[j] == str[i])
        {
            i++;
            j++;
        }

        if (j == m)
        { // found one
            findings.push_back(i - j);
            j = LPS[j - 1];
        }
        else if (i < n && pattern[j] != str[i])
        {
            if (j > 0)
                j = LPS[j - 1];
            else
                i++;
        }
    }

    return findings;
}

pair<vector<int>, set<string>> findWordOrGetSug(string &str, string &keyword)
{
    vector<int> pos;
    bool read = 1;
    int ind = 0;

    string currentWord;
    set<string> sugStrs;

    for (int i = 0; i < str.size(); i++)
    {
        if (!isLetter(str[i]))
        {
            if (ind == keyword.size() && read)
                pos.push_back(i - ind);
            else if (ind > 0) // First character has to be matched
            {
                int minEdit = lvnstn_dis(currentWord, keyword);

                if (minEdit < max(keyword.size(), currentWord.size()) / 2)
                    sugStrs.insert(currentWord);
            }

            currentWord = "";
            read = 1;
            ind = 0;
        }
        else if (read && ind <= keyword.size() && str[i] == keyword[ind])
        {
            ind++;
            currentWord.push_back(str[i]);
        }
        else
        {
            read = 0;
            currentWord.push_back(str[i]);
        }
    }

    if (ind == keyword.size() && read)
        pos.push_back(str.size() - ind);
    else
    {
        int minEdit = lvnstn_dis(currentWord, keyword);

        if (minEdit <= max(keyword.size(), currentWord.size()) / 2)
            sugStrs.insert(currentWord);
    }

    pair<vector<int>, set<string>> returnVal = {pos, sugStrs};

    return returnVal;
}
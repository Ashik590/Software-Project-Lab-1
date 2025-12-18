#include <bits/stdc++.h>
#include "../Headers/soundex.h"
using namespace std;

string soundex(string word)
{
    if (!word.size())
        return "";

    for (auto &e : word)
        e = toupper(e);

    char charMapping[26];

    for (char i = 'A'; i <= 'Z'; i++)
    {
        if (i == 'B' || i == 'F' || i == 'P' || i == 'V')
            charMapping[i - 'A'] = '1';
        else if (i == 'C' || i == 'G' || i == 'J' || i == 'K' ||
                 i == 'Q' || i == 'S' || i == 'X' || i == 'Z')
            charMapping[i - 'A'] = '2';
        else if (i == 'D' || i == 'T')
            charMapping[i - 'A'] = '3';
        else if (i == 'L')
            charMapping[i - 'A'] = '4';
        else if (i == 'M' || i == 'N')
            charMapping[i - 'A'] = '5';
        else if (i == 'R')
            charMapping[i - 'A'] = '6';
        else
            charMapping[i - 'A'] = '0';
    }

    char firstChar = word[0];
    string result = "";
    result += firstChar;

    char prevCode = charMapping[firstChar - 'A'];
    char curCode, curChar;

    for (int i = 1; i < word.size(); i++)
    {
        curChar = word[i];
        curCode = charMapping[curChar - 'A'];

        if (curCode != '0' && curCode != prevCode)
            result += curCode;

        prevCode = curCode;
    }

    while (result.size() > 4)
        result.pop_back();

    while (result.size() < 4)
        result.push_back('0');

    return result;
}

bool soundex_check(string &code, string &word)
{
    string codeAnother = soundex(word);

    return code == codeAnother;
}
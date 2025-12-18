#include <bits/stdc++.h>
#include "../Headers/jaro_winkler.h"
using namespace std;
#define nl "\n"

double jaro_winkler_implementation(string &word, string &keyword)
{
    if (!word.size() || !keyword.size())
        return 0;

    int match_distance = max(word.size(), keyword.size()) / 2 - 1;

    if (match_distance < 0)
        match_distance = 0;

    vector<bool> mWord(word.size(), false), mKeyword(keyword.size(), false);

    double matches = 0;

    for (int i = 0; i < word.size(); i++)
    {
        int lo = max(0, i - match_distance);
        int hi = min((int)keyword.size() - 1, i + match_distance);

        for (int j = lo; j <= hi; j++)
        {
            if (word[i] == keyword[j] && !mKeyword[j])
            {
                mWord[i] = true;
                mKeyword[j] = true;
                matches++;
                break;
            }
        }
    }

    if (!matches)
        return 0;

    int j = 0;
    double transpositions = 0;

    for (int i = 0; i < word.size(); i++)
    {
        if (mWord[i])
        {
            while (j < keyword.size() && !mKeyword[j])
                j++;

            if (j < keyword.size() && word[i] != keyword[j])
                transpositions++;

            j++;
        }
    }

    double jaro_score = ((double)1 / 3) * (matches / word.size() +
                                           matches / keyword.size() +
                                           (matches - transpositions / 2) / matches);

    double prefix_len = 0;

    for (int i = 0; i < min({4, (int)word.size(), (int)keyword.size()}); i++)
    {
        if (word[i] == keyword[i])
            prefix_len++;
        else
            break;
    }

    double scaling_factor = 0.1;

    double jaro_winkler_score = jaro_score + (prefix_len * scaling_factor * (1 - jaro_score));

    return jaro_winkler_score;
}

bool jaro_winkler(string &word, string &keyword)
{
    double sc = jaro_winkler_implementation(word, keyword);

    return (sc >= 0.8);
}
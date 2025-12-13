#include <bits/stdc++.h>
#include "../Headers/levenshtein_distance.h"
using namespace std;
#define nl "\n"

int recurse(vector<vector<int>> &dp, string &word, string &keyword, int i, int j)
{
    if (dp[i][j] != -1)
        return dp[i][j];

    int edit;
    if (word[i - 1] == keyword[j - 1])
        edit = recurse(dp, word, keyword, i - 1, j - 1);
    else
        edit = 1 + min({recurse(dp, word, keyword, i, j - 1),
                        recurse(dp, word, keyword, i - 1, j),
                        recurse(dp, word, keyword, i - 1, j - 1)});

    dp[i][j] = edit;

    return edit;
}

int lvnstn_dis(string &word, string &keyword)
{
    int n = word.size();
    int m = keyword.size();

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, -1));

    dp[0][0] = 0;

    for (int i = 1; i <= m; i++)
        dp[0][i] = i;

    for (int i = 1; i <= n; i++)
        dp[i][0] = i;

    int min_edit = recurse(dp, word, keyword, n, m);

    return min_edit;
}
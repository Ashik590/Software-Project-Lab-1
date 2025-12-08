#include <bits/stdc++.h>
#include "./Headers/pdf_search.h"
#include "./Headers/utility.h"
using namespace std;
#define nl "\n"
#define ll long long

// Compilation command
// g++ main.cpp Implementation/* `pkg-config --cflags --libs poppler-cpp`

int main()
{
    int mode;

    vector<string> mode_options = {
        "PDF Content Search",
        "BMP Human Skin Search"};

    mode = menu_bar((string) "Mode", mode_options);

    if (mode == 1)
    {
        // Testing --
        // vector<string> keywords(1);
        // cin.ignore();
        // getline(cin, keywords[0]);
        // vector<string> s = {"Files"};
        // pdf_search_func(s, 2, 1, 1, 1, keywords);
        // Testing --

        vector<string> roots;

        int searchScope;

        vector<string> searchScope_options = {
            "Whole Computer",
            "Specify Roots"};
        searchScope = menu_bar("Search Scope", searchScope_options);

        if (searchScope == 1)
        {
            fs::path root = "/";
            roots.push_back(root);
        }
        else
        {
            cout << "Number of roots = ";

            int num_of_roots;
            cin >> num_of_roots;

            cout << "Enter roots =>" << nl;
            for (int i = 0; i < num_of_roots; i++)
            {
                string root;
                cout << i + 1 << " - ";
                cin >> root;
                roots.push_back(root);
            }
        }

        int searchDepth;
        vector<string> searchDepth_options = {
            "With Sub Directories",
            "Without Sub Directories"};
        searchDepth = menu_bar("Search Depth", searchDepth_options);

        int isWord;
        vector<string> isWord_options = {
            "Whole Word",
            "Partial word"};

        isWord = menu_bar("", isWord_options);

        bool isDeep = 0;
        if (isWord == 1)
        {
            vector<string> isDeep_options = {
                "Quick Search",
                "Deep Search (With Suggestion)"};

            isDeep = menu_bar("Search Level", isDeep_options) - 1;
        }

        int keywordMode;
        vector<string> keywordMode_options = {
            "Single Keyword Search",
            "Multiple Keyword Search",
            "Multiple Keyword in Single Page Search"};

        keywordMode = menu_bar("Keyword Mode", keywordMode_options);

        int num_of_keyword;

        if (keywordMode != 1)
        {
            cout << "Number of Keywords => ";
            cin >> num_of_keyword;
        }
        else
            num_of_keyword = 1;

        cin.ignore();
        cout << "Enter keywords" << nl;
        vector<string> keywords(num_of_keyword);
        for (int i = 0; i < num_of_keyword; i++)
        {
            cout << i + 1 << " - ";
            getline(cin, keywords[i]);
        }

        for (auto &k : keywords)
            k = trim(k);

        int caseSensitivity;
        vector<string> caseSensitivity_options = {
            "Case Sensitive",
            "Case Insensitive"};

        caseSensitivity = menu_bar("Case Sensitivity", caseSensitivity_options);

        if (caseSensitivity == 2)
        {
            for (auto &k : keywords)
                k = to_lower_str(k);
        }

        pdf_search_func(roots, searchDepth, isWord, keywordMode, caseSensitivity, keywords, isDeep);
    }
    else
    {
        cout << "Not Available Right Now" << nl;
    }

    return 0;
}
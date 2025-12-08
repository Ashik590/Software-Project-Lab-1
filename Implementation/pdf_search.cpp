#include <bits/stdc++.h>
#include <poppler-document.h>
#include <poppler-page.h>
#include "../Headers/utility.h"
#include "../Headers/pdf_utility.h"
#include "../Headers/pdf_search.h"
#define nl "\n"
#define ll long long

using namespace std;
using namespace poppler;

namespace fs = std::filesystem;

struct pageResult
{
    int pageNumber;
    vector<string> findings;
    set<string> suggestions;
};

struct FileInfo
{
    string fileName;
    vector<pageResult> results;
};

FileInfo searchKEYWORD(fs::path pathName, vector<string> keywords, int isWord, int caseSensitivity, bool isDeep)
{
    FileInfo FI;
    FI.fileName = pathName;

    unique_ptr<document> pdf(document::load_from_file(pathName));

    if (!pdf)
    {
        cout << "Cannot open PDF file!" << nl;
        return FI;
    }

    int numPages = pdf->pages();

    // Iterate through all pages
    for (int i = 0; i < numPages; i++)
    {
        unique_ptr<page> p(pdf->create_page(i));
        if (!p)
            continue;

        // Get text from the page as a string
        auto pageBytes = p->text();
        string pageText(pageBytes.begin(), pageBytes.end());

        if (caseSensitivity == 2)
            pageText = to_lower_str(pageText);

        pageResult res;
        res.pageNumber = i + 1;

        // Formatting the text content into a good shape
        string line;
        vector<string> lines;
        string pageContent;

        for (auto e : pageText)
        {
            if (e == '\n')
            {
                line = trim(line);

                if (line.size())
                {
                    if (line.back() != '-')
                        line += " ";
                    lines.push_back(line);
                    pageContent += line;
                }

                line.clear();
            }
            else
                line.push_back(e);
        }
        if (line.size())
        {
            if (line.back() != '-') // A line will be added of a space unless it has '-' in last
                line += " ";
            lines.push_back(line);
            line.clear();
        }

        map<int, int> lineNumberOfChars; // Which character belongs to which line
        int currentCharNum = 1;

        for (int i_line = 0; i_line < lines.size(); i_line++)
        {
            for (auto ch : lines[i_line])
                lineNumberOfChars[currentCharNum++] = i_line + 1;
        }

        set<int> found_line_nums; // Resulting line numbers
        if (isWord == 1)
        {
            for (auto key : keywords)
            {
                vector<int> pos;
                if (isDeep)
                {
                    pair<vector<int>, set<string>> deepResult = findWordOrGetSug(pageContent, key);                            

                    pos = deepResult.first;

                    for (auto sg: deepResult.second)
                        res.suggestions.insert(sg);
                }
                else
                {
                    pos = findWord(pageContent, key);
                }
                for (auto position : pos)
                {
                    int lineNumber = lineNumberOfChars[position + 1];
                    int extraLine = lineNumberOfChars[position + key.size()];

                    while (lineNumber <= extraLine)
                        found_line_nums.insert(lineNumber++);
                }
            }
        }
        else // May not be a word
        {
            for (auto key : keywords)
            {
                vector<int> pos = find_KMP(pageContent, key);

                for (auto position : pos)
                {
                    int lineNumber = lineNumberOfChars[position + 1];
                    int extraLine = lineNumberOfChars[position + key.size()];

                    while (lineNumber <= extraLine)
                        found_line_nums.insert(lineNumber++);
                }
            }
        }

        for (auto i_f_line = found_line_nums.begin(); i_f_line != found_line_nums.end(); i_f_line++)
        {
            int indexOfLine = *i_f_line;
            res.findings.push_back("(" + to_string(indexOfLine) + "/" + to_string(lines.size()) + ") " + lines[indexOfLine - 1]);
        }

        if (res.findings.size() || res.suggestions.size())
            FI.results.push_back(res);
    }

    return FI;
}

void pdf_search_func(vector<string> roots, int searchDepth, int isWord, int keywordMode, int caseSensitivity, vector<string> keywords, bool isDeep)
{
    // fs::path root = "Files";
    vector<fs::path> pathRoots(roots.begin(), roots.end());

    for (auto root : roots)
    {
        if (!fs::exists(root))
        {
            cout << "\"" << root << "\" " << "this path doesnt exist..." << nl;
            return;
        }
    }

    vector<FileInfo> results;

    for (auto root : pathRoots)
    {
        if (searchDepth == 1)
        {
            for (const auto &entry : fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied))
            {
                if (!isPDF(entry.path()))
                    continue;

                FileInfo FI = searchKEYWORD(entry.path(), keywords, isWord, caseSensitivity, isDeep);

                if (FI.results.size())
                {
                    results.push_back(FI);
                }
            }
        }
        else
        {
            for (const auto &entry : fs::directory_iterator(root, fs::directory_options::skip_permission_denied))
            {
                if (!isPDF(entry.path()))
                    continue;

                FileInfo FI = searchKEYWORD(entry.path(), keywords, isWord, caseSensitivity, isDeep);

                if (FI.results.size())
                {
                    results.push_back(FI);
                }
            }
        }
    }

    vector<pair<string, int>> serials;

    for (auto FI : results)
    {
        print_text_red(FI.fileName);
        cout << nl;

        for (auto page : FI.results)
        {
            cout << serials.size() + 1 << ") Page No - " << page.pageNumber << " :" << nl;

            serials.push_back({FI.fileName, page.pageNumber});

            for (auto line : page.findings)
            {
                cout << "- ";
                print_text_blue(line);
                cout << nl;
            }

            int indSug = 0;
            print_text_cyan("Suggestions : ");
            for (auto k = page.suggestions.begin(); k != page.suggestions.end(); k++)
            {
                print_text_cyan_fade(*k);
                if (++indSug == page.suggestions.size())
                    cout << nl;
                else
                    print_text_cyan_fade(", ");
            }
            cout << nl;
        }
        cout << nl;
    }

    if (results.size())
    {
        while (1)
        {
            int ind;
            cout << "Which one you want to open? (0 for exit)" << nl;
            cout << "=> ";
            cin >> ind;

            if (ind == 0)
                break;
            else
            {
                openPDF(serials[ind - 1].first, serials[ind - 1].second);
            }
        }
    }
    else
        cout << "No results found" << nl;
}
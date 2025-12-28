#include <poppler-document.h>
#include <poppler-page.h>
#include "../Headers/utility.h"
#include "../Headers/pdf_utility.h"
#include "../Headers/pdf_search.h"
#include "../Headers/soundex.h"
#include "../Headers/cache.h"
#define nl "\n"
#define ll long long

using namespace std;
using namespace poppler;

namespace fs = std::filesystem;

struct pageResult
{
    int pageNumber;
    vector<string> findings;
    set<string> suggestions_LV;
    set<string> suggestions_JW;
    set<string> suggestions_SD;
};

struct FileInfo
{
    string fileName;
    bool cached;
    vector<pageResult> results;
};

FileInfo searchKEYWORD(fs::path pathName, vector<string> keywords, int isWord, int caseSensitivity, bool isDeep, vector<string> keywordCodes)
{
    FileInfo FI;
    FI.fileName = pathName;
    int numPages;

    vector<vector<string>> cacheData;
    vector<vector<string>> dataToBeCached;

    if (has_cache_valid_copy(pathName))
        FI.cached = 1;

    unique_ptr<document> pdf;
    if (!FI.cached)
    {
        pdf.reset(document::load_from_file(pathName));
        if (!pdf)
        {
            cout << "Cannot open PDF file!" << nl;
            return FI;
        }

        numPages = pdf->pages();
    }
    else
    {
        cacheData = retrieveCache(pathName);
        numPages = cacheData.size();
    }

    // Iterate through all pages
    for (int i = 0; i < numPages; i++)
    {
        pageResult res;
        res.pageNumber = i + 1;
        string pageContent;
        vector<string> lines;

        if (!FI.cached)
        {
            unique_ptr<page> p(pdf->create_page(i));
            if (!p)
                continue;

            // Get text from the page as a string
            auto pageBytes = p->text();
            string pageText(pageBytes.begin(), pageBytes.end());

            // Formatting the text content into a good shape
            string line;

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
                    }

                    line.clear();
                }
                else
                    line.push_back(e);
            }
            // One final refining
            line = trim(line);
            if (line.size())
            {
                if (line.back() != '-')
                    line += " ";
                lines.push_back(line);
            }
            line.clear();

            dataToBeCached.push_back(lines);
        }
        else
            lines = cacheData[i];

        // Filling out the pageContent
        for (auto line : lines)
            pageContent += line;

        if (caseSensitivity == 2)
            pageContent = to_lower_str(pageContent);

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
            for (int key = 0; key < keywords.size(); key++)
            {
                vector<int> pos;
                if (isDeep)
                {
                    pair<vector<int>, vector<set<string>>> deepResult = findWordOrGetSug(pageContent, keywords[key], keywordCodes[key]);

                    pos = deepResult.first;

                    for (auto sg : deepResult.second[0])
                        res.suggestions_LV.insert(sg);

                    for (auto sg : deepResult.second[1])
                        res.suggestions_JW.insert(sg);

                    for (auto sg : deepResult.second[2])
                        res.suggestions_SD.insert(sg);
                }
                else
                {
                    pos = findWord(pageContent, keywords[key]);
                }
                for (auto position : pos)
                {
                    int lineNumber = lineNumberOfChars[position + 1];
                    int extraLine = lineNumberOfChars[position + keywords[key].size()];

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

        if (res.findings.size() || res.suggestions_LV.size() || res.suggestions_JW.size() || res.suggestions_SD.size())
            FI.results.push_back(res);
    }

    if (!FI.cached)
        insertCache(pathName, dataToBeCached);

    return FI;
}

void pdf_search_func(vector<string> roots, int searchDepth, int isWord, int keywordMode, int caseSensitivity, vector<string> keywords, bool isDeep)
{
    // fs::path root = "Files";
    vector<fs::path> pathRoots(roots.begin(), roots.end());

    for (auto root : pathRoots)
    {
        if (!fs::exists(root))
        {
            cout << "" << root << " this path doesnt exist..." << nl;
            return;
        }
    }

    vector<string> keywordCodes;
    if (isDeep == 1)
        for (auto key : keywords)
            keywordCodes.push_back(soundex(key));

    vector<FileInfo> results;

    for (auto root : pathRoots)
    {
        if (searchDepth == 1)
        {
            for (const auto &entry : fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied))
            {
                if (!isPDF(entry.path()))
                    continue;

                FileInfo FI = searchKEYWORD(entry.path(), keywords, isWord, caseSensitivity, isDeep, keywordCodes);

                if (FI.results.size())
                    results.push_back(FI);
            }
        }
        else
        {
            for (const auto &entry : fs::directory_iterator(root, fs::directory_options::skip_permission_denied))
            {
                if (!isPDF(entry.path()))
                    continue;

                FileInfo FI = searchKEYWORD(entry.path(), keywords, isWord, caseSensitivity, isDeep, keywordCodes);

                if (FI.results.size())
                    results.push_back(FI);
            }
        }
    }

    vector<pair<string, int>> serials;

    for (auto FI : results)
    {
        print_text_red(FI.fileName);
        if (FI.cached)
            print_text_red_fade(" (cached)");
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

            if (isDeep)
            {
                print_text_yellow("Suggestions : ");
                cout << nl;
                print_text_cyan_italic("    *Levenshtein - ");
                for (auto k = page.suggestions_LV.begin(); k != page.suggestions_LV.end(); k++)
                {
                    print_text_cyan_fade(*k);
                    print_text_cyan_fade(", ");
                }
                cout << nl;

                print_text_cyan_italic("    *Jaro-winkler - ");
                for (auto k = page.suggestions_JW.begin(); k != page.suggestions_JW.end(); k++)
                {
                    print_text_cyan_fade(*k);
                    print_text_cyan_fade(", ");
                }
                cout << nl;

                print_text_cyan_italic("    *Soundex - ");
                for (auto k = page.suggestions_SD.begin(); k != page.suggestions_SD.end(); k++)
                {
                    print_text_cyan_fade(*k);
                    print_text_cyan_fade(", ");
                }
                cout << nl;
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
                openPDF(serials[ind - 1].first, serials[ind - 1].second);
        }
    }
    else
        cout << "No results found" << nl;
}
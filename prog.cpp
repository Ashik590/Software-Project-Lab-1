#include <bits/stdc++.h>
#include <memory>
#include <poppler-document.h>
#include <poppler-page.h>
#include <X11/Xlib.h>
#include <unistd.h>

using namespace std;
using namespace poppler;

namespace fs = std::filesystem;

#define nl "\n"
#define ll long long

bool isPDF(fs::path filepath)
{
    // Check if is a regular file
    if (!fs::is_regular_file(filepath))
    {
        return false;
    }

    // Open file in binary mode
    ifstream file(filepath, ios::binary);
    if (!file)
        return false;

    char header[5];
    file.read(header, 5); // read first 5 bytes
    if (!file)
        return false;
    file.close();

    // Check PDF magic number
    return header[0] == '%' && header[1] == 'P' && header[2] == 'D' &&
           header[3] == 'F' && header[4] == '-';
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

void print_text_blue(string s)
{

    cout << "\033[1;34m" << s << "\033[0m";
}

void print_text_red(string s)
{

    cout << "\033[1;31m" << s << "\033[0m";
}

void print_text_yellow(string s)
{

    cout << "\033[0;33m" << s << "\033[0m";
}

string to_lower_str(string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        s[i] = tolower(s[i]);
    }

    return s;
}

struct SearchReturn
{
    bool found;
    vector<int> pageNums;
};

void openPDF(string path, int page)
{
    string command = "firefox \"" + path + "#page=" + to_string(page) + "\" &";
    system(command.c_str());
}

SearchReturn searchKEYWORD(fs::path pathName, string keyword, int &sl)
{
    SearchReturn sr;
    sr.found = false;

    keyword = to_lower_str(keyword);
    unique_ptr<document> pdf(document::load_from_file(pathName));

    if (!pdf)
    {
        cout << "Cannot open PDF file!" << nl;
        return sr;
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

        vector<string> lines;
        string line;
        for (auto e : pageText)
        {
            if (e == '\n')
            {
                line = trim(line);

                if (line.size())
                    lines.push_back(line);

                line.clear();
            }
            else
            {
                line.push_back(e);
            }
        }

        int oneTimePageNumPrinting = 1;

        for (int j = 0; j < lines.size(); j++)
        {
            string small_letter_line = to_lower_str(lines[j]);

            if (small_letter_line.find(keyword) != string::npos)
            {
                if (oneTimePageNumPrinting)
                {
                    cout << "---- ";
                    print_text_yellow("Page ");
                    print_text_yellow(to_string(i + 1));
                    cout << " ----" << nl << nl;

                    oneTimePageNumPrinting = 0;
                }

                print_text_blue(to_string(sl));
                cout << " - " << "( Line " << j + 1 << "/" << lines.size() << " )" << " " << lines[j] << nl << nl;
                sl++;

                sr.found = true;
                sr.pageNums.push_back(i + 1);
            }
        }
    }

    return sr;
}

int main()
{
    
    fs::path root = "Files";

    if (!fs::exists(root))
    {
        cout << "Path doesnt exist..." << nl;

        return 0;
    }

    string keyword;

    cout << "Enter keywords = ";
    getline(cin, keyword);
    cout << nl;

    int sl = 1; // serial number

    vector<pair<string, int>> results;

    for (const auto &entry : fs::recursive_directory_iterator(root))
    {
        print_text_red(entry.path());
        cout << nl << nl;

        if (!isPDF(entry.path()))
            continue;

        SearchReturn sr = searchKEYWORD(entry.path(), keyword, sl);
        if (sr.found)
        {
            for (auto pg : sr.pageNums)
            {
                pair<string, int> res = {entry.path(), pg};

                results.push_back(res);
            }
        }
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
                openPDF(results[ind - 1].first, results[ind - 1].second);
            }
        }
    }
    else
    {
        cout << "No results found" << nl;
    }

    return 0;
}
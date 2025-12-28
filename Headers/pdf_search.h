#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <map>
using namespace std;
#ifndef PDF_UTILITY_H
#define PDF_UTILITY_H
namespace fs = std::filesystem;

void pdf_search_func(vector<string> roots, int searchDepth, int isWord, int keywordMode, int caseSensitivity, vector<string> keywords, bool isDeep);

#endif
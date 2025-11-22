#include <bits/stdc++.h>
using namespace std;
#ifndef PDF_SEARCH_H
#define PDF_SEARCH_H
namespace fs = std::filesystem;

bool isPDF(fs::path filepath);
void openPDF(string path, int page);

#endif
#include <bits/stdc++.h>
using namespace std;
#ifndef CACHE_H
#define CACHE_H
namespace fs = std::filesystem;

bool has_cache_valid_copy(fs::path pathName);
vector<vector<string>> retrieveCache(fs::path pathName);
void insertCache(fs::path pathName, vector<vector<string>> dataToBeCached);
fs::path get_corresponding_cache_path(fs::path pathName);
void clearCache();
void updateCache();

#endif
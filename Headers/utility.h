#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <utility>
#include <set>
using namespace std;
namespace fs = std::filesystem;
#ifndef UTILITY_H
#define UTILITY_H

// Blues
void print_text_blue(string s);
void print_text_blue_bold(string s);
void print_text_blue_fade(string s);
void print_text_blue_italic(string s);
void print_text_blue_underline(string s);

// Reds
void print_text_red(string s);
void print_text_red_bold(string s);
void print_text_red_fade(string s);
void print_text_red_italic(string s);
void print_text_red_underline(string s);

// Yellows
void print_text_yellow(string s);
void print_text_yellow_bold(string s);
void print_text_yellow_fade(string s);
void print_text_yellow_italic(string s);
void print_text_yellow_underline(string s);

// Greens
void print_text_green(string s);
void print_text_green_bold(string s);
void print_text_green_fade(string s);
void print_text_green_italic(string s);
void print_text_green_underline(string s);

// Magentas
void print_text_magenta(string s);
void print_text_magenta_bold(string s);
void print_text_magenta_fade(string s);
void print_text_magenta_italic(string s);
void print_text_magenta_underline(string s);

// Cyans
void print_text_cyan(string s);
void print_text_cyan_bold(string s);
void print_text_cyan_fade(string s);
void print_text_cyan_italic(string s);
void print_text_cyan_underline(string s);

string trim(string s);
string to_lower_str(string s);
int menu_bar(string head, vector<string> options);
vector<int> findWord(string line, string keyword);
vector<int> getLPS(string &pattern);
vector<int> find_KMP(string &str, string &pattern);
pair<vector<int>, vector<set<string>>> findWordOrGetSug(string &str, string &keyword, string &keyCode);
time_t convert_to_seconds_since_epoch(fs::file_time_type lastModifiedTime);
long long string_to_long_long(string str);
void openImage(fs::path src);

#endif
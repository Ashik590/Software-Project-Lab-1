#include <bits/stdc++.h>
using namespace std;
#ifndef UTILITY_H
#define UTILITY_H

void print_text_blue(string s);
void print_text_red(string s);
void print_text_yellow(string s);
string trim(string s);
string to_lower_str(string s);
int menu_bar(string head, vector<string> options);
vector<int> findWord(string line, string keyword);
vector<int> getLPS(string &pattern);
vector<int> find_KMP(string &str, string &pattern);

#endif
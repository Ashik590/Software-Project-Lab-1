#include <bits/stdc++.h>
using namespace std;
#define nl "\n"
#define ll long long

int main()
{
    vector<string> v = {"hel", "helloo"};
    sort(v.begin(), v.end());

    for (auto e : v)
    {
        cout << e << ' ';
    }
    cout << nl;
    return 0;
}
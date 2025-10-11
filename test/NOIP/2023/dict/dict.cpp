// {P9868}
#include <bits/stdc++.h>

using namespace std;

int n, m, t;
vector<tuple<int, char, char>> arr;


int main() {
#ifndef ONLINE_JUDGE
    freopen("dict1.in", "r", stdin);
#endif
    cin >> n >> m;
    arr.reserve(n);
    for (string s; n; n--) {
        cin >> s;
        arr.emplace_back(
            ++t,
            *min_element(s.begin(), s.end()), 
            *max_element(s.begin(), s.end())
        );
    }
    for (const auto& [i, mi, _]: arr) {
        bool flag = true;
        for (const auto& [j, _, ma]: arr) {
            if (i != j && mi >= ma) {
                flag = false;
                break;
            }
        }
        cout << flag;
    }
    cout << endl;
}

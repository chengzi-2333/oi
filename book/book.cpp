#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<string> w;
string mi;

int main() {
#ifndef ONLINE_JUDGE
    freopen("book3.in", "r", stdin);
#endif
    cin >> n >> m; w.resize(n);
    for (auto& s: w) cin >> s;
    mi = *min_element(w.begin(), w.end());
    sort(mi.begin(), mi.end());
    reverse(mi.begin(), mi.end());
    for (auto s: w) {
        sort(s.begin(), s.end());
        cout << (s <= mi);
    }
    cout << endl;
}

#include <bits/stdc++.h>

using namespace std;

int main() {
#ifndef ONLINE_JUDGE
    freopen("book1.in", "r", stdin);
#endif
    int n, m;
    char c, b=CHAR_MAX;
    cin >> n >> m;
    vector<char> mi(n);
    for (int i=0; i<n; i++) {
        char ma=0;
        for (int j=0; j<m; j++) {
            cin >> c;
            mi[i] = min(mi[i], c);
            ma = max(ma, c);
        }
        b = min(b, ma);
    }
    for (const auto& c: mi) {
        cout << (c < b);
    }
    cout << endl;
}

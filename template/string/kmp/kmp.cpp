// {P3375}
#include <bits/stdc++.h>

using namespace std;

tuple<vector<int>, vector<int>> kmp(string s, string p) {
    vector<int> nxt(p.length()), mth;
    // next array
    nxt[0] = -1;
    for (int i=1, j=-1; i<(int) p.length(); i++) {
        while (j>=0 && p[i] != p[j+1]) j = nxt[j];
        if (p[i] == p[j+1]) j++;
        nxt[i] = j;
    }
    // search
    for (int i=0, j=-1; i<(int) s.length(); i++) {
        while (j>=0 && s[i] != p[j+1]) j = nxt[j];
        if (s[i] == p[j+1]) j++;
        if (j == (int) p.length()-1) {
            mth.push_back(i-p.length()+1);
            j = nxt[j];
        }
    }
    return {mth, nxt};
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("kmp.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    string s, p;
    cin >> s >> p;
    auto [mth, nxt] = kmp(s, p);
    for (const auto& m: mth) cout << m+1 << '\n';
    for (const auto& i: nxt) cout << i+1 << ' ';
    cout << endl;
}

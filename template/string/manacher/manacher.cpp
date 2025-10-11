// {P3805}
#include <bits/stdc++.h>

using namespace std;

vector<int> manacher(const string& s, int f) {
    vector<int> d(s.size());
    for (int i = 0, l = 0, r = -1; i < (int) d.size(); i++) {
        int k = (i > r) ? !f : min(d[l + r - i + f], r - i + 1);
        while (i - k - f >= 0 && i + k < (int) d.size() && s[i - k - f] == s[i + k]) k++;
        d[i] = k--;
        if (i + k > r) {
            l = i - k - f;
            r = i + k;
        }
    }
    return d;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("manacher.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    string s; cin >> s;
    vector<int> d;
    d = manacher(s, 0);
    int m1 = *max_element(d.begin(), d.end())*2-1;
    d = manacher(s, 1);
    int m2 = *max_element(d.begin(), d.end())*2;
    cout << max(m1, m2) << endl;
}

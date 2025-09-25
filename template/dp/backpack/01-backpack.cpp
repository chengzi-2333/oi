#include <bits/stdc++.h>

using namespace std;

int n, c;
vector<int> f;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("backpack.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> c >> n; f.resize(c+1);
    for (int v, w; n; n--) {
        cin >> v >> w;
        for (int j=c; j>=v; j--) {
            f[j] = max(f[j], f[j-v]+w);
        }
    }
    cout << f[c] << endl;
}

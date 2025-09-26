#include <bits/stdc++.h>

using namespace std;

constexpr int N = 1e5, P = 16+1;

int n, t;
array<int, N+5> lg;
array<array<int, P>, N+5> f;

inline void build() {
    for (int j=1; j<=lg[n]; j++) {
        for (int i=1; i<=n-(1<<(j-1))+1; i++) {
            f[i][j] = max(f[i][j-1], f[i+(1<<(j-1))][j-1]);
        }
    }
}

inline int query(int l, int r) {
    int k = lg[r-l+1];
    return max(f[l][k], f[r-(1<<k)+1][k]);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("sparse-table.in", "r", stdin);
#endif
    cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> t;
    for (int i=2; i<=n; i++) {
        lg[i] = lg[i>>1]+1;
    }
    for (int i=1; i<=n; i++) {
        cin >> f[i][0];
    }
    build();
    for (int l, r; t; t--) {
        cin >> l >> r;
        cout << query(l, r) << '\n';
    }
    cout << flush;
}

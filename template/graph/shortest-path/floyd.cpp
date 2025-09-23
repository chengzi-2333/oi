// floyd
#include <bits/stdc++.h>

using namespace std;

constexpr int N = 100;
constexpr long long inf = 0x3f3f3f3f3f3f3f3f;

int n, m;
long long f[N+2][N+2];


signed main() {
#ifndef ONLINE_JUDGE
    freopen("multi-source-1.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m;
    if (n > 100) return 1;
    memset(f, 0x3f, sizeof(f));
    for (int i=1; i<=n; i++) f[i][i] = 0;
    for (int u, v, w; m; m--) {
        cin >> u >> v >> w;
        f[u][v] = min(f[u][v], (long long) w);
    }
    for (int k=1; k<=n; k++) {
        for (int u=1; u<=n; u++) {
            for (int v=1; v<=n; v++) {
                if (f[u][k] != inf && f[k][v] != inf) {
                    f[u][v] = min(f[u][v], f[u][k]+f[k][v]);
                }
            }
        }
    }
    for (int i=1; i<=n; i++) {
        long long sum = 0;
        for (int j=1; j<=n; j++) {
            if (f[i][j] == inf) sum += (long long) j*1e9;
            else sum += j*f[i][j];
        }
        cout << sum << '\n';
    }
    cout << flush;
}

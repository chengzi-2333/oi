// dijkstra
#include <bits/stdc++.h>

using namespace std;

constexpr int N = 3e3;
constexpr long long inf = 0x3f3f3f3f3f3f3f3f;

using PII = pair<int, int>;

int n, m;
long long d[N+2][N+2];
vector<vector<PII>> g;

void dijkstra(int s) {
    priority_queue<PII, vector<PII>, greater<PII>> q;
    vector<bool> vis(n+1);
    d[s][s] = 0;
    q.push({0, s});
    while (!q.empty()) {
        int u = q.top().second;
        q.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto& [w, v]: g[u]) {
            if (d[s][v] > d[s][u] + w) {
                d[s][v] = d[s][u] + w;
                q.push({d[s][v], v});
            }
        }
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("multi-source-1.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m; g.resize(n+1);
    for (int u, v, w; m; m--) {
        cin >> u >> v >> w;
        if (w < 0) return 1;
        g[u].push_back({w, v});
    }
    memset(d, 0x3f, sizeof(d));
    for (int i=1; i<=n; i++) {
        dijkstra(i);
        long long sum = 0;
        for (int j=1; j<=n; j++) {
            if (d[i][j] == inf) sum += (long long) j*1e9;
            else sum += j*d[i][j];
        }
        cout << sum << '\n';
    }
    cout << flush;
}

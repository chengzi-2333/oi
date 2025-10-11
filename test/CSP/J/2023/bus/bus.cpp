// {P9751}
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

constexpr int INF = 0x3f3f3f3f;

using PII = pair<int, int>;

int n, m, k;
vector<vector<PII>> g;
vector<vector<int>> d;
vector<vector<bool>> vis;
std::priority_queue<PII, vector<PII>, std::greater<PII>> q;

void dijkstra(int s) {
    d[s][0] = 0;
    q.emplace(0, s);
    while (!q.empty()) {
        auto [p, u] = q.top();
        q.pop();
        if (vis[u][p%k]) continue;
        vis[u][p%k] = true;
        for (const auto& [w, v]: g[u]) {
            int t = 1 + p + (p < w)*((w-p+k-1)/k)*k;
            if (d[v][t%k] > t) {
                d[v][t%k] = t;
                q.emplace(t, v);
            }
        }
    }
}


signed main() {
#ifndef ONLINE_JUDGE
    freopen("bus1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m >> k;
    g.resize(n+1);
    vis.resize(n+1, vector<bool>(k+1));
    d.resize(n+1, vector<int>(k+1, INF));
    for (int u, v, w; m; m--) {
        cin >> u >> v >> w;
        g[u].emplace_back(w, v);
    }
    dijkstra(1);
    if (vis[n][0]) cout << d[n][0];
    else cout << -1;
    cout << endl;
}

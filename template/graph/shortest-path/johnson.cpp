// {P5905}
#include <bits/stdc++.h>

constexpr int N = 3e3;
constexpr long long inf = 0x3f3f3f3f3f3f3f3f;

using PII = std::pair<int, int>;

int n, m;
long long d[N + 2][N + 2];
std::vector<std::vector<PII>> g;

bool bellman_ford(int s) {
    bool flag = true;
    d[s][s] = 0;
    for (int i = 1; i <= n && flag; i++) {
        flag = false;
        for (int u = 0; u <= n; u++) {
            for (const auto& [w, v] : g[u]) {
                if (d[s][u] == inf) continue;
                if (d[s][v] > d[s][u] + w) {
                    d[s][v] = d[s][u] + w;
                    flag = true;
                }
            }
        }
    }
    return flag;
}

void dijkstra(int s) {
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> q;
    std::vector<bool> vis(n + 1);
    std::fill(d[s], d[s] + n + 1, inf);
    d[s][s] = 0;
    q.emplace(0, s);
    while (!q.empty()) {
        int u = q.top().second;
        q.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto& [w, v] : g[u]) {
            if (d[s][v] > d[s][u] + w) {
                d[s][v] = d[s][u] + w;
                q.emplace(d[s][v], v);
            }
        }
    }
}

void johnson() {
    for (int u = 1; u <= n; u++) {
        for (auto& [w, v] : g[u]) {
            w += d[0][u] - d[0][v];
        }
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("multi-source-1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize(n + 1);
    for (int p = 1; p <= n; p++) {
        g[0].emplace_back(0, p);
    }
    for (int u, v, w; m; m--) {
        std::cin >> u >> v >> w;
        g[u].emplace_back(w, v);
    }
    memset(d, 0x3f, sizeof(d));
    if (bellman_ford(0)) {
        std::cout << -1 << std::endl;
        return 0;
    }
    johnson();
    for (int i = 1; i <= n; i++) {
        dijkstra(i);
        long long sum = 0;
        for (int j = 1; j <= n; j++) {
            if (d[i][j] == inf) {
                sum += (long long)j * 1e9;
            } else {
                sum += j * (d[i][j] - d[0][i] + d[0][j]);
            }
        }
        std::cout << sum << '\n';
    }
}

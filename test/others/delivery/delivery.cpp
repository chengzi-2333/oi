#include <bits/stdc++.h>

using PII = std::pair<int, int>;
constexpr int INF = 0x3f3f3f3f;

int t, n, m, k, p;
std::vector<std::vector<PII>> g;


inline std::vector<PII> dijkstra() {
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> h;
    std::vector<bool> vis(n + 1);
    std::vector<PII> dis(n + 1, {INF, 0});
    h.emplace(0, 1);
    dis[1] = {0, 1};
    while (!h.empty()) {
        auto u = h.top().second;
        h.pop();
        if (vis[u]) continue;
        vis[u] = true;
        const auto& [du, fu] = dis[u];
        for (const auto& [w, v]: g[u]) {
            auto& [dv, fv] = dis[v];
            if (dv > du + w) {
                dv = du + w;
                h.emplace(dv, v);
                fv = fu;
            } else if (dv == du + w) {
                fv = (fv + fu) % p;
            }
        }
    }
    return dis;
}

inline int solve(const std::vector<PII>& dis) {
    if (k == 0) return dis[n].second % p;
    std::queue<int> que;
    std::vector<bool> vis(n + 1);
    std::vector<std::unordered_map<int, int>> f(n + 1);
    que.push(1);
    while (!que.empty()) {
        int u = que.front();
        que.pop();
        if (vis[u]) continue;
        vis[u] = true;
        const auto& [du, fu] = dis[u];
        for (const auto& [w, v]: g[u]) {
            // TODO: when k != 0
            const auto& [dv, fv] = dis[v];
            f[v][dv] = (f[v][dv] + f[u][du - w] + w) % p;
        }
    }
    return f[n][dis[n].first];  // TODO: zero circle
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("delivery1.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin >> t;
    while (t--) {
        std::cin >> n >> m >> k >> p;
        g.assign(n + 1, {});
        for (int u, v, w; m; m--) {
            std::cin >> u >> v >> w;
            g[u].emplace_back(w, v);
        }
        std::cout << solve(dijkstra()) << '\n';
    }
}

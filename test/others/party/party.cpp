#include <bits/stdc++.h>

#define int long long

using PII = std::pair<int, int>;

constexpr int INF = 0x3f3f3f3f;
int n, m, k;
std::vector<std::vector<PII>> g;
std::vector<int> shops, dis1;

// O((n + m) * log(n))
std::vector<int> dijkstra(int s) {
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> h;
    std::vector<int> dis(n + 1, INF);
    std::vector<bool> vis(n + 1);
    dis[s] = 0;
    h.emplace(dis[s], s);
    while (!h.empty()) {
        auto u = h.top().second;
        h.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto& [v, w] : g[u]) {
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                h.emplace(dis[v], v);
            }
        }
    }
    return dis;
}

// O(n * ((n + m) * log(n) + k)) == O(n * m * log(n))
bool naive(int s) {
    if (shops[s]) return true;
    const auto dis2 = dijkstra(s);
    for (int k = 1; k <= n; k++) {
        if (shops[k] == 0) continue;
        if (dis2[k] + dis1[k] <= shops[k] + dis1[s]) return true;
    }
    return false;
}

// O(/* TODO: better time complexity */)
void solve() {
    // TODO
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("party1.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> k;
    bool use_naive = n * m * std::__lg(n) < 1e8;
    g.resize(n + 1);
    for (int u, v, w; m; m--) {
        std::cin >> u >> v >> w;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    shops.resize(n + 1);
    for (int u, v; k; k--) {
        std::cin >> u >> v;
        shops[u] = std::max(shops[u], v);
    }
    dis1 = dijkstra(n);
    if (use_naive) {
        for (int s = 1; s < n; s++) {
            std::cout << naive(s) << '\n';
        }
    } else solve();
}

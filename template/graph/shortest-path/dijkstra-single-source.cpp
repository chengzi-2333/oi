// {P4779} {P3371}
#include <bits/stdc++.h>

constexpr int inf = 0x3f3f3f3f;
using PII = std::pair<int, int>;

int n, m, s;
std::vector<std::vector<PII>> g;
std::priority_queue<PII, std::vector<PII>, std::greater<PII>> q;
std::vector<int> d;
std::vector<bool> vis;

void dijkstra() {
    std::fill(d.begin() + 1, d.end(), inf);
    d[s] = 0;
    q.emplace(0, s);
    while (!q.empty()) {
        int u = q.top().second;
        q.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto& [w, v] : g[u]) {
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
                q.emplace(d[v], v);
            }
        }
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("single-source.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> s;
    g.resize(n + 1);
    d.resize(n + 1);
    vis.resize(n + 1);
    for (int u, v, w; m; m--) {
        std::cin >> u >> v >> w;
        g[u].emplace_back(w, v);
    }
    dijkstra();
    for (auto it = d.begin() + 1; it != d.end(); it++) {
        if (*it == inf)
            std::cout << INT_MAX;
        else
            std::cout << *it;
        std::cout << ' ';
    }
    std::cout << std::endl;
}

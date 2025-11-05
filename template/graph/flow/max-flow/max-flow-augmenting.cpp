// {P3376}
#include <bits/stdc++.h>

using PII = std::pair<int, int>;

int n, m, s, t;
std::vector<std::unordered_map<int, int>> g;
std::vector<bool> vis;

int capacity() {
    return std::accumulate(g[s].begin(), g[s].end(), 0,
                           [](int a, const PII& b) { return a + b.second; });
}

std::pair<bool, int> naive(int u, int neck) {
    if (u == t) return {true, neck};
    vis[u] = true;
    bool flag = false;
    for (auto& [v, w] : g[u]) {
        if (w == 0 || vis[v]) continue;
        std::tie(flag, neck) = naive(v, std::min(neck, w));
        if (flag) {
            w -= neck;
            break;
        }
    }
    vis[u] = false;
    return {flag, neck};
}

std::pair<bool, int> ford_fulkerson(int u, int neck) {
    if (u == t) return {true, neck};
    vis[u] = true;
    bool flag = false;
    for (auto& [v, w] : g[u]) {
        if (w == 0 || vis[v]) continue;
        std::tie(flag, neck) = ford_fulkerson(v, std::min(neck, w));
        if (flag) {
            g[v][u] += neck;  // the only difference
            w -= neck;
            break;
        }
    }
    vis[u] = false;
    return {flag, neck};
}

int naive() {
    vis.resize(n + 1);
    auto prev = capacity();
    while (naive(s, INT_MAX).first);
    return prev - capacity();
}

int ford_fulkerson() {
    vis.resize(n + 1);
    auto prev = capacity();
    while (ford_fulkerson(s, INT_MAX).first);
    return prev - capacity();
}

int edmonds_karp() {
    // TODO
}

int dinic() {
    // TODO
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("max-flow.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> s >> t;
    g.resize(n + 1);
    for (int u, v; m; m--) std::cin >> u >> v >> g[u][v];
    std::cout << edmonds_karp() << std::endl;
}

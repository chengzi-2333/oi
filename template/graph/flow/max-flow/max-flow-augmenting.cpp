// {P3376}
#include <bits/stdc++.h>

#define int long long
#define MAX LONG_LONG_MAX

using PII = std::pair<int, int>;

int n, m, s, t;
std::vector<std::unordered_map<int, int>> g;

int capacity() {
    return std::accumulate(g[s].begin(), g[s].end(), static_cast<int>(0),
                           [](int a, const PII& b) { return a + b.second; });
}

namespace FordFulkerson {

std::vector<bool> vis;

std::pair<bool, int> ford_fulkerson(int u, int neck) {
    if (u == t) return {true, neck};
    vis[u] = true;
    bool flag = false;
    for (auto& [v, w] : g[u]) {
        if (w == 0 || vis[v]) continue;
        std::tie(flag, neck) = ford_fulkerson(v, std::min(neck, w));
        if (flag) {
            g[v][u] += neck;
            w -= neck;
            break;
        }
    }
    vis[u] = false;
    return {flag, neck};
}

int ford_fulkerson() {
    vis.resize(n + 1);
    auto prev = capacity();
    while (ford_fulkerson(s, MAX).first);
    return prev - capacity();
}

}  // namespace FordFulkerson

namespace EdmondsKarp {

std::vector<int> shortest_path() {
    std::vector<bool> vis(g.size());
    std::vector<int> fa(g.size());
    std::queue<int> que;
    que.push(s);
    while (!que.empty()) {
        auto u = que.front();
        que.pop();
        if (u == t) break;
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto& [v, w] : g[u]) {
            if (w == 0 || vis[v]) continue;
            fa[v] = u;
            que.push(v);
        }
    }
    return fa;
}

int edmonds_karp() {
    auto prev = capacity();
    for (auto fa = shortest_path(); fa[t]; fa = shortest_path()) {
        int neck = MAX;
        for (int u = fa[t], v = t; u; v = u, u = fa[u]) {
            neck = std::min(neck, g[u][v]);
        }
        for (int u = fa[t], v = t; u; v = u, u = fa[u]) {
            g[v][u] += neck;
            g[u][v] -= neck;
        }
    }
    return prev - capacity();
}

}  // namespace EdmondsKarp

namespace Dinic {

std::vector<int> dep;

bool bfs() {
    dep.assign(g.size(), 0);
    std::queue<int> que;
    dep[s] = 1;
    que.push(s);
    while (!que.empty()) {
        auto u = que.front();
        que.pop();
        for (const auto& [v, w] : g[u]) {
            if (dep[v] || w == 0) continue;
            dep[v] = dep[u] + 1;
            que.push(v);
        }
    }
    return dep[t] != 0;
}

int dfs(int u, int resi) {
    if (u == t || resi == 0) return resi;
    for (auto& [v, w] : g[u]) {
        if (dep[v] == dep[u] + 1 && (resi = dfs(v, std::min(resi, w)))) {
            g[v][u] += resi;
            w -= resi;
            if (resi == 0) return resi;
            // TODO
        }
    }
    return resi;
}

int dinic() {
    auto prev = capacity();
    while (bfs()) dfs(s, MAX);
    return prev - capacity();
}

}  // namespace Dinic

signed main() {
#ifndef ONLINE_JUDGE
    freopen("max-flow.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> s >> t;
    g.resize(n + 1);
    for (int u, v, w; m; m--) {
        std::cin >> u >> v >> w;
        g[u][v] += w;
    }
    std::cout << Dinic::dinic() << std::endl;
}

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

namespace Naive {

std::vector<bool> vis;

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

int naive() {
    vis.resize(n + 1);
    auto prev = capacity();
    while (naive(s, MAX).first);
    return prev - capacity();
}

}  // namespace Naive

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
            g[v][u] += neck;  // the only difference
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
    std::vector<bool> vis(n + 1);
    std::vector<int> fa(n + 1);
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

std::vector<int> lv;

bool bfs() {
    // TODO
}

int dfs(int u) {
    // TODO
}

int dinic() {
    lv.resize(n + 1);
    int ans = 0;
    while (bfs()) ans += dfs(s);
    return ans;
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

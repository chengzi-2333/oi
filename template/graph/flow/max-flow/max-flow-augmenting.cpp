// {P3376}
#include <bits/stdc++.h>

#define int long long
#define INF LONG_LONG_MAX

int n, m, s, t;
std::vector<std::unordered_map<int, int>> g;

namespace EdmondsKarp {

int augment() {
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
    int neck = INF;
    for (int u = fa[t], v = t; u; v = u, u = fa[u]) {
        neck = std::min(neck, g[u][v]);
    }
    if (neck == INF) return 0;
    for (int u = fa[t], v = t; u; v = u, u = fa[u]) {
        g[v][u] += neck;
        g[u][v] -= neck;
    }
    return neck;
}

int edmonds_karp() {
    int tmp = 0, flow = 0;
    do {
        tmp = augment();
        flow += tmp;
    } while (tmp);
    return flow;
}

}  // namespace EdmondsKarp

namespace Dinic {

std::vector<int> dep;

bool make_level() {
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

int augment(int u, int fneck = INF) {
    if (u == t || fneck == 0) return fneck;
    for (const auto& [v, w] : g[u]) {
        // TODO: current arc
        if (dep[v] == dep[u] + 1 && w) {
            auto neck = augment(v, std::min(fneck, w));
            if (neck) {
                g[v][u] += neck;
                g[u][v] -= neck;
                return neck;
            } else {
                dep[v] = 0;
            }
        }
    }
    return 0;
}

int dinic() {
    int tmp = 0, flow = 0;
    while (make_level()) {
        do {
            tmp = augment(s);
            flow += tmp;
        } while (tmp);
    }
    return flow;
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

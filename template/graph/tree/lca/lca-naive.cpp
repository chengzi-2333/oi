// {P3379}
// naive
#include <bits/stdc++.h>

int n, m, s;
std::vector<std::unordered_set<int>> g;
std::vector<int> fa, dep;

void dfs(int u, int f) {
    fa[u] = f;
    dep[u] = dep[f] + 1;
    for (const auto& v : g[u])
        if (v != f) dfs(v, u);
}

int lca(int a, int b) {
    if (dep[a] < dep[b]) std::swap(a, b);
    while (dep[a] != dep[b]) a = fa[a];
    if (a == b) return a;
    while (fa[a] != fa[b]) a = fa[a], b = fa[b];
    return fa[a];
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("lca.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> s;
    g.resize(n + 1);
    fa.resize(n + 1), dep.resize(n + 1);
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> u >> v;
        if (u == v) continue;
        g[u].insert(v);
        g[v].insert(u);
    }
    dfs(s, 0);
    for (int a, b; m; m--) {
        std::cin >> a >> b;
        std::cout << lca(a, b) << '\n';
    }
}

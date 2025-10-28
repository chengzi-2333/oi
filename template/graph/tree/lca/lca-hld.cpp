// {P3379}
#include <bits/stdc++.h>

int n, m, s;
std::vector<std::vector<int>> g;
std::vector<int> fa, sz, dep, hson, top;

void build_tree(int u, int f) {
    fa[u] = f;
    dep[u] = dep[f] + 1;
    sz[u] = 1;
    for (const auto& v : g[u]) {
        if (v == f) continue;
        build_tree(v, u);
        sz[u] += sz[v];
        if (sz[v] > sz[hson[u]]) hson[u] = v;
    }
}

void cut_tree(int u, int ftop) {
    top[u] = ftop;
    if (hson[u]) cut_tree(hson[u], ftop);
    for (const auto& v: g[u]) {
        if (v != hson[u] && v != fa[u]) cut_tree(v, v);
    }
}

int lca(int u, int v) {
    while (top[u] != top[v]) {
        if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
        u = fa[top[u]];
    }
    return (dep[u] < dep[v] ? u : v);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("lca.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> s;
    g.resize(n + 1);
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }

    fa.resize(n + 1), sz.resize(n + 1);
    dep.resize(n + 1), hson.resize(n + 1);
    build_tree(s, 0);

    top.resize(n + 1);
    cut_tree(s, s);

    for (int u, v; m; m--) {
        std::cin >> u >> v;
        std::cout << lca(u, v) << '\n';
    }
}

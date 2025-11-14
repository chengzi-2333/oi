// {P3128}
#include <bits/stdc++.h>

int n, m, t;
std::vector<std::vector<int>> g;
std::vector<int> dep, sz, fa, hson, top, dfn, rnk, diff;

void build_tree(int u, int f) {
    dep[u] = dep[f] + 1;
    fa[u] = f;
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
    dfn[u] = ++t;
    rnk[dfn[u]] = u;
    if (hson[u]) cut_tree(hson[u], ftop);
    for (const auto& v : g[u]) {
        if (v != hson[u] && v != fa[u]) cut_tree(v, v);
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("max-flow-p.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize(n + 1);
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    dep.resize(n + 1), sz.resize(n + 1);
    fa.resize(n + 1), hson.resize(n + 1);
    build_tree(1, 0);
    top.resize(n + 1), dfn.resize(n + 1), rnk.resize(n + 1);
    cut_tree(1, 1);

    diff.resize(n + 1);
    for (int u, v; m; m--) {
        std::cin >> u >> v;
        while (top[u] != top[v]) {
            if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
            diff[dfn[top[u]]] += 1, diff[dfn[u] + 1] -= 1;
            u = fa[top[u]];
        }
        if (dep[u] > dep[v]) std::swap(u, v);
        diff[dfn[u]] += 1, diff[dfn[v] + 1] -= 1;
    }
    std::partial_sum(diff.begin(), diff.end(), diff.begin());
    std::cout << *std::max_element(diff.begin(), diff.end()) << std::endl;
}

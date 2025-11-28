// {P3379}
// binary lifting
#include <bits/stdc++.h>

constexpr int P = 21;

int n, m, s;
std::vector<std::unordered_set<int>> g;
std::vector<std::array<int, P>> fa;
std::vector<int> dep;

void dfs(int u, int f) {
    fa[u][0] = f;
    dep[u] = dep[f] + 1;
    for (int i = 1; i < P; i++) fa[u][i] = fa[fa[u][i - 1]][i - 1];
    for (const auto& v : g[u])
        if (v != f) dfs(v, u);
}

int lca(int a, int b) {
    if (dep[a] < dep[b]) std::swap(a, b);
    int d = abs(dep[a] - dep[b]);
    for (int i = 0; d; i++, d >>= 1)
        if (d & 1) a = fa[a][i];
    if (a == b) return a;
    for (int i = P - 1; i >= 0 && a != b; i--)
        if (fa[a][i] != fa[b][i]) a = fa[a][i], b = fa[b][i];
    return fa[a][0];
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

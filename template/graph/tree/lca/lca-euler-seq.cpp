// {P3379}
// Euler sequence + RMQ (ST implemented)
#include <bits/stdc++.h>

constexpr int P = 21;

int n, m, s, t;
std::vector<std::unordered_set<int>> g;
std::vector<std::array<int, P>> st;
std::vector<int> dep;

void dfs(int u, int f) {
    if (!st[u][0]) st[u][0] = ++t;
    dep[u] = dep[f] + 1;
    for (const auto& v : g[u])
        if (v != f) dfs(v, u);
}

void init_st() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= std::__lg(n); j++) {
            // st[i][j] = min(st[i][j-1], st[][]);
            // TODO
        }
    }
}

int lca(int a, int b) {
    int l = std::min(st[a][0], st[b][0]), r = std::max(st[a][0], st[b][0]);
    int k = std::__lg(r - l + 1);
    return 0;  // TODO
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("lca.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> s;
    g.resize(n + 1);
    st.resize(n + 1), dep.resize(n + 1);
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> u >> v;
        if (u == v) continue;
        g[u].insert(v);
        g[v].insert(u);
    }
    dfs(s, 0);
    init_st();
    for (int a, b; m; m--) {
        std::cin >> a >> b;
        std::cout << lca(a, b) << '\n';
    }
}

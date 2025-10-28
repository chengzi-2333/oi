#include <bits/stdc++.h>

int n, m;
std::vector<std::vector<int>> g;
std::vector<int> fa, sz, dep, hson, top;
std::vector<std::pair<int, int>> task;
std::vector<int> detect, cnt;

std::vector<std::vector<std::pair<int, int>>> op;
std::unordered_map<int, int> sum;

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
    for (const auto& v : g[u]) {
        if (v != hson[u] && v != fa[u]) cut_tree(v, v);
    }
}

inline int lca(int u, int v) {
    while (top[u] != top[v]) {
        if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
        u = fa[top[u]];
    }
    return (dep[u] < dep[v] ? u : v);
}

inline int dis(int u, int v) { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }

void dfs(int u, int f, int flag) {
    auto p = detect[u] + flag * dep[u];
    auto t = sum[p];
    for (const auto& [a, b]: op[u]) sum[a] += b;
    for (const auto& v: g[u]) {
        if (v != f) dfs(v, u, flag);
    }
    cnt[u] += sum[p] - t;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("signal1.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;

    g.resize(n + 1);
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }

    detect.resize(n + 1), cnt.resize(n + 1);
    for (int i = 1; i <= n; i++) std::cin >> detect[i];

    task.resize(m);
    for (auto& [u, v]: task) std::cin >> u >> v;

    fa.resize(n + 1), sz.resize(n + 1), dep.resize(n + 1);
    hson.resize(n + 1), top.resize(n + 1);
    build_tree(1, 0), cut_tree(1, 1);

    op.assign(n + 1, {});
    for (const auto& [u, v]: task) {
        op[u].emplace_back(dep[u], 1);
        op[fa[lca(u, v)]].emplace_back(dep[u], -1);
    }
    dfs(1, 0, 1);
    sum.clear();
    op.assign(n + 1, {});
    for (const auto& [u, v]: task) {
        auto lc = lca(u, v);
        op[v].emplace_back(dep[u] - 2 * dep[lc], 1);
        op[lc].emplace_back(dep[u] - 2 * dep[lc], -1);
    }
    dfs(1, 0, -1);

    for (int i = 1; i <= n; i++) std::cout << cnt[i] << ' ';
    std::cout << std::endl;
}

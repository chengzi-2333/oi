#include <bits/stdc++.h>

#define int long long

constexpr int INF = 0x3f3f3f3f3f3f3f3f;

int t;
int n, m, tm;
std::vector<std::vector<int>> g;
std::vector<int> fa, dep, sz, hson, top, dfn, rnk;
std::vector<std::vector<int>> tb;

void build_tree(int u) {
    dep[u] = dep[fa[u]] + 1;
    sz[u] = 1;
    for (const auto& v : g[u]) {
        if (v == fa[u]) continue;
        build_tree(v);
        sz[u] += sz[v];
        if (sz[v] > sz[hson[u]]) hson[u] = v;
    }
}

void cut_tree(int u, int ftop) {
    top[u] = ftop;
    dfn[u] = ++tm;
    rnk[dfn[u]] = u;
    if (u == hson[fa[u]]) cut_tree(hson[u], ftop);
    for (const auto& v : g[u]) {
        if (v != hson[u] && v != fa[u]) cut_tree(v, v);
    }
}

void assign_tree(int u, int d) {
    tb.front()[dfn[u]] = m - d;
    for (const auto& v : g[u]) {
        if (v != fa[u]) assign_tree(v, d + 1);
    }
}

inline void make_st() {
    for (int i = 1; i <= std::__lg(n); i++) {
        for (int j = 1; j <= n - (1 << (i - 1)) + 1; j++) {
            tb[i][j] = std::min(tb[i - 1][j], tb[i - 1][j + (1 << (i - 1))]);
        }
    }
}

inline int query_st(int l, int r) {
    if (l > r) return 0;
    int k = std::__lg(r - l + 1);
    return std::min(tb[k][l], tb[k][r - (1 << k) + 1]);
}

int get_sum() {
    int sum = 0;
    for (int u = 1; u <= n; u++) {
        sum += query_st(1, dfn[u] - 1) + query_st(dfn[u] + sz[u], n);
    }
    return sum;
}

signed main() {
    freopen("tree.in", "r", stdin);
    freopen("tree.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> t;
    while (t--) {
        std::cin >> n >> m;
        m++;
        g.assign(n + 1, {});
        fa.assign(n + 1, {});
        for (int u, v = 2; v <= n; v++) {
            std::cin >> u;
            fa[v] = u;
            g[u].push_back(v), g[v].push_back(u);
        }

        tm = 0;
        dep.assign(n + 1, {}), sz.assign(n + 1, {}), hson.assign(n + 1, {});
        top.assign(n + 1, {}), dfn.assign(n + 1, {}), rnk.assign(n + 1, {});
        build_tree(1);
        cut_tree(1, 1);

        tb.assign(std::__lg(n) + 1, std::vector<int>(n + 2, INF));
        tb.front().assign(n + 2, 0);
        assign_tree(1, 0);
        make_st();
        std::cout << get_sum() << '\n';
    }
}

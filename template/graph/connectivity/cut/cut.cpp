// {P3388}
#include <bits/stdc++.h>

constexpr char lf = '\n';

int n, m;
std::vector<std::vector<int>> g;

int t;
std::vector<int> dfn, low, pt;

void tarjan(int u, int f) {
    dfn[u] = low[u] = ++t;
    int child = 0;
    for (const auto& v : g[u]) {
        if (!dfn[v]) {
            child++;
            tarjan(v, u);
            low[u] = std::min(low[u], low[v]);
            if (f != u && low[v] >= dfn[u]) pt.push_back(u);
        } else if (v != f) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }
    if (f == u && child >= 2) pt.push_back(u);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("cut.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize(n + 1);
    for (int u, v; m; m--) {
        std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfn.resize(n + 1), low.resize(n + 1);
    for (int u = 1; u <= n; u++) {
        if (!dfn[u]) tarjan(u, u);
    }
    std::cout << pt.size() << lf;
    std::sort(pt.begin(), pt.end());
    std::copy(pt.begin(), pt.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

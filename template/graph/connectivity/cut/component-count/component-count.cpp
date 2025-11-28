#include <bits/stdc++.h>

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m, ma, cnt, rt;
std::vector<std::vector<int>> g;

int t;
std::vector<int> dfn, low;

void tarjan(int u) {
    dfn[u] = low[u] = ++t;
    int child = 0;
    for (const auto& v : g[u]) {
        if (!dfn[v]) {
            tarjan(v);
            low[u] = std::min(low[u], low[v]);
            if (low[v] >= dfn[u]) child++;
        } else
            low[u] = std::min(low[u], dfn[v]);
    }
    if (u != rt) child++;
    ma = std::max(ma, child);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("component-count1.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    while (n || m) {
        ma = cnt = t = 0;
        g.assign(n + 1, std::vector<int>());
        for (int u, v; m; m--) {
            std::cin >> u >> v;
            u++, v++;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        dfn.assign(n + 1, 0);
        low.assign(n + 1, 0);
        for (rt = 1; rt <= n; rt++) {
            if (!dfn[rt]) cnt++, tarjan(rt);
        }
        std::cout << cnt - 1 + ma << lf;
        std::cin >> n >> m;
    }
}

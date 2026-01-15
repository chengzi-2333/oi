#include <bits/stdc++.h>

int n, m, t;
std::vector<std::vector<int>> g;
std::vector<int> dfn, low, blk;

void tarjan(int u) {
    dfn[u] = low[u] = ++t;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("2-SAT.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize(2 * n + 2);
    for (int u, v, a, b; m; m--) {
        std::cin >> u >> a >> v >> b;
        // TODO
        // g[a ? u : u].push_back(b ? v : v);
        // g[b ? v : v].push_back(a ? u : u);
    }
    dfn.resize(g.size()), low.resize(g.size());
}

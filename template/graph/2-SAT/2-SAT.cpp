// {P4782}
#include <bits/stdc++.h>

int n, m, t, cnt;
std::vector<std::vector<int>> g;
std::vector<int> dfn, low, blk;
std::vector<bool> vis;
std::stack<int, std::vector<int>> st;

void tarjan(int u) {
    dfn[u] = low[u] = ++t;
    st.push(u);
    vis[u] = true;
    for (const auto& v : g[u]) {
        if (!dfn[v]) {
            tarjan(v);
            low[u] = std::min(low[u], low[v]);
        } else if (vis[v]) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }
    if (dfn[u] == low[u]) {
        auto tp = st.top();
        cnt++;
        do {
            tp = st.top();
            st.pop();
            vis[tp] = false;
            blk[tp] = cnt;
        } while (tp != u);
    }
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
        g[a ? u + n : u].push_back(b ? v : v + n);
        g[b ? v + n : v].push_back(a ? u : u + n);
    }
    dfn.resize(g.size()), low.resize(g.size());
    vis.resize(g.size()), blk.resize(g.size());
    for (int u = 1; u < (int) g.size(); u++) {
        if (!dfn[u]) tarjan(u);
    }
    for (int u = 1; u <= n; u++) {
        if (blk[u] == blk[u + n]) {
            std::cout << "IMPOSSIBLE" << std::endl;
            return 0;
        }
    }
    std::cout << "POSSIBLE" << std::endl;
    for (int u = 1; u <= n; u++) {
        std::cout << (blk[u] < blk[u + n]) << " ";
    }
    std::cout << std::endl;
}

// {B4016}
#include <bits/stdc++.h>

int n, d;
std::vector<std::vector<int>> g;
std::vector<int> dp;

void dfs(int u, int f) {
    for (const auto& v : g[u]) {
        if (v == f) continue;
        dfs(v, u);
        d = std::max(d, dp[u] + dp[v] + 1);
        dp[u] = std::max(dp[u], dp[v] + 1);
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("diameter.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    g.resize(n + 1);
    dp.resize(n + 1);
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    dfs(1, 0);
    std::cout << d << std::endl;
}

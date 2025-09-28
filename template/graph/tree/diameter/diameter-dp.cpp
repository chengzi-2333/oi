#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, d;
vector<vector<int>> g;
vector<int> dp;

void dfs(int u, int f) {
    for (const auto& v: g[u]) {
        if (v == f) continue;
        dfs(v, u);
        d = max(d, dp[u]+dp[v]+1);
        dp[u] = max(dp[u], dp[v] + 1);
    }
}


signed main() {
#ifndef ONLINE_JUDGE
    freopen("diameter.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n; g.resize(n+1); dp.resize(n+1);
    for (int u, v, i=1; i<n; i++) {
        cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    dfs(1, 0);
    cout << d << endl;
}

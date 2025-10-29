// {P1352}
#include <bits/stdc++.h>

int n, rt;
std::vector<std::vector<int>> g;
std::vector<int> fa, f[2];

void dp(int u) {
    for (const auto& v: g[u]) {
        dp(v);
        f[0][u] += std::max(f[0][v], f[1][v]);
        f[1][u] += f[0][v];
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("ball.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    g.resize(n + 1), fa.resize(n + 1);
    f[0].resize(n + 1), f[1].resize(n + 1);
    for (int i = 1; i <= n; i++) std::cin >> f[1][i];
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> v >> u;
        g[u].push_back(v);
        fa[v] = u;
    }
    rt = std::distance(fa.begin(), std::find(fa.begin() + 1, fa.end(), 0));
    dp(rt);
    std::cout << std::max(f[0][rt], f[1][rt]) << std::endl;
}

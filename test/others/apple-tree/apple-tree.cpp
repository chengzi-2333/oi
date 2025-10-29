// {P2015}
#include <bits/stdc++.h>

int n, q;
std::vector<std::vector<std::pair<int, int>>> g;
std::vector<std::vector<int>> f;

int dp(int u, int fa) {
    int szu = 1, szv = 0;
    for (const auto& [w, v] : g[u]) {
        if (v == fa) continue;
        szv = dp(v, u);
        szu += szv;
        for (int i = std::min(q, szu); i; i--) {
            for (int j = std::min(szv, i - 1); j >= 0; j--) {
                f[u][i] = std::max(f[u][i], f[u][i - j - 1] + f[v][j] + w);
            }
        }
    }
    return szu;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("apple-tree.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> q;
    g.resize(n + 1);
    for (int u, v, w, i = 1; i < n; i++) {
        std::cin >> u >> v >> w;
        g[u].emplace_back(w, v);
        g[v].emplace_back(w, u);
    }
    f.resize(n + 1, std::vector<int>(q + 1));
    dp(1, 0);
    std::cout << f[1][q] << std::endl;
}

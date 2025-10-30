// {P3478}
#include <bits/stdc++.h>

int n;
std::vector<long long> sz, dep, f;
std::vector<std::vector<int>> g;

void dfs(int u, int fa) {
    dep[u] = dep[fa] + 1;
    sz[u] = 1;
    for (const auto& v : g[u]) {
        if (v == fa) continue;
        dfs(v, u);
        sz[u] += sz[v];
    }
}

void dp(int u, int fa) {
    for (const auto& v : g[u]) {
        if (v == fa) continue;
        f[v] = f[u] - 2 * sz[v] + sz[1];
        dp(v, u);
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("sta-station.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    g.resize(n + 1);
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    sz.resize(n + 1), dep.resize(n + 1), f.resize(n + 1);
    dfs(1, 0);
    f[1] = std::accumulate(dep.begin(), dep.end(), 0);
    dp(1, 0);
    std::cout << std::distance(f.begin(), std::max_element(f.begin(), f.end())) << std::endl;
}

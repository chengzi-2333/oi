#include <bits/stdc++.h>

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m;
std::vector<std::vector<int>> g, pg;

void dfs(int u, int f) {
    for (const auto& v: pg[u]) {
        if (v == f) continue;
        g[u].push_back(v);
        dfs(v, u);
    }
}

// TODO
// brute force
int search(int u) {
    if (g[u].size() == 0) return 1;
    int mi = 0x3f3f3f3f;
    for (const auto& i: g[u]) {
        int ans = 1;
        for (const auto& v: g[u]) {
            if (v == i) continue;
            ans += search(v);
        }
        mi = std::min(mi, ans);
    }
    return mi;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("infection.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize(n+1); pg.resize(n+1);
    for (int u, v; m; m--) {
        std::cin >> u >> v;
        pg[u].push_back(v);
        pg[v].push_back(u);
    }
    dfs(1, 0); pg.clear();
    std::cout << search(1) << std::endl;
}

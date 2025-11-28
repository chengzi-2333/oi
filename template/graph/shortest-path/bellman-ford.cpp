// {P5905}
#include <bits/stdc++.h>

constexpr int N = 3e3;
constexpr long long inf = 0x3f3f3f3f3f3f3f3f;

int n, m;
long long d[N + 2][N + 2];
std::vector<std::tuple<int, int, int>> pos;

bool bellman_ford(int s) {
    bool flag = true;
    d[s][s] = 0;
    for (int i = 1; i <= n && flag; i++) {
        flag = false;
        for (const auto& [u, v, w] : pos) {
            if (d[s][u] == inf) continue;
            if (d[s][v] > d[s][u] + w) {
                d[s][v] = d[s][u] + w;
                flag = true;
            }
        }
    }
    return flag;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("multi-source-2.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    for (int p = 1; p <= n; p++) pos.emplace_back(0, p, 0);
    for (int u, v, w; m; m--) {
        std::cin >> u >> v >> w;
        pos.emplace_back(u, v, w);
    }
    memset(d, 0x3f, sizeof(d));
    for (int p = 0; p <= n; p++) {
        if (bellman_ford(p)) {
            std::cout << -1 << std::endl;
            return 0;
        }
    }
    for (int i = 1; i <= n; i++) {
        long long sum = 0;
        for (int j = 1; j <= n; j++) {
            if (d[i][j] == inf)
                sum += (long long)j * 1e9;
            else
                sum += j * d[i][j];
        }
        std::cout << sum << '\n';
    }
}

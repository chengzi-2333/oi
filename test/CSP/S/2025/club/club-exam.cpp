#include <bits/stdc++.h>

#define int long long

bool flagA = true;
int t, n;
std::vector<int> w[4];

inline int solve1() {
    std::priority_queue<std::tuple<int, int, int>> h;
    std::vector<bool> vis(n);
    for (int i = 0; i < n; i++) {
        for (int j = 1; j <= 3; j++) h.emplace(w[j][i], j, i);
    }
    int x[4] = {0, 0, 0, 0}, ans = 0;
    while (!h.empty()) {
        auto w = std::get<0>(h.top()), v = std::get<1>(h.top()), i = std::get<2>(h.top());
        h.pop();
        if (vis[i] || x[v] >= n/2) continue;
        ans += w;
        x[v]++;
        vis[i] = true;
    }
    return ans;
}

std::vector<int> vis(4);

int dfs(int u, int d) {
    if (u == n - 1) return w[d][u];
    std::vector<int> a(4);
    for (int i = 1; i <= 3; i++) {
        if (vis[i] < n/2) {
            vis[i]++;
            a[i] = dfs(u + 1, i);
            vis[i]--;
        }
    }
    auto ma = std::max_element(a.begin(), a.end());
    return *ma + w[d][u];
}

inline int solve2() {
    vis.assign(4, 0);
    return std::max(dfs(0, 1), std::max(dfs(0, 2), dfs(0, 3)));
}

signed main() {
    freopen("club.in", "r", stdin);
    freopen("club.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> t;
    while (t--) {
        std::cin >> n;
        w[1].assign(n, 0), w[2].assign(n, 0), w[3].assign(n, 0);
        for (int i = 0; i < n; i++) {
            std::cin >> w[1][i] >> w[2][i] >> w[3][i];
            if (w[2][i] || w[3][i]) flagA = false;
        }
        if (flagA) {
            std::sort(w[1].begin(), w[1].end(), std::greater<int>());
            std::cout << std::accumulate(w[1].begin(), w[1].begin() + n/2, 0);
        } else if (n <= 200) std::cout << solve2();
        else std::cout << solve1();
        std::cout << std::endl;
    }
    return 0;
}

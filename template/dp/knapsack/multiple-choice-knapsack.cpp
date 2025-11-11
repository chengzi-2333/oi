// {P1757}
#include <bits/stdc++.h>

int n, c;
std::unordered_map<int, std::vector<std::pair<int, int>>> items;
std::vector<int> f;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("multiple-choice-knapsack.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> c >> n;
    for (int v, w, k, i = 1; i <= n; i++) {
        std::cin >> v >> w >> k;
        items[k].emplace_back(v, w);
    }
    f.resize(c + 1);
    for (const auto& [k, group] : items) {
        for (int j = c; j >= 0; j--) {
            for (const auto& [v, w] : group) {
                if (j >= v) f[j] = std::max(f[j], f[j - v] + w);
            }
        }
    }
    std::cout << f[c] << std::endl;
}

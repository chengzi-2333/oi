// {P1048}
#include <bits/stdc++.h>

int n, c;
std::vector<int> f;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("01-knapsack.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> c >> n;
    f.resize(c + 1);
    for (int v, w; n; n--) {
        std::cin >> v >> w;
        for (int j = c; j >= v; j--) {
            f[j] = std::max(f[j], f[j - v] + w);
        }
    }
    std::cout << f[c] << std::endl;
}

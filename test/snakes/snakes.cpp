#include <bits/stdc++.h>

constexpr char lf = '\n';
constexpr char sp = ' ';

int t, n;
std::vector<int> a;

int solve() {
    return 0;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("snakes1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> t >> n;
    a.resize(n);
    for (auto& i: a) std::cin >> i;
    std::cout << solve() << lf;
    while (--t) {
        std::cin >> n;
        for (int x, y; n; n--) {
            std::cin >> x >> y;
            a[x] = y;
        }
        std::cout << solve() << lf;
    }
}

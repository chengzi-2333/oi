#include <bits/stdc++.h>

#define int long long

constexpr int INF = 0x3f3f3f3f3f3f3f3f;
int n, m;
std::vector<int> height, width;

inline int solve1() {
    std::partial_sum(width.begin(), width.end(), width.begin());
    std::vector<int> f(n + 1, INF);
    f[0] = 0;
    for (int i = 1; i <= n; i++) {
        for (int ma = 0, j = i; j && width[i] - width[j - 1] <= m; j--) {
            ma = std::max(ma, height[j]);
            f[i] = std::min(f[i], f[j - 1] + ma);
        }
    }
    return f[n];
}

inline int solve2() {
    // TODO
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("shelf1.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin >> n >> m;
    height.resize(n + 1), width.resize(n + 1);
    for (int i = 1; i <= n; i++)
        std::cin >> height[i] >> width[i];
    std::cout << solve1() << std::endl;
}

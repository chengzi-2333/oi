#include <bits/stdc++.h>

#define int long long

constexpr int MOD = 998244353;

using TP = std::tuple<int, int, int>;

int c, t;
bool fA = true, fB = true;
int n, m;
std::vector<int> a;

// brute force
inline int solve() {
    int msum = 0, cnt = 0;
    for (int st = 0; st < (1 << n); st++) {
        std::vector<TP> ca;
        for (int i = 0; i < n; i++) {
            ca.emplace_back(a[i] >> !!(st & (1 << i)), a[i], -i);
        }
        std::sort(ca.rbegin(), ca.rend());
        int cost = 0, sum = 0;
        for (const auto& [r, v, i] : ca) {
            if (sum > m) break;
            int w = 1 + (r != v);
            if (m - sum >= w) {
                cost += w;
                sum += v;
            }
        }
        msum = std::max(msum, sum);
    }
    return msum;
}

signed main() {
    freopen("sale.in", "r", stdin);
    freopen("sale.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> c >> t;
    while (t--) {
        std::cin >> n >> m;
        a.resize(n);
        for (auto& v : a) {
            std::cin >> v;
        }
        std::cout << solve() << '\n';
    }
}

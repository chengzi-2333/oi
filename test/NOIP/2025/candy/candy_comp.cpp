#include <bits/stdc++.h>

#define int long long

using PII = std::pair<int, int>;

bool fA = true, nfB = true;
int n, m, ans;
std::priority_queue<PII, std::vector<PII>, std::greater<PII>> h;

signed main() {
    freopen("candy.in", "r", stdin);
    freopen("candy.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    for (int a, b, i = 1; i <= n; i++) {
        std::cin >> a >> b;
        if (a != b) fA = false;
        if (a > b) nfB = false;
        h.emplace(a, b);
    }
    if (fA) std::cout << (m / h.top().first);
    else /* if (nfB) */ {
        while (m >= 0) {
            auto [a, b] = h.top();
            h.pop();
            m -= a;
            if (m >= 0) ans++;
            h.emplace(b, a);
        }
        std::cout << ans;
    }
    std::cout << std::endl;
}

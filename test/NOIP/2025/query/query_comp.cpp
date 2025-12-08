#include <bits/stdc++.h>

#define int long long

int n, q;
std::vector<int> a(1);

signed main() {
    freopen("query.in", "r", stdin);
    freopen("query.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    std::copy_n(std::istream_iterator<int>(std::cin), n, 
             std::back_insert_iterator<std::vector<int>>(a));
    std::partial_sum(a.begin(), a.end(), a.begin());
    std::cin >> q;
    for (int u, v; q; q--) {
        std::cin >> u >> v;
        unsigned int s = 0;
        for (int i = 1; i <= n; i++) {
            int k = LONG_LONG_MIN;
            for (int len = u; len <= v; len++) {
                for (int l = 1; l + len - 1 <= n; l++) {
                    int r = l + len - 1;
                    if (i < l || i > r) continue;
                    k = std::max(k, a[r] - a[l - 1]);
                }
            }
            s ^= i * k;
        }
        std::cout << s << '\n';
    }
}

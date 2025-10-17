#include <bits/stdc++.h>

constexpr int N = 1e4;
int n;
int a[N + 5], f[N + 5];

inline int lis(const std::function<bool(int, int)>& cmp) {
    auto rev_cmp = std::bind(cmp, std::placeholders::_2, std::placeholders::_1);
    memset(f, 0, (n+1) * sizeof(int));
    int* fp = f + 1;
    *fp = *(a + 1);
    for (auto ap = a + 2; ap - a <= n; ap++) {
        (cmp(*ap, *fp) ? *++fp : *std::lower_bound(f + 1, fp + 1, *ap, rev_cmp)) = *ap;
    }
    return fp - f;
}

signed main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    for (int i = 1; i <= n; i++)
        std::cin >> a[i];
    std::cout << lis(std::greater<>()) << std::endl;
}

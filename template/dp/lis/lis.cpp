#include <bits/stdc++.h>

constexpr int N = 1e4;
int n;
int a[N + 5], f[N + 5];

inline int lis(const std::function<bool(int, int)>& cmp) {
    memset(f, 0, (n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) {
        f[i] = 1;
        for (int j = 0; j < i; j++) {
            if (cmp(a[i], a[j])) {
                f[i] = std::max(f[i], f[j] + 1);
            }
        }
    }
    return *std::max_element(f, f + n + 1);
}

signed main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    for (int i = 1; i <= n; i++)
        std::cin >> a[i];
    std::cout << lis(std::greater<>()) << std::endl;
}

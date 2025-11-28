#include <bits/stdc++.h>

constexpr int N = 1e4;
int n;
std::vector<int> a(1);
int f[N + 5];

template <typename C>
inline int lis(const C& cmp) {
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
    std::copy_n(std::istream_iterator<int>(std::cin), n,
                std::back_insert_iterator<std::vector<int>>(a));
    std::cout << lis(std::greater<>()) << std::endl;
}

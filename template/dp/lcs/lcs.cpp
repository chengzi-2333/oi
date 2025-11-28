// {P1439}
// bruteforce O(n^2)
#include <bits/stdc++.h>

int n;
std::vector<int> a(1), b(1);
std::vector<std::vector<int>> f;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("lcs.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    std::copy_n(std::istream_iterator<int>(std::cin), n,
                std::back_insert_iterator<std::vector<int>>(a));
    std::copy_n(std::istream_iterator<int>(std::cin), n,
                std::back_insert_iterator<std::vector<int>>(b));
    f.resize(n + 1, std::vector<int>(n + 1));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            f[i][j] = (a[i] == b[j]) ? f[i - 1][j - 1] + 1
                                     : std::max(f[i - 1][j], f[i][j - 1]);
        }
    }
    std::cout << f[n][n] << std::endl;
}

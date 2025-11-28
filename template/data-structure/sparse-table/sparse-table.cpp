// {P3865}
#include <bits/stdc++.h>

int n, t;
std::vector<std::vector<int>> tbl;

inline void build() {
    for (int i = 1; i <= std::__lg(n); i++) {
        for (int j = 1; j <= n - (1 << (i - 1)) + 1; j++) {
            tbl[i][j] = std::max(tbl[i - 1][j], tbl[i - 1][j + (1 << (i - 1))]);
        }
    }
}

inline int query(int l, int r) {
    int k = std::__lg(r - l + 1);
    return std::max(tbl[k][l], tbl[k][r - (1 << k) + 1]);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("sparse-table.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> t;
    tbl.resize(std::__lg(n) + 1, std::vector<int>(n + 2));
    std::copy_n(std::istream_iterator<int>(std::cin), n,
                tbl.front().begin() + 1);
    build();
    for (int l, r; t; t--) {
        std::cin >> l >> r;
        std::cout << query(l, r) << '\n';
    }
}

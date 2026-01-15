#include <bits/stdc++.h>

#define int long long

constexpr int MOD = 1e9 + 7;

template <typename T>
struct Mat {
    // TODO
    typedef std::vector<std::vector<T>> container;
    typedef container::iterator iterator;
    container dat;

    Mat(int rows, int cols);

    Mat<T> operator*(const Mat<T>& v);

    iterator begin();

    iterator end();

    template <typename T>
    Mat<T> qpow(T p);
};

int n, k;

template <typename T, typename N>
T qpow(T n, N p) {
    T ans;
    while (p) {
        if (p & 1) ans = ans * n;
        n = n * n;
        p >>= 1;
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("matrix-qpow.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> k;
    Mat<int> mat(n, n);
    for (auto& i : mat) {
        for (auto& j : i) {
            std::cin >> j;
        }
    }
    Mat<int> ans = qpow(mat, k);
    for (const auto& i : ans) {
        for (const auto& j : i) {
            std::cout << j;
        }
        std::cout << '\n';
    }
}

// {P3390}
#include <bits/stdc++.h>

#define int long long

constexpr int MOD = 1e9 + 7;

template <typename T>
struct SqrMat {
    typedef std::vector<std::vector<T>> C;
    typedef typename C::iterator Iter;
    C dat;

    int n = 0;

    SqrMat(int _n) : n(_n) { dat.resize(n, std::vector<T>(n)); }

    void identity() {
        for (int i = 0; i < n; i++) {
            dat[i][i] = 1;
        }
    }

    SqrMat<T> operator*(const SqrMat<T>& v) const {
        SqrMat<T> res(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    res.dat[i][j] =
                        (res.dat[i][j] + dat[i][k] * v.dat[k][j]) % MOD;
                }
            }
        }
        return res;
    }

    Iter begin() { return dat.begin(); }

    Iter end() { return dat.end(); }

    template <typename P>
    SqrMat<T> pow(P p) const {
        SqrMat<T> ans(n), b(*this);
        ans.identity();
        while (p) {
            if (p & 1) ans = ans * b;
            b = b * b;
            p >>= 1;
        }
        return ans;
    }
};

int n, k;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("matrix-qpow.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> k;
    SqrMat<int> mat(n);
    for (auto& i : mat) {
        for (auto& j : i) {
            std::cin >> j;
        }
    }
    for (const auto& i : mat.pow(k)) {
        for (const auto& j : i) {
            std::cout << j << ' ';
        }
        std::cout << '\n';
    }
}

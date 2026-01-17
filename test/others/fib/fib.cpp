// {P1349}
#include <bits/stdc++.h>

#define int unsigned long long

int p, q, a[2], n, m;

struct SqrMat2i {
    int dat[2][2]{};

    SqrMat2i operator*(const SqrMat2i& v) const {
        SqrMat2i res;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    res.dat[i][j] =
                        (res.dat[i][j] + (dat[i][k] * v.dat[k][j]) % m) % m;
                }
            }
        }
        return res;
    }

    template <typename P>
    SqrMat2i pow(P p) {
        SqrMat2i res{{{1, 0}, {0, 1}}}, b(*this);
        while (p) {
            if (p & 1) res = res * b;
            b = b * b;
            p >>= 1;
        }
        return res;
    }
} mat, vec;

/*
[   a_{n}   ] = [ p q ][ a_{n - 1} ]
[ a_{n - 1} ] = [ 1 0 ][ a_{n - 2} ]
*/

signed main() {
#ifndef ONLINE_JUDGE
    freopen("fib.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> p >> q >> a[0] >> a[1] >> n >> m;
    mat = {{{p, q}, {1, 0}}}, vec = {{{a[1], 0}, {a[0], 0}}};
    if (n > 2) {
        const auto& res = mat.pow(n - 2) * vec;
        std::cout << res.dat[0][0];
    } else
        std::cout << a[n - 1] % m;
    std::cout << std::endl;
}

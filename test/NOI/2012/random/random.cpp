// {P2044}
#include <bits/stdc++.h>

#define int unsigned long long

int m, a, c, x, n, g;

struct SqrMat2i {
    __int128 dat[2][2]{};

    SqrMat2i operator*(const SqrMat2i& v) {
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

signed main() {
#ifndef ONLINE_JUDGE
    freopen("random.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> m >> a >> c >> x >> n >> g;
    mat = {{{a, c}, {0, 1}}}, vec = {{{x, 0}, {1, 0}}};
    std::cout << (int)((mat.pow(n) * vec).dat[0][0] % g) << std::endl;
}

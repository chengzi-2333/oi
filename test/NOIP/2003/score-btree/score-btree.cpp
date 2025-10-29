// {P1040}
#include <bits/stdc++.h>

int n;
std::vector<std::vector<unsigned int>> f, rt;

void put(int l, int r) {
    if (l > r) return;
    std::cout << rt[l][r] << ' ';
    if (l == r) return;
    put(l, rt[l][r] - 1), put(rt[l][r] + 1, r);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("score-btree.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    f.resize(n + 1, std::vector<unsigned int>(n + 1));
    rt.resize(n + 1, std::vector<unsigned int>(n + 1));
    for (int i = 1; i <= n; i++) {
        std::cin >> f[i][i];
        rt[i][i] = i;
    }
    for (int len = 1; len <= n; len++) {
        for (int l = 1, r = l + len; r <= n; l++, r = l + len) {
            f[l][r] = f[l + 1][r] + f[l][l];
            rt[l][r] = l;
            for (int k = l + 1; k < r; k++) {
                auto val = f[l][k - 1] * f[k + 1][r] + f[k][k];
                if (val > f[l][r]) {
                    f[l][r] = val;
                    rt[l][r] = k;
                }
            }
        }
    }
    std::cout << f[1][n] << '\n';
    put(1, n);
    std::cout << std::endl;
}

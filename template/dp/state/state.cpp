// {P1879}
#include <bits/stdc++.h>

constexpr int MOD = 1e8;
int m, n;
std::vector<int> field;
std::vector<std::vector<int>> f;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("state.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> m >> n;
    field.resize(m), f.resize(m + 2, std::vector<int>(1 << n));
    for (auto& state : field) {
        for (int flag, j = 0; j < n; j++) {
            std::cin >> flag;
            state |= (flag << j);
        }
    }
    field.push_back(0);
    f[0][0] = 1;
    for (int i = 1; i <= m + 1; i++) {
        for (int j = 0; j < (1 << n); j++) {
            for (int k = 0; k < (1 << n); k++) {
                if ((~field[i - 1] & j) || (k & j) || (j & (j << 1))) continue;
                f[i][j] = (f[i][j] % MOD + f[i - 1][k] % MOD) % MOD;
            }
        }
    }
    std::cout << f[m + 1][0] << std::endl;
}

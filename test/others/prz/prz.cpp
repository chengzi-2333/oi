// {P5911}
#include <bits/stdc++.h>

int c, n;
std::vector<int> w, t, tm, wt, f;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("prz.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> c >> n;
    w.resize(n + 1), t.resize(n + 1);
    tm.resize(1 << n), wt.resize(1 << n);
    for (int i = 1; i <= n; i++) std::cin >> t[i] >> w[i];
    for (int i = 0; i < (1 << n); i++) {
        for (int j = 1; j <= n; j++) {
            if (i & (1 << (j - 1))) {
                tm[i] = std::max(tm[i], t[j]);
                wt[i] += w[j];
            }
        }
    }
    f.resize(1 << n, 0x3f3f3f3f);
    f[0] = 0;
    for (int i = 0; i < (1 << n); i++) {
        for (int j = i; ; j = i & (j - 1)) {
            if (wt[i ^ j] <= c) f[i] = std::min(f[i], f[j] + tm[i ^ j]);
            if (!j) break;
        }
    }
    std::cout << f[(1 << n) - 1] << std::endl;
}

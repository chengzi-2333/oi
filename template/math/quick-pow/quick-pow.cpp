// {P1226}
#include <bits/stdc++.h>

template <typename T>
T qpow(T n, T p, T m) {
    T ans = 1;
    while (p) {
        if (p & 1) ans = ans * n % m;
        n = n * n % m;
        p >>= 1;
    }
    return ans;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("quick-pow.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    long long n, p, m;
    std::cin >> n >> p >> m;
    printf("%lld^%lld mod %lld=%lld", n, p, m, qpow(n, p, m));
}

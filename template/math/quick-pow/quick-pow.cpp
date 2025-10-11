// {P1226}
#include <bits/stdc++.h>

using namespace std;

long long n, p, m;

long long qpow(long long n, long long p, long long m) {
    long long ans = 1;
    while (p) {
        if (p & 1) ans *= n % m;
        ans %= m;
        n *= n % m;
        n %= m;
        p >>= 1;
    }
    return ans;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("quick-pow.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> p >> m;
    printf("%lld^%lld mod %lld=%lld", n, p, m, qpow(n, p, m));
}

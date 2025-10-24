#include <bits/stdc++.h>


inline void output(int a, int b) {
    if ((a < 0) ^ (b < 0)) std::cout << '-';
    a = std::abs(a), b = std::abs(b);
    int fac = std::__gcd(a, b);
    a /= fac, b /= fac;
    std::cout << a;
    if (b != 1) std::cout << '/' << b;
    std::cout << std::endl;
}

int n, ma = INT_MIN, mi = INT_MAX, nma = INT_MIN, nmi = INT_MAX;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("capacity4.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin >> n;
    if (n == 2) {
        std::cin >> ma >> mi;
        if (std::abs(ma) < std::abs(mi) && !((ma < 0) ^ (mi < 0))) 
            std::swap(ma, mi);
        output(ma, mi);
    } else {
        for (int v; n; n--) {
            std::cin >> v;
            if (v > 0) {
                ma = std::max(ma, v);
                mi = std::min(mi, v);
            } else {
                nma = std::max(nma, v);
                nmi = std::min(nmi, v);
            }
        }
        if ((nma == INT_MIN) || (nmi == INT_MAX)) output(ma, mi);
        else if ((ma == INT_MIN) || (mi == INT_MAX)) output(nmi, nma);
        // a/b > c/d => a*d > c*b
        else if (ma * -nma > mi * -nmi) output(ma, mi);
        else output(nmi, nma);
    }
}

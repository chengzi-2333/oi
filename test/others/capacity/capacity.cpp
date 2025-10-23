#include <bits/stdc++.h>


int n, ma, mi = INT_MAX, fac;

signed main() {
#ifndef ONLINE_JUDGE
    // freopen("capacity.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin >> n;
    if (n == 2) {
        std::cin >> ma >> mi;
        ma = std::abs(ma), mi = std::abs(mi);
        if (ma < mi) std::swap(ma, mi);
        fac = std::__gcd(ma, mi);
        ma /= fac, mi /= fac;
        std::cout << ma;
        if (mi != 1) std::cout << '/' << mi;
        std::cout << std::endl;
        // TODO: negative
    }
    else if (n <= 3000) {
        for (int v; n; n--) {
            std::cin >> v;
            ma = std::max(ma, std::abs(v));
            mi = std::min(mi, std::abs(v));
        }
        if (ma < mi) std::swap(ma, mi);
        fac = std::__gcd(ma, mi);
        ma /= fac, mi /= fac;
        std::cout << ma;
        if (mi != 1) std::cout << '/' << mi;
        std::cout << std::endl;
    }
    else {
        // TODO
    }
}

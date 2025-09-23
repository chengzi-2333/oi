#include <bits/stdc++.h>

constexpr char lf = '\n';
constexpr char sp = ' ';

int t, n;
std::vector<int> a;

int solve() {  // TODO
    std::deque<int> p(a.begin(), a.end());
    while (p.size() > 1) {
        if (p.back() - p.front() > *(p.rbegin()+1)) {  // Just eat it!
            p.back() -= p.front();
            p.pop_front();
        } else break;
    }
    return p.size();
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("snakes1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> t >> n;
    a.resize(n);
    for (auto& i: a) std::cin >> i;
    std::cout << solve() << lf;
    while (--t) {
        std::cin >> n;
        for (int x, y; n; n--) {
            std::cin >> x >> y;
            a[x-1] = y;
        }
        std::cout << solve() << lf;
    }
}

// {P2678}
#include <bits/stdc++.h>

int d, n, m;
std::vector<int> a;

bool check(int x) {
    int sum = 0;
    int u = 0;
    for (const auto& v: a) {
        if (v-u < x) sum++;
        else u = v;
    }
    return sum <= m;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("jumping-rock.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> d >> n >> m;
    a.resize(n);
    for (auto& i: a) std::cin >> i;
    a.push_back(d);
    int l = 1, r = d;
    while (l <= r) {
        int mid = l+((r-l)>>1);
        if (check(mid)) l = mid+1;
        else r = mid-1;
    }
    std::cout << r << std::endl;
}

// {P3368}
#include <bits/stdc++.h>

int n, m;
std::vector<int> arr;

inline int lowbit(int x) { return x & -x; }

inline void update(int x, int k) {
    for (; x <= n; x += lowbit(x)) arr[x] += k;
}

inline int query(int x) {
    int sum = 0;
    for (; x; x -= lowbit(x)) sum += arr[x];
    return sum;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("bit-diff.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    arr.resize(n + 1);
    for (int a, p = 0, i = 1; i <= n; i++) {
        std::cin >> a;
        update(i, a - p);
        p = a;
    }
    for (int op, x, y, k; m; m--) {
        std::cin >> op >> x;
        if (op - 1)
            std::cout << query(x) << '\n';
        else {
            std::cin >> y >> k;
            update(y + 1, -k), update(x, k);
        }
    }
}

// {P3374}
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

inline int query(int l, int r) { return query(r) - query(l - 1); }

signed main() {
#ifndef ONLINE_JUDGE
    freopen("bit.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    arr.resize(n + 1);
    for (int a, i = 1; i <= n; i++) {
        std::cin >> a;
        update(i, a);
    }
    for (int op, x, y; m; m--) {
        std::cin >> op >> x >> y;
        if (op - 1)
            std::cout << query(x, y) << '\n';
        else
            update(x, y);
    }
}

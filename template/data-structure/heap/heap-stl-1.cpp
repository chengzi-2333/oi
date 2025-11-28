// {P3378}
// 开摆！
#include <bits/stdc++.h>

constexpr char lf = '\n';

std::priority_queue<int, std::vector<int>, std::greater<int>> heap;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("heap.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n;
    std::cin >> n;
    for (int opt, x; n; n--) {
        std::cin >> opt;
        switch (opt) {
            case 1:
                std::cin >> x;
                heap.push(x);
                break;
            case 2:
                std::cout << heap.top() << lf;
                break;
            case 3:
                heap.pop();
                break;
        }
    }
}

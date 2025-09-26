// 另一种做法（~~摆烂法~~）
// 实际上就是对二叉堆中up(x)和down(x)的封装
#include <bits/stdc++.h>

constexpr char lf = '\n';

std::vector<int> h;
auto cmp = std::greater<int>();

signed main() {
#ifndef ONLINE_JUDGE
    freopen("heap.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n; std::cin >> n;
    for (int opt, x; n; n--) {
        std::cin >> opt;
        switch (opt) {
            case 1:
                std::cin >> x;
                h.push_back(x);
                std::push_heap(h.begin(), h.end(), cmp);  // 模板库真好玩儿嘿嘿嘿
                break;
            case 2:
                std::cout << h.front() << lf;
                break;
            case 3:
                std::pop_heap(h.begin(), h.end(), cmp);
                h.pop_back();
                break;
        }
    }
}

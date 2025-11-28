// {P1886}
#include <bits/stdc++.h>

int n, k;
std::vector<int> arr(1);

template <typename C>
inline void run(const C& cmp) {
    std::deque<std::pair<int, int>> que;
    for (int i = 1; i <= n; i++) {
        while (!que.empty() && cmp(que.back().second, arr[i])) que.pop_back();
        que.emplace_back(i, arr[i]);
        while (!que.empty() && que.front().first <= i - k) que.pop_front();
        if (i >= k) std::cout << que.front().second << ' ';
    }
    std::cout << '\n';
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("monotonous-queue.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> k;
    std::copy_n(std::istream_iterator<int>(std::cin), n,
                std::back_insert_iterator<std::vector<int>>(arr));
    run(std::greater_equal<int>());
    run(std::less_equal<int>());
}

// {P14361}
#include <bits/stdc++.h>

#define int long long

int t, n;
std::vector<std::array<int, 3>> arr;

int solve() {
    int ans = 0;
    std::vector<int> choice, regret;
    std::array<int, 3> cnt;
    cnt.fill(0);
    for (const auto& it : arr) {
        auto ma = std::max_element(it.begin(), it.end());
        auto dis = std::distance(it.begin(), ma);
        ans += *ma;
        choice.push_back(dis);
        cnt[dis]++;
    }
    auto it = choice.begin();
    for (const auto& [a, b, c] : arr) {
        if (*it == 0 && cnt[0] > n / 2) regret.push_back(a - std::max(b, c));
        else if (*it == 1 && cnt[1] > n / 2) regret.push_back(b - std::max(a, c));
        else if (*it == 2 && cnt[2] > n / 2) regret.push_back(c - std::max(a, b));
        it++;
    }
    std::sort(regret.begin(), regret.end());
    int residual = 0;
    for (const auto& c : cnt) residual += std::max(0ll, c - n / 2);
    return ans - std::accumulate(regret.begin(), regret.begin() + residual, 0ll);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("club1.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> t;
    while (t--) {
        std::cin >> n;
        arr.resize(n);
        for (auto& [a, b, c] : arr) std::cin >> a >> b >> c;
        std::cout << solve() << '\n';
    }
}

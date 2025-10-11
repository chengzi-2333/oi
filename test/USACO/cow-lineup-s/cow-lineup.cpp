// {P3029}
#include <bits/stdc++.h>

int n, tot, ans = INT_MAX;
std::vector<std::pair<int, int>> cows;
std::unordered_map<int, int> tag;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("cow-lineup.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    cows.resize(n);
    for (auto& [p, t]: cows) {
        std::cin >> p >> t;
        if (!tag[t]) tot++;
        tag[t]++;
    }
    tag.clear();
    std::sort(cows.begin(), cows.end());
    auto l = cows.begin(), r = cows.begin();
    tag[l->second]++;
    for (int sum = 1; l <= r; ) {
        if (sum == tot) (ans = std::min(ans, r->first - l->first)), sum -= !(--tag[l->second]), l++;
        else if (sum > tot) sum -= !(--tag[l->second]), l++;
        else if (sum < tot && r+1 < cows.end()) r++, sum += !(tag[r->second]++);
        else sum -= !(--tag[l->second]), l++;
    }
    std::cout << ans << std::endl;
}

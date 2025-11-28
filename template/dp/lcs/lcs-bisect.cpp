// {P1439}
// LCS -> LIS bisect O(n * log(n))
#include <bits/stdc++.h>

int n;
std::vector<int> arr, seq;
std::unordered_map<int, int> mp;

template <typename C>
inline int lis(const C& cmp) {
    const auto rev_cmp = std::bind(cmp, std::placeholders::_2, std::placeholders::_1);
    seq.clear();
    for (const auto& v : arr) {
        if (seq.empty() || cmp(v, seq.back())) seq.push_back(v);
        else *std::lower_bound(seq.begin(), seq.end(), v, rev_cmp) = v;
    }
    return seq.size();
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("lcs.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    arr.resize(n + 1);
    for (int v, i = 1; i <= n; i++) {
        std::cin >> v;
        mp[v] = i;
    }
    for (int v, i = 0; i < n; i++) {
        std::cin >> v;
        arr[i] = mp[v];
    }
    std::cout << lis(std::greater<>()) << std::endl;
}

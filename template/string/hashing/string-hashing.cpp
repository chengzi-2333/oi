// {P3370}
#include <bits/stdc++.h>

using ull = unsigned long long;

constexpr ull BASE = 131;
constexpr ull MOD1 = 834625873538721586, MOD2 = 1e9 + 7;

inline ull hash(const std::string& s, const ull& mod) {
    return std::accumulate(s.begin(), s.end(), 0ull, [&mod](ull val, ull chr) {
        return (val * BASE + chr) % mod;
    });
}

inline bool cmp(const std::string& a, const std::string& b) {
    return (hash(a, MOD1) == hash(b, MOD1)) && (hash(a, MOD2) == hash(b, MOD2));
}

int n;
std::string s;
std::vector<std::pair<ull, ull>> hs;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("string-hashing.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    while (n--) {
        std::cin >> s;
        hs.emplace_back(hash(s, MOD1), hash(s, MOD2));
    }
    std::sort(hs.begin(), hs.end());
    std::cout << std::distance(hs.begin(), std::unique(hs.begin(), hs.end()))
              << std::endl;
}

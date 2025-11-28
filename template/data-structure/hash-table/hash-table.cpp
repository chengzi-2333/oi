// {P11615}
#include <bits/stdc++.h>

using ull = unsigned long long;

constexpr ull MOD1 = 18936871903456093, MOD2 = 1e9 + 7;

struct MyHash {
    size_t operator()(const ull& idx) const {
        // return std::_Fnv_hash_impl::hash(idx);
        return (idx % MOD1) ^ ((idx % MOD2) << 1);
    }
};

ull n, ans;
std::unordered_map<ull, ull, MyHash> mp;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("hash-table.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    for (ull x, y, i = 1; i <= n; i++) {
        std::cin >> x >> y;
        ans += i * mp[x];
        mp[x] = y;
    }
    std::cout << ans << std::endl;
}

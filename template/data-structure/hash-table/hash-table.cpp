// {P11615}
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/mman.h>

char* rp;

template <typename T>
inline void fast_read(T& x) {
    char c;
    x = 0;
    while (!isdigit(c = *rp++));
    do x = x * 10 + (c - '0');
    while (isdigit(c = *rp++));
}

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
    struct stat st;
    fstat(0, &st);
    rp = (char*) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
    fast_read(n);
    for (ull x, y, i = 1; i <= n; i++) {
        fast_read(x), fast_read(y);
        ans += i * mp[x];
        mp[x] = y;
    }
    std::cout << ans << std::endl;
}

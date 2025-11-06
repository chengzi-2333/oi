#include <bits/stdc++.h>

#define int long long

constexpr int MOD = 998244353;

int factorial(int x) {
    int ans = 1;
    for (int i = 1; i <= x; i++) {
        ans = (ans * i) % MOD;
    }
    return ans;
}

bool flag = true;
int n, m;
std::string diff;
std::vector<int> pat;

signed main() {
    freopen("employ.in", "r", stdin);
    freopen("employ.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> diff;
    pat.resize(n);
    for (auto& p : pat) std::cin >> p;
    for (const auto& c : diff) if (c != '1') flag = false;
    if (flag) {
        std::cout << factorial(n - std::count(pat.begin(), pat.end(), 0));
    }
    else {}
    std::cout << std::endl;
    return 0;
}

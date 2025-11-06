#include <bits/stdc++.h>

#define int long long

int n, q;
std::vector<std::pair<std::string, std::string>> p;

inline int bruteforce(const std::string& a, const std::string& b) {
    int ans = 0;
    for (const auto& s : p) {
        if (a.length() != b.length()) continue;
        int l = a.find(s.first), r = l + s.first.length() - 1;
        if (l == a.npos) continue;
        bool flag = true;
        for (int i = 0; i < b.length(); i++) {
            if (i >= l && i <= r) {
                if (s.second[i-l] != b[i]) {
                    flag = false;
                    break;
                }
            } else if (a[i] != b[i]) {
                flag = false;
                break;
            }
        }
        if (flag) ans++;
    }
    return ans;
}

signed main() {
    freopen("replace.in", "r", stdin);
    freopen("replace.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> q;
    p.resize(n);
    for (auto& pa : p) {
        std::cin >> pa.first >> pa.second;
    }
    for (std::string a, b; q; q--) {
        std::cin >> a >> b;
        std::cout << bruteforce(a, b) << '\n';
    }
    return 0;
}

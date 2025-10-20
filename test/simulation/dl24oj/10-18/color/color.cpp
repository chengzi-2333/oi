#include <bits/stdc++.h>

constexpr int N = 2e5;
int t, n, k, cnt[26];
char s[N+2];

inline int solve() {
    int c = 0;
    for (int i = 0; i < n; i++) {
        if (cnt[s[i]-'a']++ == 0) {
            c++;
        }
    }
    int ans = 0;
    if (c == 1) ans = n / k;
    else {
        int pa = 0, lf = 0;
        for (int i = 0; i < 26; i++) {
            pa += cnt[i] / 2;
            lf += cnt[i] % 2;
        }
        ans = pa / k * 2;
        lf += (pa % k) * 2;
        if (lf >= k) ans++;
    }
    return ans;
}

int main() {
    freopen("color.in", "r", stdin);
    freopen("color.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> t;
    while (t--) {
        memset(cnt, 0, sizeof(cnt));
        std::cin >> n >> k >> s;
        std::cout << solve() << '\n';
    }
}

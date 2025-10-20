#include <iostream>
#include <cstring>

int t, n, k, cnt[26];
std::string s;

inline int solve2() {
    int x = 0, y = 0;
    for (int i = 0; i < 26; i++) {
        if (cnt[i]) (x ? y : x) = cnt[i];
    }
    // 一共有 (x/2+y/2) 对 , 分配到 k 组 , 每组能得到 (x/2+y/2)/k 对
    int pa = (x / 2 + y / 2);
    int ans = pa / k * 2;
    if (x % 2 + y % 2 + pa % k * 2 >= k) ans++; // 每组还能多分个字母插在中间
    return ans;
}

inline int solve() {
    int pa = 0, lf = 0; // 有多少个成对的字母 , 除了成对的以外 , 还剩多少个
    for (int i = 0; i < 26; i++) {
        pa += cnt[i] / 2;
        lf += cnt[i] % 2;
    }
    int ans = pa / k * 2; // 每组得到的回文串的长度
    lf += (pa % k) * 2;   // 剩余的长度
    // 如果剩余的 lf>=k, 每组只能再加一个 , 放在回文串的中间
    ans += (lf >= k);
    return ans;
}

int main() {
    freopen("color1.in", "r", stdin);
    std::cin >> t;
    while (t--) {
        memset(cnt, 0, sizeof(cnt));
        int c = 0; // 统计有几种字母
        std::cin >> n >> k >> s;
        // 统计每种字母出现的次数
        for (const auto& si: s) if (!cnt[si-'a']++) c++;
        if (c == 1) std::cout << n / k; // 每个子串平均分 n/k 个字符 , 一定是回文
        else if (c == 2) std::cout << solve2();
        else std::cout << solve();
        std::cout << std::endl;
    }
}

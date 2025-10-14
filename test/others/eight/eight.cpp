// {P1379}
#include <bits/stdc++.h>

using State = std::tuple<int, int, std::string>;

int cnt;
const std::string target = "123804765";
std::string s;
std::queue<State> que;
std::unordered_set<std::string> st;

inline bool check(const int& dir, const int& p) {
    switch (dir) {
        case -1: return p % 3 != 0;
        case 1: return p % 3 != 2;
        case -3: return p > 2;
        case 3: return p < 6;
    }
    return false;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("eight.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> s;
    que.emplace(0, s.find('0'), s);
    while (!que.empty()) {
        auto [d, p, u] = que.front();
        que.pop();
        cnt++;
        if (u == target) {
            std::cout << d << std::endl;
            break;
        }
        st.insert(u);
        for (const auto& dir: {-1, 1, -3, 3}) {
            std::string v(u);
            if (check(dir, p)) {
                std::swap(v[p], v[p+dir]);
                if (st.find(v) == st.end()) {
                    que.emplace(d+1, p+dir, v);
                }
            }
        }
    }
    // std::cout << cnt << std::endl;
}

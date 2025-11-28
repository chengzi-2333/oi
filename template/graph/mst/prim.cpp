// {P3366}
#include <bits/stdc++.h>

using PII = std::pair<int, int>;
constexpr int N = 5000;
int n, m;
std::bitset<N + 2> vis;
std::vector<PII> g[N + 2];
std::priority_queue<PII, std::vector<PII>, std::greater<>> h;

inline int prim() {
    int ans = 0;
    h.emplace(0, 1);
    while (!h.empty()) {
        auto [wu, u] = h.top();
        h.pop();
        if (vis.test(u)) continue;
        vis.set(u);
        ans += wu;
        for (const auto& p : g[u]) {
            if (!vis.test(p.second)) h.push(p);
        }
    }
    return (vis.count() == n) ? ans : -1;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("mst.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    for (int u, v, w; m; m--) {
        std::cin >> u >> v >> w;
        g[u].emplace_back(w, v);
        g[v].emplace_back(w, u);
    }
    int ans = prim();
    if (ans == -1)
        std::cout << "orz";
    else
        std::cout << ans;
    std::cout << std::endl;
}

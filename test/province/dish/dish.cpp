// {P3243}
#include <bits/stdc++.h>

int t, n, m;
std::vector<int> din;
std::vector<std::vector<int>> g;

std::vector<int> topo() {
    std::priority_queue<int> que;
    std::vector<int> seq;
    for (int i = 1; i <= n; i++) {
        if (din[i] == 0) que.push(i);
    }
    while (!que.empty()) {
        int u = que.top();
        que.pop();
        seq.push_back(u);
        for (const auto& v : g[u]) {
            if (--din[v] == 0) que.push(v);
        }
    }
    return seq;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("dish.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> t;
    while (t--) {
        std::cin >> n >> m;
        din.assign(n + 1, {}), g.assign(n + 1, {});
        for (int u, v; m; m--) {
            std::cin >> v >> u;
            g[u].push_back(v);
            din[v]++;
        }
        auto seq = topo();
        if (seq.size() < n)
            std::cout << "Impossible!";
        else
            for (const auto& u : std::views::reverse(seq))
                std::cout << u << ' ';
        std::cout << std::endl;
    }
}

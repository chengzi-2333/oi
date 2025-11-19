// {P4667}
#include <bits/stdc++.h>

constexpr int INF = 0x3f3f3f3f;
std::vector<std::vector<std::pair<int, int>>> g;

int bfs(int s, int t) {
    std::vector<int> dis(g.size(), INF);
    // std::vector<bool> vis(g.size());
    std::deque<int> que;
    que.push_back(s);
    dis[s] = 0;
    while (!que.empty()) {
        int u = que.front();
        que.pop_front();
        if (u == t) break;
        // if (vis[u]) continue;
        // vis[u] = true;
        for (const auto& [v, w] : g[u]) {
            if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                if (w) que.push_back(v);
                else que.push_front(v);
            }
        }
    }
    return dis[t];
}

int n, m;

inline int state(int x, int y) {
    return x * (m + 1) + y + 1;
}

inline void add_edge(int u, int v, int w) {
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("01-bfs.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize((n + 1) * (m + 1) + 1);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < m; y++) {
            char c;
            std::cin >> c;
            for (const auto& dx : {0, 1}) {
                for (const auto& dy : {0, 1}) {
                    add_edge(state(x + dx, y + dy), state(x + !dx, y + !dy), (c == '\\') ^ (dx == dy));
                }
            }
        }
    }
    auto sol = bfs(state(0, 0), state(n, m));
    if (sol == INF) std::cout << "NO SOLUTION";
    else std::cout << sol;
    std::cout << std::endl;
}

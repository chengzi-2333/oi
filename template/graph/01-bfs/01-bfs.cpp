// {P4667}
#include <bits/stdc++.h>

constexpr int INF = 0x3f3f3f3f;
std::vector<std::vector<std::pair<int, bool>>> g;

int bfs(int s, int t) {
    std::vector<int> dis(g.size(), INF);
    std::vector<bool> vis(g.size());
    std::deque<int> que;
    que.push_back(s);
    dis[s] = 0;
    while (!que.empty()) {
        int u = que.front();
        que.pop_front();
        if (u == t) break;
        if (vis[u]) continue;
        vis[u] = true;
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

inline bool check(int x, int y) {
    return x >= 0 && y >= 0 && x < n && y < m;
}

inline int state(int x, int y) {
    return x * m + y;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("01-bfs.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize(n * m);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < m; y++) {
            char c;
            std::cin >> c;
            bool flag = (c == '\\');
            for (const auto& dx : {-1, 1}) {
                for (const auto& dy : {-1, 1}) {
                    int tx = x + dx, ty = y + dy;
                    bool w = flag ^ (dx * dy > 0);
                    if (!check(tx, ty)) continue;
                    g[state(x, y)].emplace_back(state(tx, ty), w);
                    g[state(tx, ty)].emplace_back(state(x, y), w);
                }
            }
        }
    }
    auto sol = bfs(state(0, 0), state(n - 1, m - 1));
    if (sol == INF) std::cout << "NO SOLUTION";
    else std::cout << sol;
    std::cout << std::endl;
}

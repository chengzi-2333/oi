// {P4667}
#include <bits/stdc++.h>

constexpr int INF = 0x3f3f3f3f;
int n, m;
std::vector<std::vector<bool>> g;

inline bool check(int x, int y) {
    return x >= 0 && y >= 0 && x < n && y < m;
}

int bfs() {  // TODO
    std::vector<std::vector<int>> dis(n, std::vector<int>(m, INF));
    std::deque<std::pair<int, int>> que;
    que.emplace_front(0, 0);
    dis[0][0] = 0;
    while (!que.empty()) {
        const auto [x, y] = que.front();
        que.pop_front();
        for (const auto& dx : {1, -1}) {
            for (const auto& dy : {1, -1}) {
                if (check(x + dx, y + dy)) {
                    bool flag = g[x][y] ^ g[x + dx][y + dy];
                    if (dis[x + dx][y + dy] > dis[x][y] + flag) {
                        dis[x + dx][y + dy] = dis[x][y] + flag;
                        if (dis[x + dx][y + dy] > dis[x][y]) que.emplace_back(x + dx, y + dy);
                        else que.emplace_front(x + dx, y + dy);
                    }
                }
            }
        }
    }
    return dis.back().back();
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("01-bfs.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize(n, std::vector<bool>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            char c;
            std::cin >> c;
            g[i][j] = (c == '/');
        }
    }
    std::cout << bfs() << std::endl;
}

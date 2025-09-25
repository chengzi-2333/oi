#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

constexpr int INF = 0x3f3f3f3f;

using PII = pair<int, int>;
using Triple = tuple<int, int, int>;

int n, m, k, s, t;
vector<vector<PII>> g;
vector<vector<int>> d;  // d[p][k] 到p点，消耗k免费次数
vector<vector<bool>> vis;
std::priority_queue<Triple, vector<Triple>, std::greater<Triple>> q;

void dijkstra() {
    d[s][0] = 0;
    q.emplace(0, s, 0);
    while (!q.empty()) {
        auto [_, u, c] = q.top();
        q.pop();
        if (vis[u][c]) continue;
        vis[u][c] = true;
        for (const auto& [v, w]: g[u]) {
            if (c < k && d[u][c] < d[v][c+1]) {
                d[v][c+1] = d[u][c];
                q.emplace(d[v][c+1], v, c+1);
            }
            if (d[v][c] > d[u][c] + w) {
                d[v][c] = d[u][c] + w;
                q.emplace(d[v][c], v, c);
            }
        }
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("flight.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m >> k >> s >> t;
    g.resize(n+1); 
    vis.resize(n+1, vector<bool>(k+1));
    d.resize(n+1, vector<int>(k+1, INF));
    for (int u, v, w; m; m--) {
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
        g[v].emplace_back(u, w);
    }
    dijkstra();
    cout << *std::min_element(d[t].begin(), d[t].end()) << endl;
}

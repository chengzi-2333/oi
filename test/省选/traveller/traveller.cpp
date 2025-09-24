#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

constexpr long long INF = 0x3f3f3f3f3f3f3f3f;

using PLI = pair<long long, int>;

int n, m, k;
vector<vector<PLI>> g;
vector<long long> dis;
vector<bool> prefered, vis;
std::priority_queue<PLI, vector<PLI>, std::greater<PLI>> q;


inline long long dijkstra(int s) {
    while (!q.empty()) q.pop();
    std::fill(dis.begin(), dis.end(), INF);
    std::fill(vis.begin(), vis.end(), false);
    dis[s] = 0;
    q.emplace(dis[s], s);
    while (!q.empty()) {
        int u = q.top().second;
        q.pop();
        // return after find the second "prefered" node (just a single line!)
        if (prefered[u] && u != s) return dis[u];
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto& [w, v]: g[u]) {
            if (dis[u] + w < dis[v]) {
                dis[v] = dis[u] + w;
                q.emplace(dis[v], v);
            }
        }
    }
    return INF;
}

// brute force (optimized)
long long solve() {
    long long ans = INF;
    for (int s=1; s<=n; s++)
        if (prefered[s]) 
            ans = min(ans, dijkstra(s));
    return ans;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("traveller.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        cin >> n >> m >> k;
        g.resize(n+1); 
        dis.resize(n+1);
        prefered.resize(n+1);
        vis.resize(n+1);
        for (int u, v, w; m; m--) {
            cin >> u >> v >> w;
            if (u == v) continue;
            g[u].emplace_back(w, v);
        }
        for (int u; k; k--) {
            cin >> u;
            prefered[u] = true;
        }
        cout << solve() << lf;
        g.clear();
        dis.clear();
        prefered.clear();
        vis.clear();
    }
    cout << std::flush;
}

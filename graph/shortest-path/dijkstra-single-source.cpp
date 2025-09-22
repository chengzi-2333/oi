#include <bits/stdc++.h>

using namespace std;

constexpr int inf = 0x3f3f3f3f;
using PII = pair<int, int>;

int n, m, s;
vector<vector<PII>> g;
priority_queue<PII, vector<PII>, greater<PII>> q;
vector<int> d;
vector<bool> vis;

void dijkstra() {
    fill(d.begin()+1, d.end(), inf);
    d[s] = 0;
    q.push({0, s});
    while (!q.empty()) {
        int u = q.top().second;
        q.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto& [w, v]: g[u]) {
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
                q.push({d[v], v});
            }
        }
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("single-source.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m >> s; 
    g.resize(n+1); d.resize(n+1); vis.resize(n+1);
    for (int u, v, w; m; m--) {
        cin >> u >> v >> w;
        g[u].push_back({w, v});
    }
    dijkstra();
    for (auto it=d.begin()+1; it!=d.end(); it++) {
        if (*it == inf) cout << INT_MAX;
        else cout << *it;
        cout << ' ';
    }
    cout << endl;
}

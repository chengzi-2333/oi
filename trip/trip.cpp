#include <bits/stdc++.h>

using namespace std;

int n, m, ma, mi;
vector<set<int>> g;
vector<bool> vis;

void dfs(int u, int f) {
    cout << u << ' ';
    for (const auto& v: g[u]) {
        if (v != f) {
            dfs(v, u);
        }
    }
}

int find_circle(int u, int f) {
    vis[u] = true;
    for (const auto& v: g[u]) {
        if (v != f) {
            if (vis[v]) {
                ma = u;
                vis[0] = true;
                return v;
            }
            int ret = find_circle(v, u);
            if (ret) {
                vis[u] = vis[0];
                if (ret == u) {
                    vis[0] = false;
                    mi = v;
                }
                return ret;
            }
        }
    }
    vis[u] = vis[0];
    return 0;
}

void cut(int u, int f) {
    for (const auto& v: g[u]) {
        if (v == f || !vis[v]) continue;
        if (v >= ma) {
            g[u].erase(v), g[v].erase(u);
            return;
        }
        cut(v, u);
        break;
    }
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("trip2.in", "r", stdin);
#endif
    cin >> n >> m; 
    g.resize(n+1);
    for (int i=1, u, v; i<=m; i++) {
        cin >> u >> v;
        g[u].insert(v);
        g[v].insert(u);
    }
    if (m == n) {
        vis.resize(n+1);
        cut(find_circle(1, 0), 0);
    }
    dfs(1, 0);
    cout << endl;
}

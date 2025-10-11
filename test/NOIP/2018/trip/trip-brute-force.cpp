// {P5022}
#include <bits/stdc++.h>

using namespace std;

constexpr char lf = '\n';
constexpr char space = ' ';

int n, m;
vector<set<int>> g;
vector<int> path;

void dfs(int u, int f) {
    path.push_back(u);
    for (const auto& v: g[u]) {
        if (v != f) {
            dfs(v, u);
        }
    }
}

vector<vector<int>> cg;
vector<bool> vis;

// delete edge <s, t> & <t, s>
void dfs2(int u, int f, int s, int t) {
    path.push_back(u);
    for (const auto& v: g[u]) {
        if (u == s && v == t) continue;
        if (u == t && v == s) continue;
        if (v != f) dfs2(v, u, s, t);
    }
}

int find_circle(int u, int f) {
    vis[u] = true;
    int p = 0;
    for (const auto& v: g[u]) {
        if (v == f) continue;
        p = vis[v] ? v : find_circle(v, u);
        if (p) {
            cg[u].push_back(v);
            break;
        }
    }
    return p;
}


int main() {
#ifndef ONLINE_JUDGE
    freopen("trip2.in", "r", stdin);
#endif
    cin >> n >> m; 
    g.resize(n+1);
    path.reserve(n);
    for (int i=1, u, v; i<=m; i++) {
        cin >> u >> v;
        g[u].insert(v);
        g[v].insert(u);
    }
    if (m == n) {  // brute force
        vis.resize(n+1); cg.resize(n+1);
        vector<int> mpath(n, INT_MAX);
        find_circle(1, 0);
        for (int u=1; u<=n; u++) {
            for (const auto& v: cg[u]) {
                if (u == v) continue;
                dfs2(1, 0, u, v);
                if (path.size() == mpath.size())
                    mpath = min(mpath, path);
                path.clear(); path.reserve(n);
            }
        }
        path = mpath;
    } else dfs(1, 0);
    for (const auto& p: path)
        cout << p << space;
    cout << endl;
}

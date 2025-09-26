// naive
#include <bits/stdc++.h>

using namespace std;

int n, m, s;
vector<unordered_set<int>> g;
vector<int> fa, dep;

void dfs(int u, int f) {
    fa[u] = f;
    dep[u] = dep[f]+1;
    for (const auto& v: g[u])
        if (v != f) dfs(v, u);
}

int lca(int a, int b) {
    if (dep[a] < dep[b]) swap(a, b);
    while (dep[a] != dep[b]) a = fa[a];
    if (a == b) return a;
    while (fa[a] != fa[b]) a = fa[a], b = fa[b];
    return fa[a];
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("lca.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m >> s;
    g.resize(n+1);
    fa.resize(n+1), dep.resize(n+1);
    for (int u, v, i=1; i<n; i++) {
        cin >> u >> v;
        if (u == v) continue;
        g[u].insert(v);
        g[v].insert(u);
    }
    dfs(s, 0);
    for (int a, b; m; m--) {
        cin >> a >> b;
        cout << lca(a, b) << '\n';
    }
    cout << flush;
}

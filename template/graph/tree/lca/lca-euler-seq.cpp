// Euler sequence + RMQ (ST implemented)
#include <bits/stdc++.h>

using namespace std;

constexpr int P = 21;

int n, m, s, t;
vector<unordered_set<int>> g;
vector<array<int, P>> st;
vector<int> dep, lg;

void dfs(int u, int f) {
    if (!st[u][0]) st[u][0] = ++t;
    dep[u] = dep[f]+1;
    for (const auto& v: g[u])
        if (v != f) dfs(v, u);
}

void init_st() {
    lg.resize(n+1);
    for (int i=2; i<=n; i++) lg[i] = lg[i >> 1]+1;
    for (int i=1; i<=n; i++) {
        for (int j=1; j<=lg[n]; j++) {
            st[i][j] = min(st[i][j-1], st[][]);
        }
    }
}

int lca(int a, int b) {
    int l = min(st[a][0], st[b][0]), r = max(st[a][0], st[b][0]);
    int k = lg[r-l+1];
    return ;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("lca.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m >> s;
    g.resize(n+1);
    st.resize(n+1), dep.resize(n+1);
    for (int u, v, i=1; i<n; i++) {
        cin >> u >> v;
        if (u == v) continue;
        g[u].insert(v);
        g[v].insert(u);
    }
    dfs(s, 0);
    init_st();
    for (int a, b; m; m--) {
        cin >> a >> b;
        cout << lca(a, b) << '\n';
    }
    cout << flush;
}

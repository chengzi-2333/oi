// {P3388}
#include <bits/stdc++.h>

using namespace std;

constexpr char lf = '\n';

int n, m;
vector<vector<int>> g;

int t;
vector<int> dfn, low, pt;

void tarjan(int u, int f) {
    dfn[u] = low[u] = ++t;
    int child = 0;
    for (const auto& v: g[u]) {
        if (!dfn[v]) {
            child++;
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if (f != u && low[v] >= dfn[u]) pt.push_back(u);
        } else if (v != f) {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if (f == u && child >= 2) pt.push_back(u);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("cut.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m; g.resize(n+1);
    for (int u, v; m; m--) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfn.resize(n+1), low.resize(n+1);
    for (int u=1; u<=n; u++) {
        if (!dfn[u]) tarjan(u, u);
    }
    cout << pt.size() << lf;
    sort(pt.begin(), pt.end());
    for (const auto& p: pt) cout << p << ' ';
    cout << endl;
}

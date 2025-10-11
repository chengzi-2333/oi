// {<null>}
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m, ma, cnt, rt;
vector<vector<int>> g;

int t;
vector<int> dfn, low;

void tarjan(int u) {
    dfn[u] = low[u] = ++t;
    int child = 0;
    for (const auto& v: g[u]) {
        if (!dfn[v]) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) child++;
        } else low[u] = min(low[u], dfn[v]);
    }
    if (u != rt) child++;
    ma = max(ma, child);
}


signed main() {
#ifndef ONLINE_JUDGE
    freopen("block-count1.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m;
    while (n || m) {
        ma = cnt = t = 0;
        g.assign(n+1, vector<int>());
        for (int u, v; m; m--) {
            cin >> u >> v;
            u++, v++;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        dfn.assign(n+1, 0); low.assign(n+1, 0);
        for (rt=1; rt<=n; rt++) {
            if (!dfn[rt]) cnt++, tarjan(rt);
        }
        cout << cnt-1+ma << lf;
        cin >> n >> m;
    }
    cout << std::flush;
}

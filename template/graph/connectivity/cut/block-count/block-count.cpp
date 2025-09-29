#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m;
vector<vector<int>> g;

namespace BC {
    int t;
    vector<int> dfn, low, cut;

    void tarjan(int u, int f) {
        dfn[u] = low[u] = ++t;
        int child = 0;
        for (const auto& v: g[u]) {
            if (!dfn[v]) {
                child++;
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
                if (f != u && low[v] >= dfn[u]) cut.push_back(u);
            } else if (v != f) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (f == u && child >= 2) cut.push_back(u);
    }

    void init() {
        dfn.assign(n+1, 0);
        low.assign(n+1, 0);
    }

    void run() {
        for (int u=1; u<=n; u++) {
            if (!dfn[u]) tarjan(u, u);
        }
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("block-count1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m;
    while (n && m) {
        g.assign(n+1, vector<int>());
        for (int u, v; m; m--) {
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        BC::init();
        BC::run();
    }
}

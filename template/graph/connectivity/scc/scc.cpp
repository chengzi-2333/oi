#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<int> a;
vector<vector<int>> g;

namespace SCC {
    int t;
    vector<vector<int>> ng;
    vector<int> dfn, low, index, scc(1), din;
    vector<bool> vis;
    stack<int> st;

    inline void init() {
        dfn.resize(n+1);
        low.resize(n+1);
        vis.resize(n+1);
        index.resize(n+1);
    }

    inline void clean() {
        dfn.clear();
        low.clear();
        vis.clear();
        a.clear();
    }

    void tarjan(int u) {
        dfn[u] = low[u] = ++t;
        st.push(u); vis[u] = true;
        for (const auto& v: g[u]) {
            if (!dfn[v]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (vis[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            int top;
            scc.push_back(0);
            do {
                top = st.top();
                st.pop(); vis[top] = false;
                index[top] = scc.size()-1;
                scc.back() += a[top];
            } while (top != u);
        }
    }

    inline void shrink() {
        ng.resize(scc.size());
        din.resize(scc.size());
        for (int u=1; u<=n; u++) {
            for (const auto& v: g[u]) {
                int ui = index[u], vi = index[v];
                if (ui != vi) {
                    ng[ui].push_back(vi);
                    din[vi]++;
                }
            }
        }
        g.clear();
        index.clear();
    }

    inline void run() {
        for (int i=1; i<=n; i++) {
            if (!dfn[i]) tarjan(i);
        }
    }
}

using SCC::scc, SCC::ng, SCC::din;

int topo() {
    vector<int> f(scc.size());
    queue<int> q;
    for (int i=1; i<(int) scc.size(); i++) {
        if (!din[i]) {
            q.push(i); 
            f[i] = scc[i];
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto& v: ng[u]) {
            f[v] = max(f[v], f[u] + scc[v]);
            if (--din[v] == 0) q.push(v);
        }
    }
    return *max_element(f.begin(), f.end());
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("scc.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m; 
    a.resize(n+1); g.resize(n+1);
    for (auto it=a.begin()+1; it!=a.end(); it++) cin >> *it;
    for (int u, v; m; m--) {
        cin >> u >> v;
        g[u].push_back(v);
    }
    SCC::init();
    SCC::run();
    SCC::clean();
    SCC::shrink();
    cout << topo() << endl;
}

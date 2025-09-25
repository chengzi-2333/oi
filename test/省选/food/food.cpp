#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m;
vector<vector<int>> g, ng;

int t;
vector<int> dfn, low, cnt(1), index;
vector<bool> vis;
std::stack<int> st;

vector<int> din;

void tarjan(int u) {
    dfn[u] = low[u] = ++t;
    st.push(u), vis[u] = true;
    for (const auto& v: g[u]) {
        if (!dfn[v]) {
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (vis[v]) {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if (dfn[u] == low[u]) {
        cnt.push_back(0);
        int tp = st.top();
        do {
            tp = st.top();
            st.pop(), vis[tp] = false;
            cnt.back()++;
            index[tp] = cnt.size()-1;
        } while (tp != u);
    }
}

inline void shrink() {
    for (int u=1; u<=n; u++) {
        for (const auto& v: g[u]) {
            int ui = index[u], vi = index[v];
            if (ui != vi) {
                ng[ui].push_back(vi);
                din[vi]++;
            }
        }
    }
}

int topo() {
    vector<int> d(cnt.size());
    std::queue<int> q;
    for (int i=1; i<(int) cnt.size(); i++) {
        if (!din[i]) {
            q.push(i);
            d[i] = cnt[i];
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto& v: ng[u]) {
            d[v] = max(d[v], d[u] + cnt[v]);
            if (--din[v] == 0) q.push(v);
        }
    }
    return *std::max_element(d.begin(), d.end());
}

int bridge() {
    int ans = cnt[index[1]];
    for (int u=1; u<(int) ng.size(); u++) {
        for (const auto& v: ng[u]) {
            // if (vis[1][u] && vis[0][v]) {
            //     ans = max(ans, f[0][]+f[1][]-cnt[index[1]]);
            // }
        }
    }
    return ans;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("food2.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m;
    g.resize(n+1);
    for (int u, v; m; m--) {
        cin >> u >> v;
        g[u].push_back(v);
    }
    // tarjan shrink node
    dfn.resize(n+1); low.resize(n+1);
    index.resize(n+1); vis.resize(n+1);
    for (int i=1; i<=n; i++) {
        if (!dfn[i]) tarjan(i);
    }
    dfn.clear(), low.clear(), vis.clear();
    ng.resize(cnt.size());
    din.resize(cnt.size());
    shrink(); g.clear();

    // reset();
    // topo();
    // shrink();

    // reset();
    // topo();
    // shrink();

    cout << bridge() << endl;  // TODO
}

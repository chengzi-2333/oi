#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m;
vector<vector<int>> g, ng;
vector<int> cnt, idx, din, d[2];
vector<bool> vis[2];

namespace Tarjan {
    int t;
    vector<int> dfn, low;
    vector<bool> in_stack;
    std::stack<int> st;

    void tarjan(int u) {
        dfn[u] = low[u] = ++t;
        st.push(u), in_stack[u] = true;
        for (const auto& v: g[u]) {
            if (!dfn[v]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (in_stack[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            cnt.push_back(0);
            int tp = st.top();
            do {
                tp = st.top();
                st.pop(), in_stack[tp] = false;
                cnt.back()++;
                idx[tp] = cnt.size()-1;
            } while (tp != u);
        }
    }
    
    inline void init() {
        dfn.resize(n+1);
        low.resize(n+1);
        in_stack.resize(n+1);
    }
    
    inline void run() {
        for (int i=1; i<=n; i++) {
            if (!dfn[i]) tarjan(i);
        }
    }


}

inline void shrink(bool flag = false) {
    din.assign(cnt.size(), 0);
    ng.assign(cnt.size(), vector<int>());
    for (int u=1; u<=n; u++) {
        for (const auto& v: g[u]) {
            int ui = idx[u], vi = idx[v];
            if (ui != vi) {
                if (flag) std::swap(ui, vi);
                ng[ui].push_back(vi);
                din[vi]++;
            }
        }
    }
}

void topo(vector<int>& dis, vector<bool>& vi) {
    dis.resize(cnt.size()); vi.resize(cnt.size());
    vi[idx[1]] = true; dis[idx[1]] = cnt[idx[1]];
    std::queue<int> q;
    for (int i=0; i<(int) cnt.size(); i++) {
        if (!din[i]) {
            q.push(i);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto& v: ng[u]) {
            if (vi[u]) {
                vi[v] = true;
                dis[v] = max(dis[v], dis[u]+cnt[v]);
            }
            if (--din[v] == 0) q.push(v);
        }
    }
}

int bridge() {
    int ans = d[0][idx[1]];
    for (int u=0; u<(int) ng.size(); u++) {
        for (const auto& v: ng[u]) {
            if (vis[0][u] && vis[1][v]) {
                ans = max(ans, d[1][v]+d[0][u]-cnt[idx[1]]);
            }
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
    idx.resize(n+1);

    Tarjan::init();
    Tarjan::run();
    
    shrink(false), topo(d[0], vis[0]);
    shrink(true), topo(d[1], vis[1]);
    
    cout << bridge() << endl;
}

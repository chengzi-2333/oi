#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m;
vector<vector<int>> g, ng;
vector<int> cnt(1), idx, din, d[2];
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
    
    inline void clean() {
        dfn.clear();
        low.clear();
        in_stack.clear();
    }

    inline void shrink(bool flag = false) {
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
}

void topo(vector<int>& dis, vector<bool>& vi) {
    dis.resize(cnt.size()); vi.resize(cnt.size());
    std::queue<int> q;
    for (int i=1; i<(int) cnt.size(); i++) {
        if (!din[i]) {
            q.push(i);
            dis[i] = cnt[i];
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (vi[u]) continue;
        vi[u] = true;
        for (const auto& v: ng[u]) {
            dis[v] = max(dis[v], dis[u] + cnt[v]);
            if (--din[v] == 0) q.push(v);
        }
    }
}

int bridge() {
    int ans = cnt[idx[1]];
    for (int u=1; u<(int) ng.size(); u++) {
        for (const auto& v: ng[u]) {
            if (vis[1][u] && vis[0][v]) {
                ans = max(ans, d[0][v]+d[1][u]-cnt[idx[1]]);
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
    Tarjan::clean();

    d[0].resize(cnt.size()), d[1].resize(cnt.size());

    din.clear(), din.resize(cnt.size());
    ng.clear(), ng.resize(cnt.size());
    Tarjan::shrink();
    topo(d[0], vis[0]);

    din.clear(), din.resize(cnt.size());
    ng.clear(), ng.resize(cnt.size());
    Tarjan::shrink(true);
    topo(d[1], vis[1]);

    cout << bridge() << endl;
}

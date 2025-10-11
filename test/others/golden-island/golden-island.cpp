// {<null>}
#include <bits/stdc++.h>

using namespace std;

int n, m;
vector<vector<int>> g, ng;
vector<int> a;

namespace Tarjan {
    int t;
    vector<int> dfn, low, cnt(1), idx, din;
    vector<bool> in_st;
    stack<int> st;

    void init() {
        dfn.resize(n+1);
        low.resize(n+1);
        in_st.resize(n+1);
        idx.resize(n+1);
    }

    void tarjan(int u) {
        dfn[u] = low[u] = ++t;
        st.push(u), in_st[u] = true;
        for (const auto& v: g[u]) {
            if (!dfn[v]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (in_st[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            cnt.push_back(0);
            int tp = st.top();
            do {
                tp = st.top();
                st.pop(), in_st[tp] = false;
                cnt.back() += a[tp];
                idx[tp] = cnt.size()-1;
            } while (tp != u);
        }
    }

    void run() {
        for (int i=1; i<=n; i++) {
            if (!dfn[i]) tarjan(i);
        }
    }

    void shrink() {
        ng.resize(cnt.size());
        din.resize(cnt.size());
        for (int u=1; u<=n; u++) {
            for (const auto& v: g[u]) {
                int ui = idx[u], vi = idx[v];
                if (ui != vi) {
                    ng[vi].push_back(ui);
                    din[ui]++;
                }
            }
        }
    }
}

using Tarjan::cnt;
using Tarjan::din;
using Tarjan::idx;

vector<int> topo() {
    queue<int> q;
    vector<int> d(cnt.size());
    for (int u=1; u<(int)d.size(); u++) {
        if (!din[u]) {
            q.push(u);
            d[u] = cnt[u];
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto& v: ng[u]) {
            d[v] = max(d[v], d[u]+cnt[v]);
            if (--din[v] == 0) q.push(v);
        }
    }
    return d;
}


int main() {
#ifndef ONLINE_JUDGE
    freopen("golden-island1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n;
    a.resize(n+1); g.resize(n+1);
    for (int i=1; i<=n; i++) cin >> a[i];
    for (int u=1, v; u<=n; u++) {
        cin >> v;
        if (u != v) g[u].push_back(v);
    }
    Tarjan::init();
    Tarjan::run();
    Tarjan::shrink();
    auto a = topo();
    for (int i=1; i<=n; i++) cout << a[idx[i]] << '\n';
    cout << flush;
}

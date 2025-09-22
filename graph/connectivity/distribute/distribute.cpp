#include <bits/stdc++.h>

using namespace std;

int n;
vector<vector<int>> g;

int t, cnt;
vector<int> dfn, low, idx;
vector<bool> vis;
stack<int> st;

vector<int> din, dout;

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
        cnt++;
        int top = st.top();
        do {
            top = st.top();
            st.pop(), vis[top] = false;
            idx[top] = cnt;
        } while (top != u);
    }
}

void shrink() {
    for (int u=1; u<=n; u++) {
        for (const auto& v: g[u]) {
            int ui = idx[u], vi = idx[v];
            if (ui != vi) din[vi]++, dout[ui]++;
        }
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("distribute.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n; g.resize(n+1);
    for (int u=1, v; u<=n; u++) {
        cin >> v;
        while (v) {
            g[u].push_back(v);
            cin >> v;
        }
    }
    dfn.resize(n+1), low.resize(n+1);
    vis.resize(n+1), idx.resize(n+1);
    for (int u=1; u<=n; u++) {
        if (!dfn[u]) tarjan(u);
    }
    din.resize(cnt+1), dout.resize(cnt+1);
    shrink();
    if (cnt == 1) cout << 1 << endl << 0 << endl;
    else {
        int din_cnt = count(din.begin()+1, din.end(), 0);
        int dout_cnt = count(dout.begin()+1, dout.end(), 0);
        cout << din_cnt << endl;
        cout << max(din_cnt, dout_cnt) << endl;
    }
}

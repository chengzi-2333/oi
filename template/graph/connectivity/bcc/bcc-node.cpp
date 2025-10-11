// {P8435}
#include <bits/stdc++.h>

int n, m;
std::vector<std::vector<int>> g;

int t;
std::vector<int> dfn, low;
std::stack<int, std::vector<int>> st;
std::vector<std::vector<int>> bcc;

void tarjan(int u, int f) {
    dfn[u] = low[u] = ++t;
    st.push(u);
    int child = 0;
    for (const auto& v: g[u]) {
        if (!dfn[v]) {
            child++;
            tarjan(v, u);
            low[u] = std::min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                bcc.emplace_back();
                bcc.back().push_back(u);
                while (bcc.back().back() != v) 
                    bcc.back().push_back(st.top()), st.pop();
            }
        } else if (v != f) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }
    if (f == 0 && child == 0) 
        bcc.emplace_back(), bcc.back().push_back(u);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("bcc-node1.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize(n+1);
    for (int u, v; m; m--) {
        std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfn.resize(n+1);
    low.resize(n+1);
    for (int i=1; i<=n; i++) {
        t = 0;
        if (!dfn[i]) tarjan(i, 0);
    }
    std::cout << bcc.size() << '\n';
    for (const auto& cc: bcc) {
        std::cout << cc.size() << ' ';
        for (const auto& node: cc) {
            std::cout << node << ' ';
        }
        std::cout << '\n';
    }
}

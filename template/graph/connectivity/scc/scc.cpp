// {P3387}
#include <bits/stdc++.h>

int n, m;
std::vector<int> a(1);
std::vector<std::vector<int>> g;

namespace SCC {
int t;
std::vector<std::vector<int>> ng;
std::vector<int> dfn, low, index, scc(1), din;
std::vector<bool> vis;
std::stack<int> st;

inline void init() {
    dfn.resize(n + 1);
    low.resize(n + 1);
    vis.resize(n + 1);
    index.resize(n + 1);
}

inline void clean() {
    dfn.clear();
    low.clear();
    vis.clear();
    a.clear();
}

void tarjan(int u) {
    dfn[u] = low[u] = ++t;
    st.push(u);
    vis[u] = true;
    for (const auto& v : g[u]) {
        if (!dfn[v]) {
            tarjan(v);
            low[u] = std::min(low[u], low[v]);
        } else if (vis[v]) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }
    if (dfn[u] == low[u]) {
        int top;
        scc.push_back(0);
        do {
            top = st.top();
            st.pop();
            vis[top] = false;
            index[top] = scc.size() - 1;
            scc.back() += a[top];
        } while (top != u);
    }
}

inline void shrink() {
    ng.resize(scc.size());
    din.resize(scc.size());
    for (int u = 1; u <= n; u++) {
        for (const auto& v : g[u]) {
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
    for (int i = 1; i <= n; i++) {
        if (!dfn[i]) tarjan(i);
    }
}
}  // namespace SCC

using SCC::scc, SCC::ng, SCC::din;

int topo() {
    std::vector<int> f(scc.size());
    std::queue<int> q;
    for (int i = 1; i < (int)scc.size(); i++) {
        if (!din[i]) {
            q.push(i);
            f[i] = scc[i];
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto& v : ng[u]) {
            f[v] = std::max(f[v], f[u] + scc[v]);
            if (--din[v] == 0) q.push(v);
        }
    }
    return *std::max_element(f.begin(), f.end());
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("scc.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    g.resize(n + 1);
    std::copy_n(std::istream_iterator<int>(std::cin), n,
                std::back_insert_iterator<std::vector<int>>(a));
    for (int u, v; m; m--) {
        std::cin >> u >> v;
        g[u].push_back(v);
    }
    SCC::init();
    SCC::run();
    SCC::clean();
    SCC::shrink();
    std::cout << topo() << std::endl;
}

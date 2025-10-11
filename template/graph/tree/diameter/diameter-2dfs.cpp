// {B4016}
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n;
vector<vector<int>> g;

pair<int, int> dfs(int u, int f, int dep) {
    int mx_dep = dep, mx_vertex = u;
    for (const auto& v: g[u]) {
        if (v == f) continue;
        auto [d, p] = dfs(v, u, dep+1);
        if (d > mx_dep) {
            mx_dep = d;
            mx_vertex = p;
        }
    }
    return {mx_dep, mx_vertex};
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("diameter.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n; g.resize(n+1);
    for (int u, v, i=1; i<n; i++) {
        cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    cout << dfs(dfs(1, 0, 0).second, 0, 0).first << endl;
}

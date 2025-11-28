// {P7771}
#include <bits/stdc++.h>

int n, m, s = 1, cs, ct;
std::vector<std::priority_queue<int, std::vector<int>, std::greater<int>>> g;
std::vector<int> din, dout, path;

void dfs(int u) {
    for (int v; !g[u].empty();) {
        v = g[u].top();
        g[u].pop();
        dfs(v);
    }
    path.push_back(u);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("euler-path1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    din.resize(n + 1), dout.resize(n + 1);
    g.resize(n + 1);
    for (int u, v; m; m--) {
        std::cin >> u >> v;
        g[u].push(v);
        din[v]++, dout[u]++;
    }
    for (int i = 1; i <= n; i++) {
        if (std::abs(din[i] - dout[i]) > 1) {
            std::cout << "No" << '\n';
            return 0;
        }
        if (dout[i] - din[i] == 1) {
            cs++;
            s = i;
        } else if (din[i] - dout[i] == 1) {
            ct++;
        }
    }
    if ((cs != ct) || (cs != 0 && cs != 1)) {
        std::cout << "No" << '\n';
        return 0;
    }
    dfs(s);
    std::copy(path.rbegin(), path.rend(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}

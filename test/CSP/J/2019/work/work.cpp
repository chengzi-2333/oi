// {P5663}
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

constexpr int INF = 0x3f3f3f3f;
using PII = pair<int, int>;

int n, m, q;
vector<vector<int>> g;
vector<int> d[2];

bool bruteforce(int wk, int lv) {
    std::queue<pair<int, int>> que;
    que.emplace(lv, wk);
    while (!que.empty()) {
        const auto& [l, u] = que.front();
        que.pop();
        if (u == 1 && l == 0) return true;
        for (const auto& v: g[u]) {
            if (l) que.emplace(l-1, v);
        }
    }
    return false;
}

void bfs(int s) {
    std::queue<int> que;
    que.push(s);
    d[0][s] = 0;
    while (!que.empty()) {
        int u = que.front();
        que.pop();
        for (const auto& v: g[u]) {
            if (d[1][v] > d[0][u] + 1) {
                d[1][v] = d[0][u] + 1;
                que.push(v);
            }
            if (d[0][v] > d[1][u] + 1) {
                d[0][v] = d[1][u] + 1;
                que.push(v);
            }
        }
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("work1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m >> q;
    g.resize(n+1);
    d[0].resize(n+1, INF), d[1].resize(n+1, INF);
    for (int u, v; m; m--) {
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    bfs(1);
    for (int w, l; q; q--) {
        cin >> w >> l;
        std::puts(d[l&1][w] <= l ? "Yes" : "No");
    }
}

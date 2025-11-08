// {P14362}
#include <bits/stdc++.h>

int n, m, k;
std::vector<std::tuple<int, int, int>> edges;
std::vector<std::vector<int>> bridges;

struct DSU {
    std::vector<int> fa, rank;
    
    DSU(int sz) {
        fa.resize(sz + 1);
        rank.resize(sz + 1);
    }

    int root(int x) {
        return fa[x] ? fa[x] = root(fa[x]) : x;
    }

    inline void insert(int x, int y) {
        x = root(x), y = root(y);
        if (x == y) return;
        if (rank[x] > rank[y]) std::swap(x, y);
        if (rank[x] == rank[y]) rank[y] = rank[x] + 1;
        fa[x] = y;
    }

    inline bool related(int x, int y) {
        return root(x) == root(y);
    }
};

int kruskal() {
    int ans = 0;
    DSU dsu(n);
    std::sort(edges.begin(), edges.end());
    for (const auto& [w, u, v] : edges) {
        if (dsu.related(u, v)) continue;
        dsu.insert(u, v);
        ans += w;
    }
    return ans;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("road.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> k;
    edges.resize(m);
    for (auto& [w, u, v] : edges) std::cin >> u >> v >> w;
    // TODO: input
    for (int state = 0; state < (1 << k); state++) {
        // TODO: enumerate
    }
}

// {P3366}
#include <bits/stdc++.h>

struct DSU {
    std::vector<int> uni, rank;

    DSU(int n) {
        uni.resize(n + 1);
        rank.resize(n + 1);
    }

    int root(int x) { return uni[x] ? uni[x] = root(uni[x]) : x; }

    void insert(int x, int y) {
        x = root(x), y = root(y);
        if (x == y) return;
        if (rank[y] < rank[x]) std::swap(x, y);
        if (rank[x] == rank[y]) rank[y] = rank[x] + 1;
        uni[x] = y;
    }

    bool related(int x, int y) { return root(x) == root(y); }
};

int n, m;
std::vector<std::tuple<int, int, int>> edges;

inline int kruskal() {
    DSU dsu(n);
    int tot = 0, ans = 0;
    std::sort(edges.begin(), edges.end());
    for (const auto& [w, u, v] : edges) {
        if (dsu.related(u, v)) continue;
        dsu.insert(u, v);
        ans += w;
        tot++;
        if (tot == n - 1) break;
    }
    return (tot == n - 1) ? ans : -1;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("mst.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    edges.resize(m);
    for (auto& [w, u, v] : edges) std::cin >> u >> v >> w;
    int ans = kruskal();
    if (ans == -1)
        std::cout << "orz";
    else
        std::cout << ans;
    std::cout << std::endl;
}

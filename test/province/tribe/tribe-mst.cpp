#include <bits/stdc++.h>

using PII = std::tuple<int, int>;
using TRI = std::tuple<int, int, int>;


inline int dis2(int dx, int dy) {
    return dx*dx+dy*dy;
}

inline int dis2(int xa, int ya, int xb, int yb) {
    return dis2(xb-xa, yb-ya);
}

inline int dis2(PII a, PII b) {
    auto [xa, ya] = a;
    auto [xb, yb] = b;
    return dis2(xa, ya, xb, yb);
}

struct DSU {
    std::vector<int> uni;

    DSU(int n) {
        uni.resize(n+1);
        for (int i=1; i<=n; i++) uni[i] = i;
    }

    int root(int x) {
        return uni[x]==x ? x : uni[x]=root(uni[x]);
    }

    void insert(int x, int y) {
        uni[root(x)] = root(y);
    }

    bool related(int x, int y) {
        return root(x) == root(y);
    }
};

int n, k;
std::vector<PII> p;
std::vector<TRI> e;

int kruskal() {
    int c = 0;
    DSU dsu(n);
    for (const auto& [w, u, v]: e) {
        if (dsu.related(u, v)) continue;
        dsu.insert(u, v);
        if (c++ >= n-k) return w;
    }
    return -1;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("tribe2.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> k;
    p.resize(n);
    for (auto& [x, y]: p) std::cin >> x >> y;
    for (int i=0; i<n; i++) {
        for (int j=i+1; j<n; j++) {
            e.emplace_back(dis2(p[i], p[j]), i, j);
        }
    }
    std::sort(e.begin(), e.end());
    printf("%.2f", std::sqrt(kruskal()));
}

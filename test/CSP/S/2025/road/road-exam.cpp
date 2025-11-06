#include <bits/stdc++.h>

#define int long long

using Tri = std::tuple<int, int, int>;

struct DSU {
    std::vector<int> fa;

    DSU(int n) {
        fa.resize(n + 1);
    }

    int root(int x) {
        return fa[x] ? fa[x] = root(fa[x]) : x;
    }

    void insert(int x, int y) {
        fa[root(x)] = root(y);
    }

    bool related(int x, int y) {
        return root(x) == root(y);
    }
};

bool flag = true;
int n, m, k;
std::vector<Tri> edges;
std::vector<int> city[11];

int kruskal() {
    int ans = 0;
    std::sort(edges.begin(), edges.end());
    DSU dsu(n);
    for (const auto& e : edges) {
        auto u = std::get<1>(e), v = std::get<2>(e), w = std::get<0>(e);
        if (dsu.related(u, v)) continue;
        dsu.insert(u, v);
        ans += w;
    }
    return ans;
}

signed main() {
    freopen("road.in", "r", stdin);
    freopen("road.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> k;
    edges.resize(m);
    for (auto& e: edges) std::cin >> std::get<1>(e) >> std::get<2>(e) >> std::get<0>(e);
    if (k) {
        for (int i = 1; i <= k; i++) {
            city[i].resize(n + 1);
            for (int j = 0; j <= n; j++) {
                std::cin >> city[i][j];
                // if (j) edges.emplace_back(city[i][j] + city[i][0] / n, i, j);
                if (city[i][j]) flag = false;
            }
        }
        if (flag) std::cout << 0;
        else std::cout << kruskal();
    } else std::cout << kruskal();
    std::cout << std::endl;
    return 0;
}

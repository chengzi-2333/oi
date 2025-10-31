// {P2279}
#include <bits/stdc++.h>

int n, ans;
std::vector<int> fa, dep, ord, dis;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("fire-department.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    
    std::cin >> n;
    
    fa.resize(n + 1), dep.resize(n + 1);
    for (int i = 2; i <= n; i++) {
        std::cin >> fa[i];
        dep[i] = dep[fa[i]] + 1;
    }

    ord.resize(n);
    std::iota(ord.begin(), ord.end(), 1);
    std::sort(ord.begin(), ord.end(), [](int a, int b) {
        return dep[a] > dep[b];
    });

    dis.resize(n + 1, 0x3f3f3f3f);
    for (const auto& u : ord) {
        auto w = fa[u], v = fa[fa[u]];
        dis[u] = std::min(dis[u], std::min(dis[w] + 1, dis[v] + 2));
        if (dis[u] > 2) {
            dis[v] = 0;
            dis[fa[v]] = std::min(dis[fa[v]], 1);
            dis[fa[fa[v]]] = std::min(dis[fa[fa[v]]], 2);
            ans++;
        }
    }

    std::cout << ans << std::endl;
}

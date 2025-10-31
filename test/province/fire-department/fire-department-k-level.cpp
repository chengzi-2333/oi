// {P2279}
// tree cover problem
#include <bits/stdc++.h>

int n, k = 2, ans;
std::vector<int> fa, dep, ord, dis;

inline int k_level_fa(int p, int k) {
    while (k--) p = fa[p];
    return p;
}

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
        auto v = k_level_fa(u, k);
        for (int i = 0; i <= k; i++) 
            dis[u] = std::min(dis[u], dis[k_level_fa(u, i)] + i);
        if (dis[u] > k) {
            for (int i = 0; i <= k; i++) 
                dis[k_level_fa(v, i)] = std::min(dis[k_level_fa(v, i)], i);
            ans++;
        }
    }

    std::cout << ans << std::endl;
}

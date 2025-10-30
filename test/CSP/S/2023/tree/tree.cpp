// {P9755}
#include <bits/stdc++.h>

using i128 = __int128;

struct Node {
    long long a, b, c;

    i128 days() const {
        if (b == 0) return -1;
        return a / b + !!(a % b);
    }

    bool operator<(const Node& node) { return this->days() < node.days(); }
};
int n;
std::vector<int> fa;
std::vector<Node> nodes;
std::vector<std::vector<int>> g;

template <typename T, typename C>
inline T bisect(T l, T r, C chk) {
    while (l <= r) {
        auto mid = l + ((r - l) >> 1);
        if (chk(mid))
            l = mid + 1;
        else
            r = mid - 1;
    }
    return l;
}

void dfs(int u, int f) {
    fa[u] = f;
    for (const auto& v : g[u]) {
        if (v != f) dfs(v, u);
    }
}

inline i128 calc(int p, i128 l, i128 r) {
    const auto& [a, b, c] = nodes[p];
    if (c >= 0) return (r - l + 1) * ((l + r) * c / 2 + b);
    auto mid = static_cast<i128>(1 - b) / c;
    if (mid < l) return r - l + 1;
    if (mid > r) return (r - l + 1) * ((l + r) * c / 2 + b);
    return (mid - r + 1) * ((l + mid) * c / 2 + b) + r - mid;
}

bool check(int ans) {  // TODO
    std::vector<int> p(n + 1), tx(n + 1), st(n + 1);
    std::vector<bool> vis(n + 1);
    std::iota(p.begin(), p.end(), 0);
    for (int i = 1; i <= n; i++) {
        auto a = nodes[i].a;
        if (calc(i, 1, ans) < a) return false;
        tx[i] = bisect(1, n, [&i, &a](i128 mid) { return calc(i, i, mid) >= a; });
    }
    std::sort(p.begin(), p.end(), [&tx](int a, int b) { return tx[a] < tx[b]; });
    for (int i = 1, x = 0; i <= n; i++) {
        int now = p[i], top = 0;
        while (!vis[now]) vis[st[++top] = now] = true, now = fa[now];
        while (top) if (tx[st[top--]] < ++x) return false;
    }
    return true;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("tree1.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    nodes.resize(n + 1), g.resize(n + 1);
    for (int i = 1; i <= n; i++) {
        auto& [a, b, c] = nodes[i];
        std::cin >> a >> b >> c;
    }
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    fa.resize(n + 1), dfs(1, 0);
    g.clear();
    std::cout << bisect(n, (int) 1e9, check) << std::endl;
}

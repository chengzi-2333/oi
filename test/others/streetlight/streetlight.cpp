// {P1220}
#include <bits/stdc++.h>

int n, c;
std::vector<int> p, w;

std::pair<int, int> dfs(int pos, int pre, int dep) {  // TODO
    if (pos < 1 || pos > n) return {INT_MIN, 0};
    auto ds = std::abs(p[pos] - p[pre]);
    if (dep == n) return {w[pos] * ds, -ds};
    // if (pos == pre) return dfs(pos + pos - pre, pre, dep);
    std::pair<int, int> left(INT_MIN, 0), right(INT_MIN, 0);
    if (pos - 1 != pre) left = dfs(pos - 1, pos, dep + 1);
    if (pos + 1 != pre) right = dfs(pos + 1, pos, dep + 1);
    auto [e, s] = std::max(left, right);
    return {e + w[pos] * ds, s + ds};
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("streetlight.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> c;
    p.resize(n + 1), w.resize(n + 1);
    for (int i = 1; i <= n; i++) {
        std::cin >> p[i] >> w[i];
    }
    // std::partial_sum(w.begin(), w.end(), w.begin());
    auto [saved_energy, displacement] = dfs(c, c, 1);
    std::cout << std::accumulate(w.begin(), w.end(), -w[c]) *
                         std::abs(displacement) -
                     saved_energy
              << std::endl;
    std::cout << displacement << std::endl;
}

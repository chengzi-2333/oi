#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

using Edge = tuple<int, int, int>;

struct ExDSU {
    int n;
    vector<int> u;

    ExDSU(int _n): n(_n) {
        u.resize(n*2+2);
        for (int i=1; i<=n*2; i++) u[i] = i;
    }

    int root(int x) {
        return u[x] == x ? x : u[x] = root(u[x]);
    }

    void insert(int x, int y) {
        u[root(x)] = root(y);
    }

    bool is_related(int x, int y) {
        return root(x) == root(y);
    }

    void insert_rev(int x, int y) {
        insert(x, y+n), insert(y, x+n);
    }

    bool is_related_rev(int x, int y) {
        return is_related(x, x+n) || is_related(y, y+n);
    }
};

int n, m, ans;
vector<Edge> edges;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("crime.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m; edges.resize(m);
    for (auto& [w, u, v]: edges) cin >> u >> v >> w;
    std::sort(edges.begin(), edges.end(), std::greater<Edge>());
    ExDSU dsu(n);
    for (auto& [w, u, v]: edges) {
        dsu.insert_rev(u, v);
        if (dsu.is_related_rev(u, v)) {
            ans = w;
            break;
        }
    }
    cout << ans << endl;
}

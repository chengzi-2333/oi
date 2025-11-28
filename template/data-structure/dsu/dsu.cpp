// {P3367}
#include <bits/stdc++.h>

// #define DSU_RECURSIVE

class DSU {
   private:
    std::vector<int> uni, rank;

   public:
    DSU(int n) {
        uni.resize(n + 1);
        rank.resize(n + 1);
        // std::iota(uni.begin(), uni.end(), 0);
    }

#ifdef DSU_RECURSIVE
    // recursive
    int root(int x) {
        // return (uni[x] == x) ? x : uni[x] = root(uni[x]);
        return uni[x] ? uni[x] = root(uni[x]) : x;
    }
#else
    // loop-based
    int root(int x) {
        int t = x, nt = t;
        while (uni[x]) x = uni[x];
        while (uni[t]) nt = uni[t], uni[t] = x, t = nt;
        return x;
    }
#endif

    void insert(int x, int y) {
        x = root(x), y = root(y);
        if (x == y) return;
        if (rank[y] < rank[x]) std::swap(x, y);
        if (rank[x] == rank[y]) rank[y] = rank[x] + 1;
        uni[x] = y;
    }

    bool related(int x, int y) { return root(x) == root(y); }

    int count() { return std::count(uni.begin() + 1, uni.end(), 0); }
};

int n, m;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("dsu.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    DSU dsu(n);
    for (int op, x, y; m; m--) {
        std::cin >> op >> x >> y;
        if (--op)
            std::cout << (dsu.related(x, y) ? 'Y' : 'N') << '\n';
        else
            dsu.insert(x, y);
    }
}

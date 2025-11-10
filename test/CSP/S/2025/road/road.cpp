// {P14362}
#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define int long long
#define MAX LONG_LONG_MAX

char* rp;

template <typename T>
inline void fast_read(T& x) {
    char c;
    x = 0;
    while (!isdigit(c = *rp++));
    do x = x * 10 + (c - '0');
    while (isdigit(c = *rp++));
}

template <typename T, typename... A>
inline void fast_read(T& x, A&... args) {
    fast_read(x), fast_read(args...);
}

constexpr int N = 1e4;

int n, m, k;
std::vector<std::tuple<int, int, int>> edges;
std::vector<std::vector<int>> countries;

namespace DSU {
int fa[N + 15], rank[N + 15];

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

inline void clear() {
    memset(fa, 0, sizeof(fa));
    memset(rank, 0, sizeof(rank));
}
}

inline void pick_edge() {
    DSU::clear();
    std::sort(edges.begin(), edges.end());
    auto it = edges.begin();
    for (const auto& [w, u, v] : edges) {
        if (DSU::related(u, v)) continue;
        *it++ = {w, u, v};
        DSU::insert(u, v);
        // if (std::distance(edges.begin(), it) == n - 1) break;
    }
    edges.erase(it, edges.end());
}

inline int kruskal(unsigned int state) {
    DSU::clear();
    int tot = n + std::popcount(state) - 1;
    int ans = 0;
    for (const auto& [w, u, v] : edges) {
        if (DSU::related(u, v)) continue;
        if (u <= n || (state & (1 << (u - n - 1)))) {
            DSU::insert(u, v);
            ans += w;
            if (--tot == 0) break;
        }
    }
    return ans;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("road4.in", "r", stdin);
#endif  // ONLINE_JUDGE
    struct stat st;
    fstat(0, &st);
    rp = (char*) mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);

    fast_read(n, m, k);
    edges.resize(m);
    for (auto& [w, u, v] : edges) fast_read(u, v, w);
    countries.resize(k);
    for (auto& country : countries) {
        country.resize(n + 1);
        for (auto& c : country) {
            fast_read(c);
        }
    }

    pick_edge();
    for (int i = 0; i < k; i++) {
        for (int v = 1; v <= n; v++) {
            edges.emplace_back(countries[i][v], i + n + 1, v);
        }
    }
    std::sort(edges.begin(), edges.end());
    int ans = MAX;
    for (int w = 0, state = 0; state < (1 << k); state++, w = 0) {
        for (int i = 0; i < k; i++) {
            if (state & (1 << i)) {
                w += countries[i][0];
            }
        }
        ans = std::min(ans, kruskal(state) + w);
    }
    std::cout << ans << std::endl;
}

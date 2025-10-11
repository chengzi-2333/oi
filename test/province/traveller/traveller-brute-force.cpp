// {P5304}
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

char* rp;

inline void fast_read(int& x) {
	char c;
	x = 0;
	while (!isdigit(c = *rp++));
	do x = x*10 + (c - '0');
	while (isdigit(c = *rp++));
}

template<typename T, typename... Args>
inline void fast_read(T& first, Args&... args) {
	fast_read(first); fast_read(args...);
}

inline void mmap_init() {
    struct stat st;
    fstat(0, &st);
    rp = (char*) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
}

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

constexpr long long INF = 0x3f3f3f3f3f3f3f3f;

using PLI = pair<long long, int>;

int n, m, k;
vector<vector<PLI>> g;
vector<long long> dis;
vector<bool> prefered, vis;
std::priority_queue<PLI, vector<PLI>, std::greater<PLI>> q;


inline long long dijkstra(int s) {
    while (!q.empty()) q.pop();
    std::fill(dis.begin(), dis.end(), INF);
    std::fill(vis.begin(), vis.end(), false);
    dis[s] = 0;
    q.emplace(dis[s], s);
    while (!q.empty()) {
        int u = q.top().second;
        q.pop();
        // return after find the second "prefered" node (just a single line!)
        if (prefered[u] && u != s) return dis[u];
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto& [w, v]: g[u]) {
            if (dis[u] + w < dis[v]) {
                dis[v] = dis[u] + w;
                q.emplace(dis[v], v);
            }
        }
    }
    return INF;
}

// brute force (optimized)
long long solve() {
    long long ans = INF;
    for (int s=1; s<=n; s++)
        if (prefered[s]) 
            ans = min(ans, dijkstra(s));
    return ans;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("traveller.in", "r", stdin);
#endif
    mmap_init();
    int t;
    fast_read(t);
    while (t--) {
        fast_read(n, m, k);
        g.resize(n+1); 
        dis.resize(n+1);
        prefered.resize(n+1);
        vis.resize(n+1);
        for (int u, v, w; m; m--) {
            fast_read(u, v, w);
            if (u == v) continue;
            g[u].emplace_back(w, v);
        }
        for (int u; k; k--) {
            fast_read(u);
            prefered[u] = true;
        }
        cout << solve() << lf;
        g.clear();
        dis.clear();
        prefered.clear();
        vis.clear();
    }
    cout << std::flush;
}

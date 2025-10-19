// {P3366}
#include <ctype.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <vector>
#include <tuple>
#include <algorithm>


char* rp;

inline void fast_read(int& x) {
	char c;
	x = 0;
	while (!isdigit(c = *rp++));
	do x = x * 10 + (c - '0');
	while (isdigit(c = *rp++));
}

template<typename T, typename... A>
inline void fast_read(T& fst, A&... args) {
	fast_read(fst), fast_read(args...);
}

inline void mmap_init() {
	struct stat st;
	fstat(0, &st);
	rp = (char*) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
}

class DSU {
private:
	std::vector<int> uni, rank;
	
public:
	DSU(int n) {
		uni.resize(n+1);
		rank.resize(n+1);
	}
	
	int root(int x) {
		return uni[x] ? uni[x] = root(uni[x]) : x;
	}
	
	void insert(int x, int y) {
		x = root(x), y = root(y);
        if (x == y) return;
        if (rank[y] < rank[x]) std::swap(x, y);
        if (rank[x] == rank[y]) rank[y] = rank[x]+1;
        uni[x] = y;
	}
	
	bool related(int x, int y) {
		return root(x) == root(y);
	}
};


int n, m;
std::vector<std::tuple<int, int, int>> edges;

inline int kruskal() {
	DSU dsu(n);
    int tot = 0, ans = 0;
	std::sort(edges.begin(), edges.end());
    for (const auto& [w, u, v]: edges) {
        if (dsu.related(u, v)) continue;
        dsu.insert(u, v);
        ans += w;
		tot++;
		if (tot == n - 1) break;
    }
    return (tot == n - 1) ? ans : -1;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("mst.in", "r", stdin);
#endif
	mmap_init();
	fast_read(n, m);
    edges.resize(m);
    for (auto& [w, u, v]: edges) fast_read(u, v, w);
	int ans = kruskal();
    if (ans == -1) puts("orz");
	else printf("%d\n", ans);
}

// {P3366}
#include <ctype.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <vector>
#include <tuple>
#include <algorithm>


typedef std::tuple<int, int, int> Triple;

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

class DSU {
private:
	std::vector<int> uni, rank;
	
public:
	DSU(int n) {
		uni.resize(n+1); rank.resize(n+1, 1);
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
        if (rank[x] == rank[y]) rank[y] = rank[x]+1;
        uni[x] = y;
	}
	
	bool related(int x, int y) {
		return root(x) == root(y);
	}
	
	int count() {
		int s=0;
		for (int i=1; i<(int) uni.size(); i++)
			if (!uni[i])
				s++;
		return s;
	}
};

int n, m;
std::vector<Triple> edges;

int kruskal() {
    DSU dsu(n);
    int ans = 0;
    for (const auto& [w, u, v]: edges) {
        if (dsu.related(u, v)) continue;
        dsu.insert(u, v);
        ans += w;
    }
    return ans;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("mst.in", "r", stdin);
#endif
	struct stat st;
	fstat(0, &st);
	rp = (char*) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
	fast_read(n, m);
    edges.resize(m);
    DSU dsu(n);
    for (auto& [w, u, v]: edges) {
		fast_read(u, v, w);
        dsu.insert(u, v);
    }
    if (dsu.count() != 1) puts("orz");
	else {
    	std::sort(edges.begin(), edges.end());
		printf("%d\n", kruskal());
	}
}

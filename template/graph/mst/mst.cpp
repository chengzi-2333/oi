#include <bits/stdc++.h>

typedef std::tuple<int, int, int> Triple;

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
		for (int i=1; i<=(int) uni.size(); i++)
			if (uni[i]==i)
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
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    edges.resize(m);
    DSU dsu(n);
    for (auto& [w, u, v]: edges) {
        std::cin >> u >> v >> w;
        dsu.insert(u, v);
    }
    if (dsu.count() != 1) {
        std::cout << "orz" << '\n';
        return 0;
    }
    std::sort(edges.begin(), edges.end());
    std::cout << kruskal() << std::endl;
    // TODO
}

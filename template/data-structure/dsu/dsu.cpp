#include <cstdio>
#include <vector>
#include <numeric>
#include <ctype.h>

// #define DSU_RECURSIVE

inline void fast_read(int& x) {
	char c;
	x = 0;
	while (!isdigit(c = getchar_unlocked()));
	do x = x*10 + (c - '0');
	while (isdigit(c = getchar_unlocked()));
}

template<typename T, typename... Args>
inline void fast_read(T& first, Args&... args) {
	fast_read(first); fast_read(args...);
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
		for (int i=1; i<=(int) uni.size(); i++)
			if (uni[i]==i)
				s++;
		return s;
	}
};

signed main() {
#ifndef ONLINE_JUDGE
    freopen("dsu.in", "r", stdin);
#endif // ONLINE_JUDGE
	static int n, m;
	fast_read(n, m);
	DSU dsu(n);
	while (m--) {
		static int op, x, y;
		fast_read(op, x, y);
		if (--op) 
			putchar_unlocked(dsu.related(x, y) ? 'Y': 'N'),
			putchar_unlocked('\n');
		else dsu.insert(x, y);
	}
}

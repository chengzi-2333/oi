#include <bits/stdc++.h>

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
	}

#ifdef DSU_RECURSIVE
	// recursive
	int root(int x) {
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
};

int n, m;
std::vector<int> pos, mis;
std::vector<std::tuple<int, int, int>> hole;

bool check(int ans) {
    DSU dsu(n);
    for (const auto& [w, u, v]: hole) {
        if (w < ans) break;
        dsu.insert(u, v);
    }
    for (const auto& i: mis) {
        if (!dsu.related(pos[i], i)) return false;
    }
    return true;
}

signed main() {
    #ifndef ONLINE_JUDGE
    freopen("wormhole-sort1.in", "r", stdin);
    #endif // ONLINE_JUDGE
    fast_read(n, m);
    pos.resize(n+1);
    bool flag = true;
    for (int i=1; i<=n; i++) {
        fast_read(pos[i]);
        if (pos[i] != i) {
            flag = false;
            mis.push_back(i);
        }
    }
    if (flag) {
        std::cout << -1 << std::endl;
        return 0;
    }
    hole.resize(m);
    for (auto& [w, u, v]: hole) fast_read(u, v, w);
    std::sort(hole.begin(), hole.end(), std::greater<>());
    auto l = std::get<0>(hole.back()), r = std::get<0>(hole.front());
    while (l <= r) {
        auto mid = l+((r-l)>>1);
        if (check(mid)) l = mid + 1;
        else r = mid - 1;
    }
    std::cout << r << std::endl;
}

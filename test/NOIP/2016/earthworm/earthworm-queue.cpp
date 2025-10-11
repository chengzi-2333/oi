// {P2827}
// queue implementatiom
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m, q, u, v, t;
std::deque<int> worms[3];

inline pair<int, int> gen_pair(int i) {
    return std::make_pair(worms[i].empty() ? INT_MIN : worms[i].front(), i);
}

int pick() {
    return max({gen_pair(0), gen_pair(1), gen_pair(2)}).second;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("earthworm1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m >> q >> u >> v >> t;
    worms[0].resize(n);
    for (auto& worm: worms[0]) cin >> worm;
    std::sort(worms[0].begin(), worms[0].end(), std::greater<int>());
    for (int tk=0; tk<m; tk++) {
        int i = pick();
        int worm = worms[i].front() + q*tk;
        int part = 1ll * worm * u / v;
        worms[i].pop_front();
        worms[1].push_back(part - q - q*tk);
        worms[2].push_back(worm - part - q - q*tk);
        if (tk % t == t-1) cout << worm << sp;
    }
    cout << endl;
    for (int tk=1; tk<=n+m; tk++) {
        int i = pick();
        if (tk % t == 0) cout << worms[i].front() + q*m << sp;
        worms[i].pop_front();
    }
    cout << endl;
}

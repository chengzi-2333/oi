// heap implementation
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m, q, u, v, t;
std::priority_queue<int> worms;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("earthworm1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m >> q >> u >> v >> t;
    for (int worm, i=1; i<=n; i++) {
        cin >> worm;
        worms.push(worm);
    }
    for (int tk=0; tk<m; tk++) {
        int worm = worms.top() + q*tk;
        int part = 1ll * worm * u / v;
        worms.pop();
        worms.push(part - q - q*tk);
        worms.push(worm - part - q - q*tk);
        if (tk % t == t-1) cout << worm << sp;
    }
    cout << endl;
    for (int i=1; i<=n+m; i++) {
        if (i % t == 0) cout << worms.top() + q*m << sp;
        worms.pop();
    }
    cout << endl;
}

// heap implementation
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m, q, u, v, t, offset, worm, part;
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
        offset = q*tk;
        worm = worms.top() + offset;
        if (tk % t == t-1) cout << worm << sp;
        part = worm/v*u;
        worms.pop();
        worms.push(part - q - offset);
        worms.push(worm - part - q - offset);
    }  // TODO
    cout << endl;
    for (int i=1; i<=n+m; i++) {
        if (i % t == 0) cout << worms.top() + q*m << sp;
        worms.pop();
    }
    cout << endl;
}

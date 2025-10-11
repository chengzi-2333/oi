// {<null>}
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m;
vector<bool> tr;


void update(int s, int t, int l, int r, int p, bool key) {}

int query() {}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("3d-game1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m;
    for (int op, p, x; m; m--) {
        cin >> op;
        if (--op) {
            cin >> p >> x;  // TODO
            // update(p, p+x-1, true);
        } else {
            cin >> x;
            cout << query() << lf;
        }
    }
    cout << std::flush;
}

#include <bits/stdc++.h>

using namespace std;

using Obj = tuple<int, int, int, int>;

struct SegmentTree {
    
};

int n;
vector<Obj> obj;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("scan.in", "r", stdin);
#endif
    cin >> n; obj.resize(n);
    for (auto& [xa, ya, xb, yb]: obj) {
        cin >> xa >> ya >> xb >> yb;
    }
}

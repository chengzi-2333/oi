#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n;
vector<int> a;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("inc-dec-seq.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n; a.resize(n+1);
    for (auto it=a.begin()+1; it!=a.end(); it++) {
        cin >> *it;
        // TODO
    }
}

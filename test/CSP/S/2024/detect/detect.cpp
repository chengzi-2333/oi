#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

using Car = tuple<int, int, int>;

int n, m, L, V;
vector<int> detectors;
vector<Car> cars;

tuple<int, int> solve() {
    std::sort(cars.begin(), cars.end());
    // TODO
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("detect1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        cin >> n >> m >> L >> V;
        cars.resize(n); detectors.resize(m);
        for (auto& [d, v, a]: cars) cin >> d >> v >> a;
        for (auto& p: detectors) cin >> p;
        auto [a1, a2] = solve();
        cout << a1 << sp << a2 << lf;
        cars.clear(); detectors.clear();
    }
    cout << std::flush;
}

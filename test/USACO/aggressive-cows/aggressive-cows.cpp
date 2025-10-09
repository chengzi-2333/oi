#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m;
vector<int> a;

inline bool check(int x) {
    int sum = -1;
    int u = a.front();
    for (const auto& v: a) {
        if (v-u < x) sum++;
        else u = v;
    }
    return sum <= n-m;
}

signed main() {
    #ifndef ONLINE_JUDGE
    freopen("aggressive-cows.in", "r", stdin);
    #endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n >> m;
    a.resize(n);
    for (auto& i: a) cin >> i;
    std::sort(a.begin(), a.end());
    int l = 1, r = a.back()-a.front();
    while (l <= r) {
        int mid = ((l+r)>>1);  // l+((r-l)>>1);
        if (check(mid)) l = mid+1;
        else r = mid-1;
    }
    cout << r << endl;
}

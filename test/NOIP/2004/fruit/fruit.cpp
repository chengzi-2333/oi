// {P1090}
// O(nlogn)
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, ans;
std::priority_queue<int, vector<int>, std::greater<int>> h;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("fruit.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    cin >> n;
    for (int a; n; n--) {
        cin >> a;
        h.push(a);
    }
    while (h.size() > 1) {
        int a = h.top(); h.pop();
        int b = h.top(); h.pop();
        ans += a+b;
        h.push(a+b);
    }
    cout << ans << endl;
}

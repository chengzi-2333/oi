#include <bits/stdc++.h>

using namespace std;

typedef tuple<int, int, int> Car;

int v;

auto check(int d, int v0, int a) {
    return [d, v0, a](int p) {
        return v0*v0+2*a*(p-d) > v*v;
    };
}

pair<int, int> get_range() {
    return {0, 0};
}

int cover(vector<pair<int, int>> ranges) {
    sort(ranges.begin(), ranges.end());
    int res = 0;
    return res;
}

void solve() {
    int n, m, l, cnt=0, c0=0, c1=0, res=0;
    cin >> n >> m >> l >> v;
    vector<Car> cars(n);
    vector<int> detectors(m);
    vector<pair<int, int>> ranges;
    for (auto& [di, vi, ai]: cars) 
        cin >> di >> vi >> ai;
    for (auto& det: detectors) 
        cin >> det;
    sort(detectors.begin(), detectors.end());
    for (auto& [di, vi, ai]: cars) {
        if (ai == 0) c0++;
        else if (ai > 0) c1++;
        if (di > detectors.back()) continue;
        auto checker = check(di, vi, ai);
        // if (ai >= 0 && !checker(detectors.back())) continue;
        if (checker(detectors.back()) || checker(detectors.front())) {
            cnt++;
            ranges.push_back(get_range());
        }
    }
    if (c0 == n || c1 == n) {
        if (cnt) res = m-1;
        else res = m;
    } else res = m - cover(ranges);
    cout << cnt << ' ' << res;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("speed.in", "r", stdin);
#endif
    int t; cin >> t;
    while (t--) solve();
}

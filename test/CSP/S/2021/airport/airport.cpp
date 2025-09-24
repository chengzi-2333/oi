#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair;
using std::max, std::min;

using PII = pair<int, int>;

constexpr char lf = '\n';
constexpr char sp = ' ';

int n, m1, m2, ans;
vector<PII> time_table[2];

vector<int> solve(const vector<PII>& tb) {
    vector<int> res(n+1);
    std::priority_queue<PII, vector<PII>, std::greater<PII>> plane;
    std::priority_queue<int, vector<int>, std::greater<int>> spare;
    for (int i=1; i<=n; i++) spare.push(i);
    for (const auto& [s, t]: tb) {
        while (!plane.empty() && s >= plane.top().first) {
            spare.push(plane.top().second);
            plane.pop();
        }
        if (spare.empty()) continue;
        res[spare.top()]++;
        plane.emplace(t, spare.top());
        spare.pop();
    }
    for (int i=1; i<=n; i++) res[i] += res[i-1];
    return res;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("airport1.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    // 准备入港咯！ATC要忙不过来了（
    cin >> n >> m1 >> m2;
    time_table[0].resize(m1);
    time_table[1].resize(m2);
    for (auto& [s, t]: time_table[0]) cin >> s >> t;
    for (auto& [s, t]: time_table[1]) cin >> s >> t;
    std::sort(time_table[0].begin(), time_table[0].end());
    std::sort(time_table[1].begin(), time_table[1].end());
    // 开始模拟
    auto a1 = solve(time_table[0]), a2 = solve(time_table[1]);
    for (int i=0; i<=n; i++) ans = max(ans, a1[i] + a2[n-i]);
    cout << ans << endl;
}

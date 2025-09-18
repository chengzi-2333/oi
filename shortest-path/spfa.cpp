#include <bits/stdc++.h>

using namespace std;

constexpr int N = 3e3;
constexpr long long inf = 0x3f3f3f3f3f3f3f3f;

int n, m;
long long d[N+2][N+2];


bool spfa(int s) {}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("multi-source-1.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m;
    // TODO
    for (int i=1; i<=n; i++) {
        long long sum = 0;
        for (int j=1; j<=n; j++) {
            if (d[i][j] == inf) sum += (long long) j*1e9;
            else sum += j*d[i][j];
        }
        cout << sum << '\n';
    }
    cout << flush;
}

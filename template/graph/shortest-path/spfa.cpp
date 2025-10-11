// {P5905}
#include <bits/stdc++.h>

using namespace std;

constexpr int N = 3e3;
constexpr long long inf = 0x3f3f3f3f3f3f3f3f;

int n, m;
vector<vector<pair<int, int>>> g;
long long d[N+2][N+2];


bool spfa(int s) {
    queue<int> q;
    vector<bool> in_que(n+1);
    vector<int> cnt(n+1);
    d[s][s] = 0, in_que[s] = true, q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop(), in_que[u] = false;
        for (const auto& [v, w]: g[u]) {
            if (d[s][v] > d[s][u] + w) {
                d[s][v] = d[s][u] + w;
                cnt[v] = cnt[u] + 1;
                if (cnt[v] > n) return true;
                if (!in_que[v]) q.push(v), in_que[v] = true;
            }
        }
    }
    return false;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("multi-source-1.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m; g.resize(n+1);
    for (int p=1; p<=n; p++) g[0].emplace_back(p, 0);
    for (int u, v, w; m; m--) {
        cin >> u >> v >> w;
        g[u].emplace_back(v, w);
    }
    memset(d, 0x3f, sizeof(d));
    for (int p=0; p<=n; p++) {
        if (spfa(p)) {
            cout << -1 << endl;
            return 0;
        }
    }
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

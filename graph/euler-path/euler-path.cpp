#include <bits/stdc++.h>

using namespace std;

int n, m, s=1, cs, ct;
vector<priority_queue<int, vector<int>, greater<int>>> g;
vector<int> din, dout;
stack<int> path;

void dfs(int u) {
    for (int v; !g[u].empty(); ) {
        v = g[u].top();
        g[u].pop();
        dfs(v);
    }
    path.push(u);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("euler-path1.in", "r", stdin);
#endif
    cin.tie(nullptr) -> sync_with_stdio(false);
    cin >> n >> m;
    din.resize(n+1), dout.resize(n+1);
    g.resize(n+1);
    for (int u, v; m; m--) {
        cin >> u >> v;
        g[u].push(v);
        din[v]++, dout[u]++;
    }
    for (int i=1; i<=n; i++) {
        if (abs(din[i]-dout[i]) > 1) {
            puts("No");
            return 0;
        }
        if (dout[i]-din[i] == 1) {
            cs++;
            s = i;
        } else if (din[i]-dout[i] == 1) {
            ct++;
        }
    }
    if ((cs != ct) || (cs != 0 && cs != 1)) {
        puts("No");
        return 0;
    }
    dfs(s);
    while (!path.empty()) {
        cout << path.top() << ' ';
        path.pop();
    }
    cout << endl;
}

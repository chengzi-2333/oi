#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;

constexpr int N = 100, MOD = 9999973;

int n, m;
long long ans, f[N+4][N+4][N+4];

long long comb(int n) {
    return n * (n - 1) / 2;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("chess.in", "r", stdin);
#endif
    cin >> n >> m;
    if (n < m) std::swap(n, m);
    for (int i=1; i<=n; i++) {  // row i
        for (int a=0; a<=m; a++) {  // a columns were put 1 chess
            for (int b=0; b<=m-a; b++) {  // b columns were put 2 chess
                // circumstance 1：don't put chess
                    // directly update
                    f[i][a][b] = f[i-1][a][b];
                // circumstance 2: put 1 chess  // TODO
                    // subcircumstance 1: current column has 1 chess
                    // subcircumstance 2: current column has no chess
                // circumstance 3: put 2 chess
                    // subcircumstance 1: current column has no chess
                    // subcircumstance 2: 
            }
        }
    }
    cout << ans << endl;
}

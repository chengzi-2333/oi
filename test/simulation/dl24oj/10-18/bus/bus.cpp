#include <bits/stdc++.h>

template <typename T>
inline void fast_read(T& x) {
    char c;
    x = 0;
    while (!isdigit(c = getchar()));
    do x = x * 10 + (c - '0');
    while (isdigit(c = getchar()));
}

template <typename T, typename... A>
inline void fast_read(T& x, A&... a) {
    fast_read(x), fast_read(a...);
}


constexpr int N = 1e3, M = 1e4;
struct plan {
    int t, u, v;

    bool operator <(const plan& p) const {
        return u < p.u;
    }
} p[M+5];
int n, m, k, d[N+5];

int main() {
    freopen("bus.in", "r", stdin);
    freopen("bus.out", "w", stdout);
    fast_read(n, m, k);
    for (auto dp=d+1; dp-d<n; dp++) fast_read(*dp);
    for (auto pp=p+1; pp-p<=n; pp++) fast_read(pp->t, pp->u, pp->v);
    std::sort(p+1, p+1+n);
    printf("%d\n", std::accumulate(d+1, d+n+1, 0));
}

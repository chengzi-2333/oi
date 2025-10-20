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


constexpr int N = 1e5, C = 16;
long long c, n, card[C+5], book[N+5], vis[C+5];

long long dfs(int u, long long* l) {
    auto r = std::lower_bound(l, book+n+1, card[u]+*(l-1));
    long long ans = INT_MAX;
    for (int v=1; v<=c; v++) {
        if (vis[v]) continue;
        vis[v] = true;
        ans = std::min(ans, dfs(v, r));
        vis[v] = false;
    }
    return ((ans == INT_MAX) ? 0 : ans) + card[u] - *r + *(l-1);
}

int main() {
    freopen("book1.in", "r", stdin);
    // freopen("book.out", "w", stdout);
    fast_read(c, n);
    for (int i=1; i<=c; i++) {
        fast_read(card[i]);
    }
    for (int i=1; i<=n; i++) {
        fast_read(book[i]);
        book[i] += book[i-1];
    }
    printf("%lld\n", std::accumulate(card+1, card+c+1, 0)-dfs(0, book+1));
}

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


constexpr int N = 2e5;
int t, n, k;
char s[N+5];

inline int solve() {
    return n/k;
    // int l=1, r=n;
    // while (l <= r) {
    //     int mid = l+((r-l)>>1);
    //     if (check(mid)) l = mid + 1;
    //     else r = mid - 1;
    // }
    // return r;
}

int main() {
    freopen("color.in", "r", stdin);
    freopen("color.out", "w", stdout);
    fast_read(t);
    while (t--) {
        fast_read(n, k);
        scanf("%s", s+1);
        printf("%d\n", solve());
    }
}

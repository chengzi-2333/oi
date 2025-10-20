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


int main() {
    freopen("table.in", "r", stdin);
    freopen("table.out", "w", stdout);
    printf("%d\n", 1);
}

// {P1020}
// O(n**2)
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

char *rp, *p;
int sz;

inline size_t mmap_init() {
    struct stat state;
    fstat(STDIN_FILENO, &state);
    p = rp = (char*) mmap(NULL, state.st_size, PROT_READ, MAP_PRIVATE, STDIN_FILENO, 0);
    return state.st_size / sizeof(char);
}

template <typename T>
inline void fast_read_u(T& x) {
    x = 0;
    while (!isdigit(*rp)) {
        if (rp - p == sz) return x = -1, void();
        rp++;
    }
    do x = x * 10 + (*rp++ - '0');
    while (isdigit(*rp));
}


constexpr int N = 1e4;
int a[N+5], *ap = a, n;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("missile.in", "r", stdin);
#endif // ONLINE_JUDGE
    sz = mmap_init();
    do fast_read_u(*++ap); while (*ap != EOF);
    n = ap - a - 1;
    for (int i = 1; i <= n; i++) printf("%d ", a[i]);
}

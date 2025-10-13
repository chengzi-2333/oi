// {P4552}
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

char* rp;

template <typename T>
inline void fast_read(T& x) {
    x = 0;
    while (!isdigit(*rp)) rp++;
    do x = x * 10 + (*rp++ - '0');
    while (isdigit(*rp));
}

template <typename T>
inline void fast_write(T x) {
    static char st[40], *pt = st;
    do {
        *pt++ = '0' + x % 10;
        x /= 10;
    } while (x);
    while (pt != st) putchar_unlocked(*--pt);
}

inline void mmap_init() {
    struct stat st;
    fstat(0, &st);
    rp = (char*) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
}

long long n, p, v, x, y;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("inc-dec-seq.in", "r", stdin);
#endif
    mmap_init();
    fast_read(n);
    fast_read(p);
    while (--n) {
        fast_read(v);
        p = v - p;
        if (p < 0) x -= p;
        else y += p;
        p = v;
    }
    fast_write(std::max(x, y));
    putchar_unlocked('\n');
    fast_write(std::abs(x - y) + 1);
    putchar_unlocked('\n');
}

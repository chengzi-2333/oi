#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define BUFSIZE (1 << 20)

char* rp;
char wbuf[BUFSIZE], *wp = wbuf;

template <typename T>
inline void fast_read(T& x) {
	x = 0;
	while (!isdigit(*rp)) rp++;
	do x = x*10 + (*rp++ - '0');
	while (isdigit(*rp));
}

inline void mmap_init() {
    struct stat st;
    fstat(0, &st);
    rp = (char*) mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
}

inline void flush() {
    fwrite(wbuf, 1, wp - wbuf, stdout);
    wp = wbuf;
}

inline void put(const char& c) {
    if (wp - wbuf == BUFSIZE) flush();
    *wp++ = c;
}

template <typename T>
inline void fast_write(T x) {
    static char st[40], *pt = st;
    do {
        *pt++ = '0' + x % 10;
        x /= 10;
    } while (x);
    while (pt != st) put(*--pt);
}

constexpr int N = 3e6;

struct pair {
    int first, second;
} st[N+2], *sp = st - 1;
int n, p, f[N+2], *fp = f;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("monotonous-stack.in", "r", stdin);
#endif // ONLINE_JUDGE
    mmap_init();
    fast_read(n);
    for (int a, i = 1; i <= n; i++) {
        fast_read(a);
        while (sp >= st && sp->second < a) {
            f[sp->first-1] = i;
            sp--;
        }
        *++sp = {i, a};
    }
    while (fp - f < n) fast_write(*fp++), put(' ');
    flush();
}

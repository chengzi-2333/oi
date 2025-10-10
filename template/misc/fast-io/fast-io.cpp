#include <stdio.h>
#include <ctype.h>

// #define NO_BUF

#ifndef NO_BUF
#define BUFSIZE (1 << 20)
char buf[BUFSIZE], *p1=buf, *p2=buf;
char pbuf[BUFSIZE], *pp=pbuf;
#endif // NO_BUF

inline char get() {
#ifdef NO_BUF
    return getchar_unlocked();
#else // NO_BUF
    if (p1 == p2) p2 = (p1 = buf) + fread(buf, 1, BUFSIZE, stdin);
    return (p1 == p2 ? ' ' : *p1++);
#endif // NO_BUF
}

inline void put(const char& c) {
#ifdef NO_BUF
    putchar_unlocked(c);
#else // NO_BUF
    if (pp - pbuf == BUFSIZE) fwrite(pbuf, 1, BUFSIZE, stdout), pp = pbuf;
    *pp++ = c;
#endif // NO_BUF
}

#ifndef NO_BUF
inline void push() {
    fwrite(pbuf, 1, pp - pbuf, stdout);
}
#endif

template<typename T>
inline void fast_read(T& x) {
    char c;
    x = 0;
    T neg = 1;
    while (!isdigit(c = get())) {
        if (c == '-') neg = -1;
    }
    do x = x * 10 + neg*(c - '0');
    while (isdigit(c = get()));
}

template<typename T>
inline void fast_write(T x) {
    T neg = 1;
    if (x < 0) {
        neg = -1;
        put('-');
    }
    static char st[40];
    char* p = st;
    do {
        *p++ = '0' + neg*(x%10);
        x /= 10;
    } while (x);
    while (p != st) put(*--p);
}

template<typename T>
inline void fast_read_u(T& x) {
    char c;
    x = 0;
    while (!isdigit(c = get()));
    do x = x*10 + (c - '0');
    while (isdigit(c = get()));
}

template<typename T>
inline void fast_write_u(T x) {
    static char st[40];
    char* p = st;
    do {
        *p++ = x%10 + '0';
        x /= 10;
    } while (x);
    while (p != st) put(*--p);
}

template<typename T, typename... Args>
inline void fast_read(T& first, Args&... args) {
    fast_read(first); fast_read(args...);
}

template<typename T, typename... Args>
inline void fast_read_u(T& fst, Args&... args) {
    fast_read_u(fst), fast_read_u(args...);
}

int n, a, sum;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("fast-io.in", "r", stdin);
#endif // ONLINE_JUDGE
    fast_read(n);
    while (n--) fast_read(a), sum += a;
    fast_write(sum);
#ifndef NO_BUF
    push();  // Don't forget to push buffer to stdout!
#endif
}

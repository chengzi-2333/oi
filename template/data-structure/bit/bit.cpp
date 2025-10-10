#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define BUFSIZE (1 << 20)

char *rp;
char wbuf[BUFSIZE], *wp = wbuf;

inline void flush() {
    fwrite(wbuf, 1, wp - wbuf, stdout);
    wp = wbuf;
}

inline char get() {
    return *rp++;
}

inline void put(const char& c) {
    if (wp - wbuf == BUFSIZE) flush();
    *wp++ = c;
}

inline void fast_read(int& x) {
    char c;
    x = 0;
    int neg = 1;
    while (!isdigit(c = get())) {
        if (c == '-') neg = -1;
    }
    do x = x * 10 + neg * (c - '0');
    while (isdigit(c = get()));
}

inline void fast_write(int x) {
    int neg = 1;
    if (x < 0) {
        neg = -1;
        put('-');
    }
    static char st[40];
    char* tp = st;
    do {
        *tp++ = '0' + neg * (x % 10);
        x /= 10;
    } while (x);
    while (tp != st) put(*--tp);
}

template<typename T, typename... A>
inline void fast_read(T& fst, A&... args) {
    fast_read(fst), fast_read(args...);
}

#define lowbit(x) (x & -x)

int n, m;
std::vector<int> arr;

inline void update(int x, int k) {
    for (; x<=n; x+=lowbit(x)) arr[x] += k;
}

inline int query(int x) {
    int sum = 0;
    for (; x; x-=lowbit(x)) sum += arr[x];
    return sum;
}

inline int query(int l, int r) {
    return query(r)-query(l-1);
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("bit.in", "r", stdin);
#endif // ONLINE_JUDGE
    struct stat state;
    fstat(STDIN_FILENO, &state);
    rp = (char*) mmap(NULL, state.st_size, PROT_READ, MAP_PRIVATE, STDIN_FILENO, 0);
    // main
    fast_read(n, m);
    arr.resize(n+1);
    for (int a, i=1; i<=n; i++) {
        fast_read(a);
        update(i, a);
    }
    for (int op, x, y; m; m--) {
        fast_read(op, x, y);
        if (op-1) fast_write(query(x, y)), put('\n');
        else update(x, y);
    }
    flush();
}

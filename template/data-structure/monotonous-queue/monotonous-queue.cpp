#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define BUFSIZE (1 << 20)
#define MMAP

namespace IO {
#ifdef MMAP
    char* rp;
#else
    char rbuf[BUFSIZE], *rp = rbuf, *bp = rbuf;
#endif
    char wbuf[BUFSIZE], *wp = wbuf;

    inline char get() {
    #ifdef MMAP
        return *rp++;
    #else
        if (rp == bp)
            bp = (rp = rbuf) + fread(rbuf, 1, BUFSIZE, stdin);
        return (rp == bp ? ' ' : *rp++);
    #endif
    }

    inline void flush() {
        fwrite(wbuf, 1, wp - wbuf, stdout);
        wp = wbuf;
    }

    inline void put(const char& c) {
        if (wp - wbuf == BUFSIZE)
            flush();
        *wp++ = c;
    }

    inline void mmap_init() {
    #ifdef MMAP
        struct stat state;
        fstat(STDIN_FILENO, &state);
        rp = (char*)mmap(NULL, state.st_size, PROT_READ, MAP_PRIVATE, STDIN_FILENO, 0);
    #endif  // MMAP
    }

    // #define get() getchar_unlocked()
    // #define put(x) putchar_unlocked(x)

    template <typename T>
    inline void fast_read(T& x) {
        char c;
        x = 0;
        T neg = 1;
        while (!isdigit(c = get())) {
            if (c == '-')
                neg = -1;
        }
        do x = x * 10 + neg * (c - '0');
        while (isdigit(c = get()));
    }

    template <typename T>
    inline void fast_write(T x) {
        T neg = 1;
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

    template <typename T>
    inline void fast_read_u(T& x) {
        char c;
        x = 0;
        while (!isdigit(c = get()));
        do x = x * 10 + (c - '0');
        while (isdigit(c = get()));
    }

    template <typename T>
    inline void fast_write_u(T x) {
        static char st[40];
        char* tp = st;
        do {
            *tp++ = x % 10 + '0';
            x /= 10;
        } while (x);
        while (tp != st) put(*--tp);
    }

    template <typename T, typename... Args>
    inline void fast_read(T& first, Args&... args) {
        fast_read(first), fast_read(args...);
    }

    template <typename T, typename... Args>
    inline void fast_read_u(T& fst, Args&... args) {
        fast_read_u(fst), fast_read_u(args...);
    }
}  // namespace IO


signed main() {
#ifndef ONLINE_JUDGE
    freopen("monotonous-queue.in", "r", stdin);
#endif // ONLINE_JUDGE
    // std::cin.tie(nullptr)->sync_with_stdio(false);
    IO::mmap_init();
    // TODO
}

// {P1064}
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


int n, c, cnt;
std::vector<int> f;
std::vector<std::deque<std::pair<int, int>>> groups;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("budget.in", "r", stdin);
#endif // ONLINE_JUDGE
    IO::mmap_init();
    IO::fast_read_u(c, n);
    f.resize(c + 1);
    groups.resize(n + 1);
    for (int v, w, k, i = 1; i <= n; i++) {
        IO::fast_read_u(v, w, k);
        w *= v;
        if (k) groups[k].emplace_back(v, w);
        else groups[i].emplace_front(v, w);
    }
    for (auto& group: groups) {
        if (!group.empty()) {
            if (group.size() >= 2) {
                auto& [v, w] = group[0];
                auto& [vi, wi] = group[1];
                if (group.size() == 3) {
                    auto& [vj, wj] = group[2];
                    group.emplace_back(v + vi + vj, w + wi + wj);
                    vj += v, wj += w;
                }
                vi += v, wi += w;
            }
        }
    }
    for (const auto& group: groups) {
        for (int j = c; j >= 0; j--) {
            for (const auto& [vi, wi]: group) {
                if (j >= vi) {
                    f[j] = std::max(f[j], f[j-vi] + wi);
                }
            }
        }
    }
    IO::fast_write_u(f[c]);
    IO::put('\n');
    IO::flush();
}

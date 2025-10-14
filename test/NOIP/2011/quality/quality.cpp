// {P1314}
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


int n, m;
long long s;
std::vector<std::pair<int, int>> stone, range;

long long calculate(const int& w) {
    long long sum = 0;
    static std::vector<long long> sum_a(n+1), sum_b(n+1);
    for (int i = 1; i <= n; i++) {
        sum_a[i] = (stone[i-1].first >= w);
        sum_b[i] = sum_a[i] * stone[i-1].second;
        sum_a[i] += sum_a[i-1];
        sum_b[i] += sum_b[i-1];
    }
    for (const auto& [l, r]: range) {
        sum += (sum_a[r] - sum_a[l-1]) * (sum_b[r] - sum_b[l-1]);
    }
    return sum;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("quality.in", "r", stdin);
#endif // ONLINE_JUDGE
    // std::cin.tie(nullptr)->sync_with_stdio(false);
    IO::mmap_init();
    IO::fast_read_u(n, m, s);
    stone.resize(n); range.resize(m);
    for (auto& [w, v]: stone) IO::fast_read_u(w, v);
    for (auto& [l, r]: range) IO::fast_read_u(l, r);
    long long ans = s;
    int l = 1, r = std::max_element(stone.begin(), stone.end())->first;
    while (l <= r) {
        int mid = l + ((r - l) >> 1);
        long long res = calculate(mid);
        if (res > s) l = mid + 1;
        else r = mid - 1;
        ans = std::min(ans, std::abs(s - res));
    }
    IO::fast_write_u(ans);
    IO::put('\n');
    IO::flush();
}

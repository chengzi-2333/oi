// {P1020}
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


constexpr int N = 1e5;
int n, a[N+5], f[N+5];

inline int read_missiles() {
    int* ap = a;
    sz = mmap_init();
    do fast_read_u(*++ap); while (*ap != EOF);
    return ap - a - 1;
}

// O(n**2) implementation
// inline int lis(const std::function<bool(int, int)>& cmp) {
//     memset(f, 0, (n+1) * sizeof(int));
//     for (int i = 1; i <= n; i++) {
//         f[i] = 1;
//         for (int j = 0; j < i; j++) {
//             if (cmp(a[i], a[j])) {
//                 f[i] = std::max(f[i], f[j]+1);
//             }
//         }
//     }
//     return *std::max_element(f, f+n+1);
// }

// O(nlogn) implementation
inline int lis(const std::function<bool(int, int)>& cmp) {
    auto rev_cmp = std::bind(cmp, std::placeholders::_2, std::placeholders::_1);
    memset(f, 0, (n+1) * sizeof(int));
    int* fp = f + 1;
    *fp = *(a + 1);
    for (auto ap = a + 2; ap - a <= n; ap++) {
        (cmp(*ap, *fp) ? *++fp : *std::lower_bound(f + 1, fp + 1, *ap, rev_cmp)) = *ap;
    }
    return fp - f;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("missile.in", "r", stdin);
#endif // ONLINE_JUDGE
    n = read_missiles();
    printf("%d\n%d\n", lis(std::less_equal<>()), lis(std::greater<>()));
}

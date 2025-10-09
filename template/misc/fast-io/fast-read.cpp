#include <stdio.h>
#include <ctype.h>

// modify reference
inline void fast_read(int& x) {
    char c;
    x = 0;
    bool neg = false;
    while (!isdigit(c = getchar_unlocked())) {
        if (c == '-') neg = true;
    }
    do x = x * 10 + (c - '0');
    while (isdigit(c = getchar_unlocked()));
    if (neg) x = -x;
}

// return read value
inline int fast_read() {
    char c;
    int x = 0;
    bool neg = false;
    while (!isdigit(c = getchar_unlocked())) {
        if (c == '-') neg = true;
    }
    do x = x * 10 + (c - '0');
    while (isdigit(c = getchar_unlocked()));
    return neg ? -x : x;
}

// template
template<typename T, typename... Args>
inline void fast_read(T& first, Args&... args) {
    fast_read(first); fast_read(args...);
}

int n, sum;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("fast-read.in", "r", stdin);
#endif // ONLINE_JUDGE
    n = fast_read();
    while (n--) sum += fast_read();
    printf("%d", sum);
}

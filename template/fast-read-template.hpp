#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>

char* rp;

template <typename T>
inline void fast_read(T& x) {
	x = 0;
	while (!isdigit(*rp)) rp++;
	do x = x*10 + (*rp++ - '0');
	while (isdigit(*rp));
}

template <typename T, typename... Args>
inline void fast_read(T& first, Args&... args) {
	fast_read(first); fast_read(args...);
}

inline void mmap_init() {
    struct stat st;
    fstat(0, &st);
    rp = (char*) mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
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

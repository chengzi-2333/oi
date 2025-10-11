#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

char* rp;

inline void fast_read(int& x) {
	char c;
	x = 0;
	while (!isdigit(c = *rp++));
	do x = x*10 + (c - '0');
	while (isdigit(c = *rp++));
}

template<typename T, typename... Args>
inline void fast_read(T& first, Args&... args) {
	fast_read(first); fast_read(args...);
}

inline void mmap_init() {
    struct stat st;
    fstat(0, &st);
    rp = (char*) mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, 0, 0);
}

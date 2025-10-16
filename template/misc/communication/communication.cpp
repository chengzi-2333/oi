// {P12509}
// Hamming Code
#include <string>

inline bool is_pw2(int x) {return x && !((x-1) & x);}

inline int fill_buf(const std::string& s, unsigned char* buf) {
    int k = 0;
    while ((1 << k) < s.length() + k + 1) k++;
    int buf_size = (1 << k) - 1;
    unsigned char* pbuf = buf + 1;
    for (auto it = s.begin() + 1; pbuf - buf <= buf_size && it < s.end(); pbuf++) {
        if (!is_pw2(pbuf - buf)) *pbuf = *it++ - '0';
    }
    return buf_size;
}

int Alice(std::string S) {
    int cnt[21] = {0};
    unsigned char buf[1 << 20] = {0};
    int n = fill_buf(S, buf);
    for (int i = 1; i <= n; i++) {
        if (buf[i]) {
            for (int c = 0, bi = i; bi; bi >>= 1, c++) {
                if (bi & 1) cnt[c]++;
            }
        }
    }
    int ret = 0;
    for (int i = 0; (1 << i) <= n; i++) {
        ret |= (buf[1 << i] = (cnt[i] & 1)) << i;
    }
    return ret;
}

int Bob(std::string T, int X) {
    unsigned char buf[1 << 20] = {0};
    int n = fill_buf(T, buf);
    for (int i = 0; (1 << i) <= n; i++) {
        buf[1 << i] = (X >> i) & 1;
    }
    unsigned int pos = 0;
    for (int i = 1; i <= n; i++) {
        if (buf[i]) pos ^= i;
    }
    return pos ? pos - std::__lg(pos) : 0;
}

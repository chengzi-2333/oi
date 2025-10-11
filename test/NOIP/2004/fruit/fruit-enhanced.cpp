// {P1090}
// O(n+k)
#include <bits/stdc++.h>

using std::cin, std::cout, std::endl;
using std::vector, std::pair, std::tuple;
using std::max, std::min;

constexpr char lf = '\n';
constexpr char sp = ' ';

inline void fast_read(long long &x) {
    char c;
    x = 0;
    bool neg = false;
    while (!isdigit(c = getchar_unlocked())) {
        if (c == '-') neg = true;
    }
    do {
        x = x * 10 + (c - '0');
    } while (isdigit(c = getchar_unlocked()));
    if (neg) x = -x;
}

constexpr int K = 1e5;

int n;
long long ans;
std::array<long long, K+5> bucket;
std::queue<long long> que[2];

inline std::queue<long long>& pick() {
    if (que[0].empty()) return que[1];
    else if (que[1].empty()) return que[0];
    return que[0].front() < que[1].front() ? que[0] : que[1];
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("fruit.in", "r", stdin);
#endif // ONLINE_JUDGE
    cin >> n;
    for (long long a, i=1; i<=n; i++) {
        // cin >> a;
        fast_read(a);
        bucket[a]++;
    }
    // bucket sort
    for (long long i=1; i<=K; i++) {
        while (bucket[i]) {
            bucket[i]--;
            que[0].push(i);
        }
    }
    while (que[0].size() + que[1].size() > 1) {
        auto &que1 = pick();
        auto a = que1.front(); que1.pop();
        auto &que2 = pick();
        auto b = que2.front(); que2.pop();
        ans += a+b;
        que[1].push(a+b);
    }
    cout << ans << endl;
}

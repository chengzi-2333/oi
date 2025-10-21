#include <bits/stdc++.h>

template <typename T>
inline void fast_read(T& x) {
    char c;
    x = 0;
    while (!isdigit(c = getchar()));
    do x = x * 10 + (c - '0');
    while (isdigit(c = getchar()));
}

template <typename T, typename... A>
inline void fast_read(T& x, A&... a) {
    fast_read(x), fast_read(a...);
}


int c, n;
std::deque<int> card, book;

// T_1(n, c) = (c * log(c) + c! * (n + c)) == O(c! * n)
int solve1() {
    int ans = -1;
    std::sort(card.begin(), card.end());
    do {
        for (auto cd = card.begin(), bk = book.begin(); cd < card.end(); cd++) {
            int s = 0;
            while (bk < book.end() && s + *bk <= *cd) s += *bk++;
            if (bk == book.end()) {
                ans = std::max(ans, std::accumulate(cd + 1, card.end(), 0));
                break;
            }
        }
    } while (std::next_permutation(card.begin(), card.end()));
    return ans;
}

// T_2(n, c) = (c * log(c) + n + c! * (log(n) + c)) == O(c! * log(n))
int solve2() {
    int ans = -1;
    book.push_front(0);
    std::sort(card.begin(), card.end());
    std::partial_sum(book.begin(), book.end(), book.begin());
    do {
        for (auto cd = card.begin(), bk = book.begin() + 1; cd < card.end(); cd++) {
            // Replace accumulation with bisection and prefix-sum
            bk = std::upper_bound(bk, book.end(), *cd + *(bk - 1));
            if (bk == book.end()) {
                ans = std::max(ans, std::accumulate(cd + 1, card.end(), 0));
                break;
            }
        }
    } while (std::next_permutation(card.begin(), card.end()));
    return ans;
}

// T_3(n, c) ~ T_2(n, c) = (c * log(c) + n + c! * (c + log(n))) == O(c! * log(n))
int solve3() {
    int ans = -1;
    card.push_front(0);
    book.push_front(0);
    std::sort(card.begin() + 1, card.end());
    std::partial_sum(book.begin(), book.end(), book.begin());
    do {
        std::partial_sum(card.begin(), card.end(), card.begin());
        for (auto cd = card.begin() + 1, bk = book.begin() + 1; cd < card.end(); cd++) {
            // Replace accumulation with bisection and prefix-sum
            bk = std::upper_bound(bk, book.end(), *cd - *(cd - 1) + *(bk - 1));
            if (bk == book.end()) {
                ans = std::max(ans, card.back() - *cd); // prefix-sum
                break;
            }
        }
        std::adjacent_difference(card.begin(), card.end(), card.begin());
    } while (std::next_permutation(card.begin() + 1, card.end()));
    return ans;
}

// T_4(n, c) = (2^c * c + 2^c * c * log(n)) == O(2^c * c * log(n))
int solve4() {
    int ans = -1;
    book.push_front(0);
    std::partial_sum(book.begin(), book.end(), book.begin());
    std::vector<int> sum(1 << c);
    std::vector<std::deque<int>::iterator> dp(1 << c, book.begin() - 1);
    dp[0] = book.begin();
    for (int s = 0; s < (1 << c); s++) {
        for (int i = 0; i < c; i++) {
            if ((s >> i) & 1) continue;
            sum[s] += card[i];
        }
    }
    for (int s = 0; s < (1 << c); s++) {
        if (dp[s] < book.begin()) continue;
        for (int i = 0; i < c; i++) {
            if ((s >> i) & 1) continue;
            int si = (s | (1 << i));
            dp[si] = std::max(dp[si], std::upper_bound(dp[s] + 1, book.end(), *dp[s] + card[i]) - 1);
            if (dp[si] == book.end() - 1) ans = std::max(ans, sum[si]);
        }
    }
    return ans;
}

int main() {
    freopen("book.in", "r", stdin);
    freopen("book.out", "w", stdout);
    fast_read(c, n);
    card.resize(c);
    book.resize(n);
    for (auto& c: card) fast_read(c);
    for (auto& b: book) fast_read(b);
    std::cout << solve4() << std::endl;
}

#include <bits/stdc++.h>

#define int long long

using PII = std::pair<int, int>;

int n, m;
std::vector<PII> items;

// O(m * n)
inline void solve1() {
    for (int v, cnt = 0; m; m--, cnt = 0) {
        std::cin >> v;
        for (const auto& [w, c]: items) {
            auto num = std::min(v / w, c);
            cnt += num;
            v -= num * w;
        }
        std::cout << cnt << '\n';
    }
}

struct Item {
    int w, c = 0, wcs = 0, cs = 0;
};

inline std::vector<Item>::iterator bisect1(std::vector<Item>::iterator l, std::vector<Item>::iterator r, int v) {
    while (l <= r) {
        auto mid = l + ((r - l) >> 1);
        if (mid->w > v) l = mid + 1;
        else r = mid - 1;
    }
    return l;
}

inline std::vector<Item>::iterator bisect2(std::vector<Item>::iterator l, std::vector<Item>::iterator r, int v) {
    auto st = l - 1;
    while (l <= r) {
        auto mid = l + ((r - l) >> 1);
        if (mid->wcs - st->wcs <= v) l = mid + 1;
        else r = mid - 1;
    }
    return l - 1;
}

// O(m * log(n))
inline void solve2() {
    std::vector<Item> sum(n + 1);
    for (int i = 1; i <= n; i++) {
        std::tie(sum[i].w, sum[i].c) = items[i - 1];
        sum[i].wcs = sum[i - 1].wcs + sum[i].w * sum[i].c;
        sum[i].cs = sum[i - 1].cs + sum[i].c;
    }
    for (int v, cnt = 0; m; m--, cnt = 0) {
        std::cin >> v;
        auto l = sum.begin(), r = sum.begin();  // [l, r]
        while (v) {
            // l = std::lower_bound(r + 1, sum.end(), Item(v), [](const Item& mid, const Item& val){
            //     return mid.w > val.w;
            // });
            l = bisect1(r + 1, sum.end() - 1, v);
            if (l == sum.end()) break;
            // r = std::lower_bound(l, sum.end(), Item{v}, [l](const Item& mid, const Item& val){
            //     return mid.wcs - l->wcs <= val.wcs;
            // }) - 1;
            r = bisect2(l, sum.end() - 1, v);
            if (r != std::prev(l)) {
                v -= r->wcs - std::prev(l)->wcs;
                cnt += r->cs - std::prev(l)->cs;
            } else {
                auto num = v / l->w;
                v -= l->w * num;
                cnt += num;
                r = l;
            }
        }
        std::cout << cnt << '\n';
    }
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("shop1.in", "r", stdin);
#endif // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    items.resize(n);
    for (auto& [w, c]: items) std::cin >> w >> c;
    std::sort(items.begin(), items.end(), std::greater<PII>());
    solve2();
}

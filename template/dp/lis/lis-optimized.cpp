#include <bits/stdc++.h>

int n;
std::vector<int> arr, seq;

template <typename C>
inline int lis(const C& cmp) {
    const auto rev_cmp = std::bind(cmp, std::placeholders::_2, std::placeholders::_1);
    seq.clear();
    for (const auto& v : arr) {
        if (seq.empty() || cmp(v, seq.back())) seq.push_back(v);
        else *std::lower_bound(seq.begin(), seq.end(), v, rev_cmp) = v;
    }
    return seq.size();
}

signed main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    arr.resize(n);
    for (auto& v : arr) std::cin >> v;
    std::cout << lis(std::greater<>()) << std::endl;
}

// {P1856}
#include <bits/stdc++.h>

struct SegmentTree {
    using tree_ptr = std::unique_ptr<SegmentTree>;

    int lb, rb;
    int key = 0, tag = 0;
    tree_ptr left = nullptr, right = nullptr;

    inline int middle(int l, int r) { return l + ((r - l) >> 1); }

    inline int middle() { return this->middle(lb, rb); }

    inline bool check(int l, int r) {
        return l <= r && this->rb >= l && this->lb <= r;
    }

    inline bool contains(int l, int r) {
        return l <= this->lb && r >= this->rb;
    }

    template <typename... A>
    inline static tree_ptr& allocate(tree_ptr& ptr, const A&... args) {
        if (ptr == nullptr) ptr = std::make_unique<SegmentTree>(args...);
        return ptr;
    }

    inline void allocate() {
        auto mid = this->middle();
        this->allocate(this->left, this->lb, mid);
        this->allocate(this->right, mid + 1, this->rb);
    }

    // specialized push_up function
    inline void push_up() {
        if (tag) this->key = this->rb + 1 - this->lb;
        else if (this->lb != this->rb) this->key = this->left->key + this->right->key;
        else this->key = 0;
    }

    // specialized update function
    inline void update(int k) {
        this->tag = this->tag + k;
        this->push_up();
    }

    // no need to push down

    SegmentTree(int l, int r) : lb(l), rb(r) {}

    void update(int l, int r, int k) {
        if (!this->check(l, r)) return;
        this->allocate();  // dynamic allocate
        if (this->contains(l, r)) return this->update(k);
        this->left->update(l, r, k), this->right->update(l, r, k);
        this->push_up();
    }
};

int n, min_pos, max_pos;
long long ans;
std::array<std::vector<std::tuple<int, int, int, int>>, 2> edges;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("scanning-perimeter.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    for (int xa, ya, xb, yb; n; n--) {
        std::cin >> xa >> ya >> xb >> yb;

        // horizontal
        edges[0].emplace_back(xa, -1, ya, yb);
        edges[0].emplace_back(xb, 1, ya, yb);

        // vertical
        edges[1].emplace_back(ya, -1, xa, xb);
        edges[1].emplace_back(yb, 1, xa, xb);

        const auto pos = {xa, ya, xb, yb};
        const auto& [min_ptr, max_ptr] = std::minmax_element(pos.begin(), pos.end());
        min_pos = std::min(min_pos, *min_ptr);
        max_pos = std::max(max_pos, *max_ptr);
    }
    for (auto& arr : edges) std::sort(arr.begin(), arr.end());

    SegmentTree tree(1, max_pos - min_pos + 1);
    for (const auto& arr : edges) {
        int tmp = 0;
        for (const auto& [_, k, l, r] : arr) {
            tree.update(l - min_pos + 1, r - min_pos, -k);
            ans += std::abs(tree.key - tmp);
            tmp = tree.key;
        }
    }
    std::cout << ans << std::endl;
}

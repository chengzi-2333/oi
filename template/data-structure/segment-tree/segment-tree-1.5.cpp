// {P13825}
#include <bits/stdc++.h>

template <typename T, typename R = size_t>
struct SegTree {
    using tree_ptr = std::unique_ptr<SegTree>;

    R l, r;
    T key = 0, tag = 0;
    tree_ptr left = nullptr, right = nullptr;

    inline static R middle(R l, R r) { return l + ((r - l) >> 1); }

    inline R middle() { return middle(this->l, this->r); }

    inline bool contains(R l, R r) { return this->l >= l && this->r <= r; }

    inline bool check(R l, R r) {
        return l <= r && l <= this->r && r >= this->l;
    }

    inline void update(T k) {
        this->key += k * static_cast<T>(this->r - this->l + 1);
        this->tag += k;
    }

    inline void push_up() { this->key = this->left->key + this->right->key; }

    inline void push_down() {
        this->allocate();
        if (this->tag) {
            this->left->update(this->tag);
            this->right->update(this->tag);
            this->tag = 0;
        }
    }

    inline static tree_ptr& allocate(tree_ptr& ptr, R l, R r) {
        if (ptr == nullptr) ptr = std::make_unique<SegTree>(l, r);
        return ptr;
    }

    inline void allocate() {
        auto mid = middle();
        allocate(this->left, this->l, mid);
        allocate(this->right, mid + 1, this->r);
    }

    SegTree(R l, R r) : l(l), r(r) {}

    void update(R l, R r, T k) {
        if (!check(l, r)) return;
        if (this->contains(l, r)) return this->update(k);
        this->push_down();
        this->left->update(l, r, k), this->right->update(l, r, k);
        this->push_up();
    }

    T query(R l, R r) {
        if (!check(l, r)) return 0;
        if (this->contains(l, r)) return this->key;
        this->push_down();
        return this->left->query(l, r) + this->right->query(l, r);
    }
};

unsigned long long n, m;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("segment-tree-1.5.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    SegTree<unsigned long long, unsigned long long> tree(1, n);
    for (unsigned long long op, l, r, k; m; m--) {
        std::cin >> op >> l >> r;
        if (op == 2) {
            std::cout << tree.query(l, r) + ((r - l + 1) * (r + l) / 2) << '\n';
        } else {
            std::cin >> k;
            tree.update(l, r, k);
        }
    }
}

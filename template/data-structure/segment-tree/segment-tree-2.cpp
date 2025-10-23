// {P3373}
#include <iostream>
#include <memory>


constexpr int N = 1e5;
long long n, q, m, a[N + 2];

template <typename T>
struct SegTree {
    int l, r;
    T key = 0, mul = 1, add = 0;
    std::unique_ptr<SegTree> left, right;

    inline static int middle(int l, int r) {
        return l + ((r - l) >> 1);
    }

    int middle() {
        return middle(this->l, this->r);
    }

    bool compare(int l, int r) {
        return this->l >= l && this->r <= r;
    }

    void modify_add(T k) {
        this->key = (this->key + k * (this->r - this->l + 1)) % m;
        this->add = (this->add + k) % m;
    }

    void modify_mul(T k) {
        this->key = (this->key * k) % m;
        this->add = (this->add * k) % m;
        this->mul = (this->mul * k) % m;
    }

    void modify(T add, T mul) {
        this->key = (this->key * mul + add * (this->r - this->l + 1)) % m;
        this->add = (this->add * mul + add) % m;
        this->mul = (this->mul * mul) % m;
    }

    void push_up() {
        this->key = (this->left->key + this->right->key) % m;
    }

    void push_down() {
        this->left->modify(this->add, this->mul);
        this->right->modify(this->add, this->mul);
        this->add = 0;
        this->mul = 1;
    }

    SegTree(int l, int r): l(l), r(r) {
        if (l != r) {
            auto mid = middle();
            this->left = std::make_unique<SegTree>(l, mid);
            this->right = std::make_unique<SegTree>(mid + 1, r);
            this->push_up();
        } else this->key = a[l];
    }

    void update_mul(int l, int r, T k) {
        if (compare(l, r)) return this->modify_mul(k);
        this->push_down();
        auto mid = middle();
        if (l <= mid) this->left->update_mul(l, r, k);
        if (r > mid) this->right->update_mul(l, r, k);
        this->push_up();
    }

    void update_add(int l, int r, T k) {
        if (compare(l, r)) return this->modify_add(k);
        this->push_down();
        auto mid = middle();
        if (l <= mid) this->left->update_add(l, r, k);
        if (r > mid) this->right->update_add(l, r, k);
        this->push_up();
    }

    T query(int l, int r) {
        if (l > r) return 0;
        if (compare(l, r)) return this->key;
        this->push_down();
        auto mid = middle();
        T res = 0;
        if (l <= mid) res = (res + this->left->query(l, r)) % m;
        if (r > mid) res = (res + this->right->query(l, r)) % m;
        return res;
    }
};


int main() {
#ifndef ONLINE_JUDGE
    freopen("segment-tree-2.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> q >> m;
    for (int i = 1; i <= n; i++) std::cin >> a[i];
    SegTree<long long> tree(1, n);
    for (long long op, l, r, k; q; q--) {
        std::cin >> op >> l >> r;
        switch (op) {
            case 1: 
                std::cin >> k;
                tree.update_mul(l, r, k); 
                break;
            case 2: 
                std::cin >> k;
                tree.update_add(l, r, k); 
                break;
            case 3: 
                std::cout << tree.query(l, r) << '\n'; 
                break;
        }
    }
}

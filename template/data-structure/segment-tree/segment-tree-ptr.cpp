// {P3372}
#include <iostream>
#include <memory>
#include <vector>

// T: element type, I: identity of the operation, F: operation
template <typename T, T I = T(), typename F = std::plus<T>>
class SegmentTree {
protected:
    F operate;
    T key = I, tag = I;
    size_t l, r;
    std::unique_ptr<SegmentTree> left, right;

    template <typename R>
    inline static R middle(const R& l, const R& r) {
        return l + ((r - l) >> 1);
    }

    inline size_t middle() const {
        return middle(this->l, this->r); // std::midpoint(this->l, this->r)
    }

    inline void modify(T k) {  // TODO: need to generalize
        this->key = operate(this->key, k * static_cast<T>(this->r - this->l + 1));
        this->tag = operate(this->tag, k);
    }

    inline void push_up() {
        this->key = operate(this->left->key, this->right->key);
    }

    inline void push_down() {
        if (this->tag != I) {
            this->left->modify(this->tag);
            this->right->modify(this->tag);
            this->tag = I;
        }
    }

public:
    SegmentTree(size_t l, size_t r, const std::vector<T>& arr = {}): l(l), r(r) {
        if (l != r) {
            auto mid = middle();
            this->left = std::make_unique<SegmentTree>(l, mid, arr);
            this->right = std::make_unique<SegmentTree>(mid + 1, r, arr);
            this->push_up();
        } else if (l < arr.size()) this->key = arr[l];
    }

    void update(size_t l, size_t r, T k) {
        if (this->l >= l && this->r <= r) {
            this->modify(k);
            return;
        }
        this->push_down();
        auto mid = middle();
        if (l <= mid) this->left->update(l, r, k);
        if (r > mid) this->right->update(l, r, k);
        this->push_up();
    }

    T query(size_t l, size_t r) {
        if (l > r) return I;
        if (this->l >= l && this->r <= r) return this->key;
        this->push_down();
        auto mid = middle();
        T res = I;
        if (l <= mid) res = operate(res, this->left->query(l, r));
        if (r > mid) res = operate(res, this->right->query(l, r));
        return res;
    }
};


long long n, m;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("segment-tree.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    std::vector<long long> a(n+1);
    for (auto it=a.begin()+1; it!=a.end(); it++) std::cin >> *it;
    SegmentTree<long long> tree(1, n, a);
    for (long long op, l, r, k; m; m--) {
        std::cin >> op;
        if (--op) {
            std::cin >> l >> r;
            std::cout << tree.query(l, r) << '\n';
        } else {
            std::cin >> l >> r >> k;
            tree.update(l, r, k);
        }
    }
    std::cout << std::flush;
}

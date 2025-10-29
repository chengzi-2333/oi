// {P3372}
#include <iostream>
#include <memory>
#include <vector>

// T: element type, I: identity of the operation, F: operation, R: range type
template <typename T, T I = T(), typename F = std::plus<T>, typename R = size_t>
class SegmentTree {
    using tree_ptr = std::unique_ptr<SegmentTree>;

   protected:
    F operate;
    T key = I, tag = I;
    R l, r;
    tree_ptr left, right;
    const std::vector<T>& arr;

    template <typename L>
    inline static L middle(const L& l, const L& r) {
        return l + ((r - l) >> 1);
    }

    inline R middle() const {
        return middle(this->l, this->r);  // std::midpoint(this->l, this->r)
    }

    inline bool contains(R l, R r) { return this->l >= l && this->r <= r; }

    inline bool check(R l, R r) {
        return l <= r && l <= this->r && r >= this->l;
    }

    inline tree_ptr& allocate(tree_ptr& ptr, R l, R r) {
        if (ptr == nullptr)
            ptr = std::make_unique<SegmentTree>(l, r, this->arr);
        return ptr;
    }

    inline void allocate() {
        auto mid = middle();
        this->allocate(this->left, this->l, mid);
        this->allocate(this->right, mid + 1, this->r);
    }

    inline void update(T k) {
        this->key =
            operate(this->key, k * static_cast<T>(this->r - this->l + 1));
        this->tag = operate(this->tag, k);
    }

    inline void assign(T k) {
        this->key = k * (this->r - this->l + 1);
        this->tag = k;
    }

    inline void push_up() {
        this->key = operate(this->left->key, this->right->key);
    }

    inline void push_down() {
        this->allocate();
        if (this->tag != I) {
            this->left->update(this->tag);
            this->right->update(this->tag);
            this->tag = I;
        }
    }

   public:
    SegmentTree(R l, R r, const std::vector<T>& arr = {}, bool dynamic = false)
        : l(l), r(r), arr(arr) {
        if (l != r && !dynamic) {
            this->allocate();
            this->push_up();
        } else if (l < arr.size()) {
            this->key = arr[l];
        }
    }

    void update(R l, R r, T k) {
        if (!this->check(l, r)) return;
        if (this->contains(l, r)) return this->update(k);
        this->push_down();
        this->left->update(l, r, k), this->right->update(l, r, k);
        this->push_up();
    }

    T query(R l, R r) {
        if (!this->check(l, r)) return I;
        if (this->contains(l, r)) return this->key;
        this->push_down();
        return operate(this->left->query(l, r), this->right->query(l, r));
    }

    void assign(R l, R r, T k) {
        if (!this->check(l, r)) return;
        if (this->contains(l, r)) return this->assign(k);
        this->push_down(true);
        this->left->assign(l, r, k), this->right->assign(l, r, k);
        this->push_up();
    }
};

long long n, m;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("segment-tree.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    std::vector<long long> a(n + 1);
    for (auto it = a.begin() + 1; it != a.end(); it++) std::cin >> *it;
    SegmentTree<long long> tree(1, n, a, true);
    for (long long op, l, r, k; m; m--) {
        std::cin >> op >> l >> r;
        if (--op)
            std::cout << tree.query(l, r) << '\n';
        else {
            std::cin >> k;
            tree.update(l, r, k);
        }
    }
    std::cout << std::flush;
}

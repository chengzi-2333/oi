#include <bits/stdc++.h>


template <typename T, typename R = int>
struct SegTree {
    using tree_ptr = std::unique_ptr<SegTree>;
    R l, r;
    T key = 0, tag = 0, mi = INT_MAX;
    tree_ptr left, right;
    const int* arr;

    inline R middle() {
        return (this->l + this->r) >> 1;
    }

    inline tree_ptr& allocate(tree_ptr& ptr, R l, R r) {
        return ptr ? ptr : ptr = std::make_unique<SegTree>(l, r, arr);
    }

    inline void allocate() {
        auto mid = middle();
        allocate(this->left, this->l, mid);
        allocate(this->right, mid + 1, this->r);
    }

    inline bool contains(R l, R r) {
        return this->l >= l && this->r <= r;
    }

    inline bool check(R l, R r) {
        return l <= r && l <= this->r && r >= this->l;
    }

    inline void push_up() {
        this->key = this->left->key + this->right->key;
        this->mi = std::min(this->left->mi, this->right->mi);
    }

    inline void update(T k) {
        this->key += k * (this->r - this->l + 1);
        this->tag += k;
    }

    inline void push_down() {
        this->allocate();
        if (this->tag) {
            this->left->update(this->tag);
            this->right->update(this->tag);
            this->tag = 0;
        }
    }

    SegTree(R l, R r, const int* arr): l(l), r(r), arr(arr) {
        if (l == r) this->key = arr[l];
    }

    void update(R l, R r, T k) {
        if (!this->check(l, r)) return;
        if (this->contains(l, r)) return this->update(k);
        this->push_down();
        this->left->update(l, r, k), this->right->update(l, r, k);
        this->push_up();
    }

    T query(R p) {
        // if (!this->check(l, r)) return 0;
        // if (this->contains(l, r)) return this->key;
        if (this->l == this->r) return this->key;
        this->push_down();
        return p <= middle() ? this->left->query(p) : this->right->query(p);
        // return this->left->query(l, r) + this->right->query(l, r);
    }

    T query_min(R l, R r) {
        if (!this->check(l, r)) return INT_MAX;
        if (this->contains(l, r)) return this->mi;
        // this->push_down();  // There's no need to push down.
        return std::min(this->left->query_min(l, r), this->right->query_min(l, r));
    }
};

constexpr int N = 2e5;

struct Query {
    int l, r, next;
} que[N + 2];
int head[N + 2];
int lst[N + 2], p[N + 2];
int ans[N + 2];
int n, m, target[N + 2];

int main() {
    freopen("table1.in", "r", stdin);
    // freopen("table.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        std::cin >> target[i];
        p[i] = lst[target[i]];
        lst[target[i]] = i;
    }
    for (int i = 1; i <= m; ++i) {
        std::cin >> que[i].l >> que[i].r;
        que[i].next = head[que[i].r];
        head[que[i].r] = i;
    }
    SegTree<int> tree(1, n, target);
    for (int i = 1; i <= n; i++) {
        // FIXME: 疑似错误题解：未检查区间是否为空
        bool flag = tree.query_min(p[i] + 1, i - 1) < target[i];
        tree.update((flag ? 0 : p[i]) + 1, i, 1);
        for (int j = head[i]; j; j = que[j].next) {
            ans[j] = tree.query(que[j].l);
        }
    }
    for (int i = 1; i <= m; i++) std::cout << ans[i] << '\n';
}

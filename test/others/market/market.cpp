#include <bits/stdc++.h>

#define int long long

struct SegTree {
    using tree_ptr = std::unique_ptr<SegTree>;
    int l, r;
    int key = 0, tag = 0;
    tree_ptr left = nullptr, right = nullptr;

    int middle() {
        return this->l + ((this->r - this->l) >> 1);
    }

    int check(int l, int r) {
        return l <= r && this->l <= r && this->r >= l;
    }

    int contains(int l, int r) {
        return this->l <= l && this->r >= r;
    }

    template <typename... Args>
    tree_ptr& allocate(tree_ptr& ptr, const Args&... args) {
        if (ptr == nullptr) ptr = std::make_unique<SegTree>(args...);
        return ptr;
    }

    void allocate() {
        auto mid = this->middle();
        this->allocate(this->left, this->l, mid);
        this->allocate(this->right, mid + 1, this->r);
    }

    void push_up() {
        this->key = std::max(this->left->key, this->right->key);
    }

    void update(int k) {
        this->key += k * (this->r - this->l + 1);
        this->tag += k;
    }

    void push_down() {
        this->allocate();
        if (this->tag) {
            this->left->update(this->tag);
            this->right->update(this->tag);
            this->tag = 0;
        }
    }
    
    SegTree(int l, int r) : l(l), r(r) {
        // if (l == r) this->key = arr[l];
        // else {
        //     this->allocate();
        //     this->push_up();
        // }
    }

    void update(int l, int r, int k) {
        if (!this->check(l, r)) return;
        if (this->contains(l, r)) return this->update(k);
        this->push_down();
        this->left->update(l, r, k), this->right->update(l, r, k);
        this->push_up();
    }

    int query(int l, int r) {
        if (!this->check(l, r)) return 0;
        if (this->contains(l, r)) return this->key;
        this->push_down();
        return std::max(this->left->query(l, r), this->right->query(l, r));
    }
};

int p, n, m;
std::vector<std::tuple<int, int, int>> groups;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("market1.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> p >> n >> m;
    groups.resize(p);
    for (auto& [l, r, c] : groups) std::cin >> l >> r >> c;
    std::sort(groups.begin(), groups.end());
    SegTree tree(1, n);
    int ans = 0;
    for (auto& [l, r, c] : groups) {
        // TODO
        auto num = std::min(m - tree.query(l, r), c);
        tree.update(l, r, num);
        ans += num;
    }
    std::cout << ans << std::endl;
}

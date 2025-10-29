// {P2146}
#include <bits/stdc++.h>

int n, t;
std::vector<int> fa, sz, dep, hson, top, dfn, rnk;
std::vector<std::vector<int>> g;

void build_tree(int u) {
    dep[u] = dep[fa[u]] + 1;
    sz[u] = 1;
    for (const auto& v : g[u]) {
        build_tree(v);
        sz[u] += sz[v];
        if (sz[v] > sz[hson[u]]) hson[u] = v;
    }
}

void cut_tree(int u, int ftop) {
    top[u] = ftop;
    dfn[u] = ++t;
    rnk[t] = u;
    if (hson[u]) cut_tree(hson[u], ftop);
    for (const auto& v : g[u]) {
        if (v != hson[u] && v != fa[u]) cut_tree(v, v);
    }
}

struct SegmentTree {
    using tree_ptr = std::unique_ptr<SegmentTree>;

    int l, r;
    int key = 0, tag = 0;
    tree_ptr left = nullptr, right = nullptr;

    inline static int middle(int l, int r) { return l + ((r - l) >> 1); }

    inline int middle() { return middle(this->l, this->r); }

    inline void allocate(tree_ptr& node, int l, int r) {
        if (node == nullptr) node = std::make_unique<SegmentTree>(l, r);
    }

    inline void allocate() {
        auto mid = middle();
        this->allocate(this->left, l, mid);
        this->allocate(this->right, mid + 1, r);
    }

    inline void push_up() { this->key = (this->left->key + this->right->key); }

    inline void update(int k) {
        this->key += k * (this->r - this->l + 1);
        this->tag += k;
    }

    inline void assign(int k) {
        this->key = k * (this->r - this->l + 1);
        this->tag = k;
    }

    inline void push_down(bool is_assign = false) {
        this->allocate();
        if (this->tag) {
            if (is_assign) {
                this->left->assign(this->tag);
                this->right->assign(this->tag);
            } else {
                this->left->update(this->tag);
                this->right->update(this->tag);
            }
            this->tag = 0;
        }
    }

    inline bool check(int l, int r) {
        return l <= r && this->l <= r && this->r >= l;
    }

    inline bool contains(int l, int r) { return this->l >= l && this->r <= r; }

    SegmentTree(int l, int r) : l(l), r(r) {}

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
        return this->left->query(l, r) + this->right->query(l, r);
    }

    void assign(int l, int r, int k) {
        if (!this->check(l, r)) return;
        if (this->contains(l, r)) return this->assign(k);
        this->push_down(true);
        this->left->assign(l, r, k), this->right->assign(l, r, k);
        this->push_up();
    }
};

std::unique_ptr<SegmentTree> tree;

int install(int u) {  // TODO
    auto p = u, s = 0;
    while (!(s = tree->query(dfn[top[p]], dfn[p])) && fa[top[p]]) p = fa[top[p]];
    p = rnk[dfn[top[p]] + s];
    tree->assign(dfn[p], dfn[u], 1);
    return dep[u] - dep[p] + 1;
}

int uninstall(int u) {  // TODO
    auto s = tree->query(dfn[u], dfn[u] + sz[u] - 1);
    tree->assign(dfn[u], dfn[u] + sz[u] - 1, 0);
    return s;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("package-manager1.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);

    std::cin >> n;
    g.resize(n + 1);
    fa.resize(n + 1), sz.resize(n + 1);
    dep.resize(n + 1), hson.resize(n + 1);
    top.resize(n + 1), dfn.resize(n + 1), rnk.resize(n + 1);
    for (int i = 2; i <= n; i++) {
        std::cin >> fa[i];
        g[++fa[i]].push_back(i);
    }
    
    build_tree(1), cut_tree(1, 1);
    tree = std::make_unique<SegmentTree>(1, n);

    int q, pkg;
    std::cin >> q;
    std::string op;
    while (q--) {
        std::cin >> op >> pkg;
        pkg++;
        std::cout << (op[0] == 'i' ? install(pkg) : uninstall(pkg)) << '\n';
    }
}

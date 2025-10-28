// {P3384}
#include <bits/stdc++.h>

#define int long long

int n, m, r, p;
std::vector<int> val;
std::vector<std::vector<int>> g;

int t;
std::vector<int> fa, dep, sz, hson;
std::vector<int> top, dfn, rnk;

void build_tree(int u, int f) {
    dep[u] = dep[f] + 1;
    fa[u] = f;
    sz[u] = 1;
    for (const auto& v : g[u]) {
        if (v == f) continue;
        build_tree(v, u);
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

    inline int middle(int l, int r) { return l + ((r - l) >> 1); }

    inline int middle() { return middle(this->l, this->r); }

    inline bool check(int l, int r) {
        return l <= r && l <= this->r && r >= this->l;
    }

    inline bool contains(int l, int r) { return this->l >= l && this->r <= r; }

    inline void allocate(tree_ptr& node, int l, int r) {
        if (node == nullptr) node = std::make_unique<SegmentTree>(l, r);
    }

    void allocate() {
        auto mid = middle();
        this->allocate(this->left, this->l, mid);
        this->allocate(this->right, mid + 1, this->r);
    }

    void modify(int k) {
        this->key += k * (this->r - this->l + 1);
        this->tag += k;
    }

    void push_up() { this->key = this->left->key + this->right->key; }

    void push_down() {
        // this->allocate();
        if (this->tag) {
            this->left->modify(this->tag);
            this->right->modify(this->tag);
            this->tag = 0;
        }
    }

    SegmentTree(int l, int r) : l(l), r(r) {
        if (l == r) this->key = val[rnk[l]];
        else this->allocate(), this->push_up();
    }

    void update(int l, int r, int k) {
        if (!this->check(l, r)) return;
        if (this->contains(l, r)) return this->modify(k);
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
};

signed main() {
#ifndef ONLINE_JUDGE
    freopen("hld.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m >> r >> p;

    val.resize(n + 1);
    for (int i = 1; i <= n; i++) std::cin >> val[i];

    g.resize(n + 1);
    for (int u, v, i = 1; i < n; i++) {
        std::cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }

    fa.resize(n + 1), dep.resize(n + 1);
    sz.resize(n + 1), hson.resize(n + 1);
    build_tree(r, 0);

    top.resize(n + 1), dfn.resize(n + 1), rnk.resize(n + 1);
    std::iota(top.begin(), top.end(), 0);
    cut_tree(r, r);

    SegmentTree tree(1, n);
    for (int op, u, v, k; m; m--) {
        std::cin >> op;
        switch (op) {
            case 1: {
                std::cin >> u >> v >> k;
                while (top[u] != top[v]) {
                    if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
                    tree.update(dfn[top[u]], dfn[u], k);
                    u = fa[top[u]];
                }
                if (dep[u] > dep[v]) std::swap(u, v);
                tree.update(dfn[u], dfn[v], k);
                break;
            }
            case 2: {
                std::cin >> u >> v;
                int tot = 0;
                while (top[u] != top[v]) {
                    if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
                    tot += tree.query(dfn[top[u]], dfn[u]);
                    u = fa[top[u]];
                }
                if (dep[u] > dep[v]) std::swap(u, v);
                tot += tree.query(dfn[u], dfn[v]);
                std::cout << tot % p << '\n';
                break;
            }
            case 3:
                std::cin >> u >> k;
                tree.update(dfn[u], dfn[u] + sz[u] - 1, k);
                break;
            case 4:
                std::cin >> u;
                std::cout << tree.query(dfn[u], dfn[u] + sz[u] - 1) % p << '\n';
                break;
        }
    }
}

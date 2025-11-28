// {P3372}
#include <bits/stdc++.h>

template <typename T, typename Func = std::plus<T>>
struct SegmentTree {
    using Iter = typename std::vector<T>::iterator;

    size_t n;
    std::vector<T> tree, tag;
    Func op;

    inline size_t lc(size_t p) { return p << 1; }
    inline size_t rc(size_t p) { return p << 1 | 1; }
    inline Iter lc(Iter p) { return tree.begin() + lc(p - tree.begin()); }
    inline Iter rc(Iter p) { return tree.begin() + rc(p - tree.begin()); }

    template <typename E>
    inline E middle(E l, E r) {
        return l + ((r - l) >> 1);
    }

    SegmentTree(size_t _n) {
        n = _n + 1;
        tree.resize(n * 4 + 4);
        tag.resize(n * 4 + 4);
    }

    void push_up(Iter p) { *p = op(*lc(p), *rc(p)); }

    void push_up(size_t p) { tree[p] = op(tree[lc(p)], tree[rc(p)]); }

    void build(Iter p, Iter s, Iter t) {
        if (s == t)
            *p = *s;
        else {
            auto mid = middle(s, t);
            build(lc(p), s, mid);
            build(rc(p), mid + 1, t);
            push_up(p);
        }
    }

    void build(Iter s, Iter t) { return build(tree.begin() + 1, s, t); }

    void update(size_t p, size_t l, size_t r, T k) {
        tag[p] += k;
        tree[p] += k * (r - l + 1);
    }

    void push_down(size_t p, size_t l, size_t r) {
        auto mid = middle(l, r);
        update(lc(p), l, mid, tag[p]);
        update(rc(p), mid + 1, r, tag[p]);
        tag[p] = 0;
    }

    void update(size_t s, size_t t, size_t l, size_t r, size_t p, T k) {
        if (s <= l && r <= t)
            update(p, l, r, k);
        else {
            push_down(p, l, r);
            auto mid = middle(l, r);
            if (s <= mid) update(s, t, l, mid, lc(p), k);
            if (t > mid) update(s, t, mid + 1, r, rc(p), k);
            push_up(p);
        }
    }

    void update(size_t s, size_t t, T k) { return update(s, t, 1, n, 1, k); }

    T query(size_t s, size_t t, size_t l, size_t r, size_t p) {
        if (s <= l && r <= t) return tree[p];
        T res = 0;
        auto mid = middle(l, r);
        push_down(p, l, r);
        if (s <= mid) res += query(s, t, l, mid, lc(p));
        if (t > mid) res += query(s, t, mid + 1, r, rc(p));
        return res;
    }

    T query(size_t s, size_t t) { return query(s, t, 1, n, 1); }
};

std::vector<std::size_t> a(1);
std::size_t n, m;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("segment-tree.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> m;
    std::copy_n(std::istream_iterator<std::size_t>(std::cin), n,
                std::back_insert_iterator<std::vector<std::size_t>>(a));
    SegmentTree<size_t> tree(n);
    tree.build(a.begin() + 1, a.end());
    for (size_t op, l, r, k; m; m--) {
        std::cin >> op;
        if (--op) {
            std::cin >> l >> r;
            std::cout << tree.query(l, r) << '\n';
        } else {
            std::cin >> l >> r >> k;
            tree.update(l, r, k);
        }
    }
}

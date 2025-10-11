// {P3378}
#include <bits/stdc++.h>

constexpr char lf = '\n';

template <typename T, typename C = std::less<T>>
class Heap {
private:
    C cmp;
    std::vector<T> h;

    inline std::size_t lc(std::size_t node) {return node << 1;}
    inline std::size_t rc(std::size_t node) {return node << 1 | 1;}
    inline std::size_t fa(std::size_t node) {return node >> 1;}

    void up(std::size_t node) {
        while (node > 1 && cmp(h[node], h[fa(node)])) {
            std::swap(h[node], h[fa(node)]);
            node = fa(node);
        }
    }

    void down(std::size_t node) {
        while (lc(node) <= h.size()-1) {
            std::size_t child = lc(node);
            if (rc(node) <= h.size()-1 && cmp(h[child], h[rc(node)]))
                child = rc(node);
            if (!cmp(h[child], h[node])) break;
            std::swap(h[node], h[child]);
            node = child;
        }
    }

public:
    Heap() {
        h.push_back(T{});
    }

    void push(T key) {
        h.push_back(key);
        up(h.size()-1);
    }

    T top() const {
        assert(h.size() > 1);
        return h[1];
    }

    void pop() {
        assert(h.size() > 1);
        h[1] = h.back();
        if (h.size() > 1) down(1);
        h.pop_back();
    }
};

signed main() {
#ifndef ONLINE_JUDGE
    freopen("heap.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n; std::cin >> n;
    Heap<int> heap;
    for (int opt, x; n; n--) {
        std::cin >> opt;
        switch (opt) {
            case 1:
                std::cin >> x;
                heap.push(x);
                break;
            case 2:
                std::cout << heap.top() << lf;
                break;
            case 3:
                heap.pop();
                break;
        }
    }
}

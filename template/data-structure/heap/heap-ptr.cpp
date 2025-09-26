#include <bits/stdc++.h>

constexpr char lf = '\n';

template <typename T>
struct Node {
    T key;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;

    Node(T _key, Node<T>* _parent)
        : key(_key), left(nullptr), right(nullptr), parent(_parent) {}

    static void free_node(Node<T>*& node) {
        if (node == nullptr) 
            return;
        free_node(node->left);
        free_node(node->right);
        node->parent = nullptr;
        delete node;
        node = nullptr;
    }
};

template <typename T, typename C = std::less<T>>
class Heap {
private:
    C cmp;
    Node<T>* root = nullptr;
    bool flag = false;

    Node<T>* choose_branch(Node<T>* node) {
        if (node == nullptr) return nullptr;
        if (node->left == nullptr) return node->right;
        if (node->right == nullptr) return node->left;
        return cmp(node->left->key, node->right->key) ? node->left : node->right;
    }

    Node<T>* push_up(Node<T>* node) {
        if (node != nullptr) {
            while (node->parent != nullptr && cmp(node->key, node->parent->key)) {
                std::swap(node->key, node->parent->key);
                node = node->parent;
            }
        }
        return node;
    }

    Node<T>* push_down(Node<T>* node) {
        if (node == nullptr)
            return nullptr;
        Node<T>* child = choose_branch(node);
        while (child != nullptr && cmp(child->key, node->key)) {
            std::swap(node->key, child->key);
            node = child;
            child = choose_branch(node);
        }
        return node;
    }

    Node<T>* get_last_node(Node<T>* node) {
        if (node == nullptr) return nullptr;
        std::queue<Node<T>*> q;
        q.push(node);
        Node<T>* last = node;
        while (!q.empty()) {
            last = q.front();
            q.pop();
            if (last->left != nullptr) q.push(last->left);
            if (last->right != nullptr) q.push(last->right);
        }
        return last;
    };

    void disconnect(Node<T>* node) {
        if (node == nullptr || node->parent == nullptr) return;
        
        if (node->parent->left == node) {
            node->parent->left = nullptr;
        } else if (node->parent->right == node) {
            node->parent->right = nullptr;
        }
        
        node->parent = nullptr;
        delete node;
    }

public:
    ~Heap() {
        Node<T>::free_node(root);
    }

    void insert(T key, Node<T>*& node) {
        if (node == nullptr) {
            node = new Node<T>(key, nullptr);
            return;
        }
        Node<T>* temp = node;
        if (flag) {
            while (temp->right != nullptr)
                temp = temp->right;
            temp->right = new Node<T>(key, temp);
            push_up(temp->right);
        } else {
            while (temp->left != nullptr)
                temp = temp->left;
            temp->left = new Node<T>(key, temp);
            push_up(temp->left);
        }
        flag = !flag;
    }

    void pop(Node<T>*& node) {
        if (node == nullptr)
            return;
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
            return;
        }
        Node<T>* last = get_last_node(node);
        if (last == nullptr) return;
        node->key = last->key;
        disconnect(last);
        push_down(node);
    }

    T top() {
        assert(root != nullptr);
        return root->key;
    }

    void pop() {
        return pop(root);
    }

    void push(T key) {
        return push(key, root);
    }
};

signed main() {
#ifndef ONLINE_JUDGE
    freopen("heap.in", "r", stdin);
#endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    Heap<int> heap;
    int n;
    std::cin >> n;
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

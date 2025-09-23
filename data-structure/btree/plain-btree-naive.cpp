#include <bits/stdc++.h>

constexpr char lf = '\n';

template<typename P>
void free_ptr(P*& ptr) {
    if (ptr != nullptr) {
        delete ptr;
        ptr = nullptr;
    }
}

template <typename T>
struct Node {
    T key;
    size_t t_size, count;
    Node<T>* left;
    Node<T>* right;

    Node(T _key)
        : key(_key), t_size(1), count(1), left(nullptr), right(nullptr) {}
    
    static void free_node(Node<T>*& node) {
        if (node == nullptr) return;
        free_node(node->left);
        free_node(node->right);
        delete node;
        node = nullptr;
    }
};

template <typename T, typename C = std::less<T>>
class BTree {
private:
    C cmp;
    Node<T>* root = nullptr;

public:
    ~BTree() {
        Node<T>::free_node(root);
    }

    size_t get_size(Node<T>* node) {
        return node ? node->t_size : 0;
    }

    void update_size(Node<T>* node) {
        if (node == nullptr) return;
        node->t_size = node->count + get_size(node->left) + get_size(node->right);
    }

    void travel(Node<T>* node) {
        if (node == nullptr)
            return;
        travel(node->left);
        std::cout << node->key << std::endl;
        travel(node->right);
    }
    
    Node<T>* find_node(T key, Node<T>* node) {
        if (node == nullptr)
        return nullptr;
        else if (cmp(key, node->key))
        return find_node(key, node->left);
        else if (cmp(node->key, key))
        return find_node(key, node->right);
        return node;
    }
    
    Node<T>* get_max(Node<T>* node) {
        if (node == nullptr) return nullptr;
        while (node->right)
        node = node->right;
        return node;
    }
    
    Node<T>* get_min(Node<T>* node) {
        if (node == nullptr) return nullptr;
        while (node->left)
        node = node->left;
        return node;
    }
    
    Node<T>* insert(T key, Node<T>*& node) {
        if (node == nullptr)
        return node = new Node<T>(key);
        else if (cmp(key, node->key))
        insert(key, node->left);
        else if (cmp(node->key, key))
        insert(key, node->right);
        else
        node->count++;
        update_size(node);
        return node;
    }
    
    Node<T>* erase(T key, Node<T>*& node) {
        if (node == nullptr)
        return node;
        else if (cmp(key, node->key))
        node->left = erase(key, node->left);
        else if (cmp(node->key, key))
        node->right = erase(key, node->right);
        else if (node->count == 1) {
            if (node->left == nullptr) {
                Node<T>* temp = node->right;
                delete node;
                node = temp;
            } else if (node->right == nullptr) {
                Node<T>* temp = node->left;
                delete node;
                node = temp;
            } else {
                Node<T>* temp = get_max(node->left);
                node->key = temp->key;
                node->count = temp->count;
                temp->count = 1;
                node->left = erase(temp->key, node->left);
            }
        } else
        node->count--;
        update_size(node);
        return node;
    }
    
    size_t get_size(T key, Node<T>* node) {
        node = find_node(key);
        if (node == nullptr) return 0;
        return node->t_size;
    }
    
    size_t rank(T key, Node<T>* node) {
        if (node == nullptr)
        return 1;
        else if (cmp(key, node->key))
        return rank(key, node->left);
        else if (cmp(node->key, key))
        return get_size(node->left) + node->count + rank(key, node->right);
        return get_size(node->left) + 1;
    }
    
    T kth(size_t k, Node<T>* node) {
        if (node == nullptr)
        return T();
        size_t left_size = get_size(node->left);
        if (k <= left_size)
        return kth(k, node->left);
        else if (k <= left_size + node->count)
        return node->key;
        return kth(k - get_size(node->left) - node->count, node->right);
    }
    
    T lower(T key, Node<T>* node) {
        if (node == nullptr)
        return T();
        if (cmp(node->key, key)) {
            T right = lower(key, node->right);
            return (right != T() && cmp(node->key, right)) ? right : node->key;
        } else
        return lower(key, node->left);
    }
    
    T upper(T key, Node<T>* node) {
        if (node == nullptr)
        return T();
        if (cmp(key, node->key)) {
            T left = upper(key, node->left);
            return (left != T() && cmp(left, node->key)) ? left : node->key;
        } else
        return upper(key, node->right);
    }
    

    void travel() {
        std::cout << "BTree Size: " << root->t_size << std::endl;
        return travel(root);
    }

    Node<T>* find_node(T key) {
        return find_node(key, root);
    }
    
    Node<T>* insert(T key) { return insert(key, root); }
    
    Node<T>* erase(T key) { return erase(key, root); }
    
    size_t get_size(T key) {
        return get_size(key, root);
    }
    
    size_t rank(T key) {
        return rank(key, root);
    }
    
    T kth(size_t k) {
        return kth(k, root);
    }
    
    T lower(T key) {
        return lower(key, root);
    }

    T upper(T key) {
        return upper(key, root);
    }
};

signed main() {
    #ifndef ONLINE_JUDGE
    freopen("plain-btree.in", "r", stdin);
    #endif
    std::cin.tie(nullptr)->sync_with_stdio(false);
    BTree<int> tree;
    int n;
    std::cin >> n;
    for (int opt, x; n; n--) {
        std::cin >> opt >> x;
        switch (opt) {
            case 1:
                tree.insert(x);
                break;
            case 2:
                tree.erase(x);
                break;
            case 3:
                std::cout << tree.rank(x) << lf;
                break;
            case 4:
                std::cout << tree.kth(x) << lf;
                break;
            case 5:
                std::cout << tree.lower(x) << lf;
                break;
            case 6:
                std::cout << tree.upper(x) << lf;
                break;
        }
    }
    std::cout << std::flush;
}

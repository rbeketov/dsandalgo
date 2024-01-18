// Дано число N < 10^6 и последовательность целых чисел 
// из [-2^31..2^31] длиной N.
// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, 
// если root→Key ≤ K, то узел
// K добавляется в правое поддерево root; иначе в левое поддерево root.
// Требования: Рекурсия запрещена. Решение должно поддерживать 
// передачу функции сравнения снаружи.

// 2_4. Выведите элементы в порядке level-order (по слоям, “в ширину”).


#include <iostream>
#include <sstream>
#include <cassert>
#include <functional>
#include <stack>
#include <queue>
#include <utility>


template<class T>
struct Compare {
    bool operator()(const T &lhs, const T &rhs) {
        return lhs < rhs;
    };
};

template<class T, class C = Compare<T>>
class BinaryTree {

public:
    struct Node {
        T data;
        Node *left = nullptr;
        Node *right = nullptr;
    };

    explicit BinaryTree(C comparator = C()) : _root(nullptr), _compare(comparator) {};
    ~BinaryTree() {
        traversePostOrder([](Node *node) { delete node; });
    }
    BinaryTree(const BinaryTree &rhs) = delete;
    BinaryTree &operator=(const BinaryTree &rhs) = delete;
    BinaryTree(BinaryTree &&rhs) = delete;
    BinaryTree &operator=(BinaryTree &&rhs) = delete;

    void push(const T &data) {
        auto ptrToCurrent = &_root;
        auto current = _root;
        while (current) {
            if (_compare(current->data, data)) {
                ptrToCurrent = &current->right;
                current = current->right;
            } else {
                ptrToCurrent = &current->left;
                current = current->left;
            }
        }
        *ptrToCurrent = new Node{data, nullptr, nullptr};
    }

    void traverseBFS(const std::function<void(Node *)> &callback) {
        if (!_root) {
            return;
        }
    
        std::queue<Node*> que;

        que.push({_root});
        while (!que.empty()) {
            Node* node = que.front();
            que.pop();
            callback(node);

            if (node->left) {
                que.push({node->left});
            }
            if (node->right) {
                que.push({node->right});
            }
        }

    }

    void traversePostOrder(const std::function<void(Node *)> &callback) {
        if (!_root) {
            return;
        }
        std::stack<Node *> firstStack;
        std::stack<Node *> secondStack;
        firstStack.push(_root);
        Node *current = nullptr;
        while (!firstStack.empty()) {
            current = firstStack.top();
            firstStack.pop();
            if (current->left) { 
                firstStack.push(current->left);
            }
            if (current->right) {
                firstStack.push(current->right);
            }
            secondStack.push(current);
        }
        while (!secondStack.empty()) {
            callback(secondStack.top());
            secondStack.pop();
        }
    }
private:
    Node *_root;
    C _compare;


};


int main() {
    BinaryTree<int> bst;
    int value = 0;
    int sizeVal = 0;
    std::cin >> sizeVal;
    for (int i = 0; i < sizeVal; ++i) {
        std::cin >> value;
        bst.push(value);
    }

    bst.traverseBFS([](BinaryTree<int>::Node* v){std::cout << v->data << " ";});
    return 0;
}
#include <iostream>
#include <sstream>
#include <cassert>
#include <functional>
#include <stack>


template<class T>
struct Compare {
    bool operator()(const T &lhs, const T &rhs) {
        return lhs < rhs;
    };
};

template<class T, class C = Compare<T>>
class BinaryTree {
private:
    struct Node {
        T data;
        Node *left = nullptr;
        Node *right = nullptr;
    };

    Node *_root;
    C _compare;

public:
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

    bool checkSimilarPreOrder() {
        if (!_root) {
            return true;
        }
        std::stack<Node *> stack;
        T comp_elemetn = _root->data;
        stack.push(_root);
        Node *current = nullptr;
        while (!stack.empty()) {
            current = stack.top();
            stack.pop();
            if (current->right) { stack.push(current->right); }
            if (current->left) { stack.push(current->left); }
            if (current->data != comp_elemetn) {
                return false;
            }
        }
        return true;
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

};


int main() {
    BinaryTree<int> bst;
    int value = 0;
    while(std::cin >> value) {
        bst.push(value);
    }

    std::cout << bst.checkSimilarPreOrder() << std::endl;
    return 0;
}
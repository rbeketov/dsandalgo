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

    int minDeep() {
        if (!_root) {
            return 0;
        }
    
        std::queue< std::pair<Node*, int> > que;

        que.push({_root, 1});
        while( !que.empty() ) {
            std::pair<Node*, int> marker = que.front();
            if (!marker.first->left && !marker.first->right) {
                return marker.second;
            }
            que.pop();
            if (marker.first->left) {
                que.push({marker.first->left, marker.second + 1});
            }

            if (marker.first->right) {
                que.push({marker.first->right, marker.second + 1});
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

};


int main() {
    BinaryTree<int> bst;
    int value = 0;
    while(std::cin >> value) {
        bst.push(value);
    }

    std::cout << bst.minDeep() << std::endl;
    return 0;
}
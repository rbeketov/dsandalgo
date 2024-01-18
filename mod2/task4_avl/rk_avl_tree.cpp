#include <iostream>
#include <string>


template<class T>
struct DefaultComparator {
    int operator()(const T& l, const T& r) {
        if (l < r) { return -1; }
        if (l > r) { return 1; }
        return 0;
    }
};


template <class T, class Comparator = DefaultComparator<T> >
class AVLTree {
 public:
    
    AVLTree(Comparator comp = Comparator()) : root(nullptr), items_count(0), comp_(comp) {}

    ~AVLTree() { delete root;}
    AVLTree(const AVLTree&) = delete;
    AVLTree(AVLTree&&) = delete;
    AVLTree& operator= (const AVLTree&) = delete;
    AVLTree& operator= (AVLTree&&) = delete;

    size_t size() const { return items_count; }

    T* find(const T& value) {
        return findAux(root, value);
    }

    void insert(const T& value) {
        root = insertAux(root, value);
    }


    void erase(const T& value) {
        root = eraseAux(root, value);
    }

    T* next(const T& value) {
        Node* curr = root;
        Node* result = nullptr;
        while (curr) {
            if (comp_(curr->value_, value) == 1) {
                result = curr;
                curr = curr->left_;
            } else { 
                curr = curr->right_;
            }
        }
        return ((result != nullptr) ? &result->value_ : nullptr);
    }

    T* prev(const T& value) {
        Node* curr = root;
        Node* result = nullptr;
        while (curr) {
            if (comp_(curr->value_, value) == -1) {
                result = curr;
                curr = curr->right_;
            } else { 
                curr = curr->left_;
            }
        }
        return ((result != nullptr) ? &result->value_ : nullptr);
    }

 private:
    struct Node;

    T* findAux(Node* node, const T& value) {
        if (!node) {
            return nullptr;
        }

        int result_compare = comp_(value, node->value_);

        if (result_compare == -1) {
            return findAux(node->left_, value);
        } else if (result_compare == 1) {
            return findAux(node->right_, value);
        }
        return &node->value_;
    }

    Node* insertAux(Node* node, const T& value) {
        if (!node) {
            items_count++;
            return new Node(value);
        }

        int result_compare = comp_(value, node->value_);

        if (result_compare == -1) {
            node->left_ = insertAux(node->left_, value);
        } else if (result_compare == 1) {
            node->right_ = insertAux(node->right_, value);
        }

        return balance(node);
    }

    Node* eraseAux(Node* node, const T& value) {
        if (!node) {
            return nullptr;
        }

        int result_compare = comp_(value, node->value_);

        if (result_compare == -1) {
            node->left_ = eraseAux(node->left_, value);
        } else if (result_compare == 1) {
            node->right_ = eraseAux(node->right_, value);
        } else {
            Node* left = node->left_;
            Node* right = node->right_;

    
            delete node;
            items_count--;
            if (!right) {
                return left;
            }

            Node* min_node;
            right = removeMinNodeAndExtract(right, min_node);
            min_node->right_ = right;
            min_node->left_ = left;
            
            return balance(min_node);
        }

        return balance(node);


    }

    Node* findMin(Node* node) {
        if (!node->left_) {
            return node;
        }
        return findMin(node->left_);
    }

    Node* removeMinNodeAndExtract(Node* node, Node*& find_min) {
        if (!node->left_) {
            find_min = node;
            return node->right_;
        }

        node->left_ = removeMinNodeAndExtract(node->left_, find_min);
        
        return balance(node);
        
    }


    uint8_t height(Node* node) {
        if (!node) {
            return 0;
        }
        return node->height_;
    }

    void fixHeight(Node* node) {
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
    }

    int bfacotr(Node* node) {
        return height(node->right_) - height(node->left_);
    }

    Node* balance(Node* node) {
        fixHeight(node);

        int bf = bfacotr(node);
        if (bf == 2) {
            if (bfacotr(node->right_) < 0) {
                node->right_ = rotateRight(node->right_);
            }
            return rotateLeft(node);
        }

        if (bf == -2) {
            if (bfacotr(node->left_) > 0) {
                node->left_ = rotateLeft(node->left_);
            }
            return rotateRight(node);
        }

        return node;
    }


    Node* rotateLeft(Node* node) {
        Node* tmp = node->right_;
        node->right_ = tmp->left_;
        tmp->left_ = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }
    Node* rotateRight(Node* node) {
        Node* tmp = node->left_;
        node->left_ = tmp->right_;
        tmp->right_ = node;
        fixHeight(node);
        fixHeight(tmp);
        return tmp;
    }

    struct Node {
     public:
        Node(const T& value)
            : left_(nullptr), right_(nullptr), value_(value), height_(1) {}

        Node* left_;
        Node* right_;
        T value_;
        uint8_t height_;
    };

    Node* root;
    size_t items_count;
    Comparator comp_;


};




int main() {
    AVLTree<int> avl_tree;
    std::string command;
    int value = 0;
    while(std::cin >> command >> value) {
        if (command == "insert") {
            avl_tree.insert(value);
        } else if (command == "delete") {
            avl_tree.erase(value);
        } else if (command == "exists") {
            if (avl_tree.find(value)) {
                std::cout << "true" << std::endl;
            } else {
                std::cout << "false" << std::endl;
            }
        } else if (command == "next") {
            int* res = avl_tree.next(value);
            if (res) {
                std::cout << *res << std::endl; 
            } else {
                std::cout << "none" << std::endl;
            }
        } else if (command == "prev") {
            int* res = avl_tree.prev(value);
            if (res) {
                std::cout << *res << std::endl; 
            } else {
                std::cout << "none" << std::endl;
            }
        }
    }


}
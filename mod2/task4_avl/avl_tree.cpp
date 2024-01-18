// 4_2. Порядковые статистики. 
// Дано число N и N строк.
// Каждая строка содержит команду добавления или удаления натуральных чисел,
// а также запрос на получение k-ой порядковой статистики.
// Команда добавления числа A задается положительным числом A,
// команда удаления числа A задается отрицательным числом “-A”.
// Запрос на получение k-ой порядковой статистики задается числом k. 
// Требования: скорость выполнения запроса - O(log n).

#include <iostream>

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

    T* findKthElem(const size_t k) {
        return findKthElemAux(root, k);
    }

    void insert(const T& value) {
        root = insertAux(root, value);
    }


    void erase(const T& value) {
        root = eraseAux(root, value);
    }


 private:
    struct Node;

    T* findKthElemAux(Node* node, const size_t k) {
        if (!node) {
            return nullptr;
        }
        
        if (k < cnt(node->left_)) {
            return findKthElemAux(node->left_, k);
        }
        if (k > cnt(node->left_)) {
            return findKthElemAux(node->right_, k - 1 - cnt(node->left_));
        }
        
        return &node->value_;
    }
    

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
        node->elem_count--;
        
        return balance(node);
        
    }


    uint8_t height(Node* node) {
        if (!node) {
            return 0;
        }
        return node->height_;
    }

    size_t cnt(Node* node) {
        if (!node) {
            return 0;
        }
        return node->elem_count;
    }

    void fixHeight(Node* node) {
        node->height_ = std::max(height(node->left_), height(node->right_)) + 1;
    }

    void fixCnt(Node* node) {
         node->elem_count = cnt(node->left_) + cnt(node->right_) + 1;
    }

    void rightFixCnt(Node* node) {
        node->elem_count -= (cnt(node->left_->left_) + 1);
        node->left_->elem_count += cnt(node->right_) + 1;
    }

    void leftFixCnt(Node* node) {
        node->elem_count -= (cnt(node->right_->right_) + 1);
        node->right_->elem_count += cnt(node->left_) + 1;

    }

    int bfacotr(Node* node) {
        return height(node->right_) - height(node->left_);
    }

    Node* balance(Node* node) {
        fixHeight(node);
        fixCnt(node);

        int bf = bfacotr(node);
        if (bf == 2) {
            if (bfacotr(node->right_) < 0) {
                rightFixCnt(node->right_);
                node->right_ = rotateRight(node->right_);
            }
            leftFixCnt(node);
            return rotateLeft(node);
        }

        if (bf == -2) {
            if (bfacotr(node->left_) > 0) {
                leftFixCnt(node->left_);
                node->left_ = rotateLeft(node->left_);
            }
            rightFixCnt(node);
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
            : left_(nullptr), right_(nullptr), value_(value), height_(1), elem_count(1) {}

        Node* left_;
        Node* right_;
        T value_;
        uint8_t height_;
        size_t elem_count;
    };

    Node* root;
    size_t items_count;
    Comparator comp_;


};


void test() {
    AVLTree<int> avl_tree{};
    for (int i = 0; i < 10; i++) {
        avl_tree.insert(i);
    }

    for (int i = 4; i < 7; i++) {
        avl_tree.erase(i);
    }

    std::cout << *avl_tree.findKthElem(6) << std::endl;


}

void solution() {
    AVLTree<int> avl_tree{};
    size_t cnt_strings = 0;
    int first_arg = 0;
    int second_arg = 0;
    std::cin >> cnt_strings;
    for (size_t i = 0; i < cnt_strings; ++i) {
        std::cin >> first_arg >> second_arg;
        if (first_arg > 0) {
            avl_tree.insert(first_arg);
            std::cout << *avl_tree.findKthElem(second_arg) << std::endl;
        } else {
            avl_tree.erase(-first_arg);
            std::cout << *avl_tree.findKthElem(second_arg) << std::endl;
        }
    }
}


int main() {
    solution();
    return 0;
}

// Задача 4_1. Условие: В одной военной части решили построить в одну шеренгу по росту. Т
// Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
// Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие.
// За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.
// Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.
// Требуемая скорость выполнения команды - O(log n).

#include <iostream>
#include <stack>
#include <vector>

template<class T>
class CompareDefault {
 public:
    bool operator()(const T &l, const T &r) {
        return l < r;
    };
};

template<class T, class Compare>
class BinaryTree {
 public:
    BinaryTree(const Compare &_cmp) : root(nullptr), cmp(_cmp) {};
    ~BinaryTree();

    void Add(const T &key, size_t &position);

    T NextMin(const T &key);
    T NextMax(const T &key);
    void Remove(const T &key);
 private:
    struct node {
        T key;
        size_t childs;
        unsigned char height;
        node *left;
        node *right;

        node(T k) : key(k), left(0), right(0), height(1), childs(1) {}
    };

    node *root;
    Compare cmp;

    void deleteNode(node *node);

    unsigned char height(node *p) {
        return p ? p->height : 0;
    }

    int bfactor(node *p) {
        return height(p->right) - height(p->left);
    }

    size_t childs(node *p) {
        if (p) {
            return p->childs;
        }
        return 0;
    }

    void fixChilds(node *p) {
        int hl = childs(p->left);
        int hr = childs(p->right);
        p->childs = hl + hr + 1;
    }

    void fixheight(node *p) {
        unsigned char hl = height(p->left);
        unsigned char hr = height(p->right);
        p->height = std::max(hl, hr) + 1;
    }

    node *rotateright(node *p) {
        node *q = p->left;
        p->left = q->right;
        q->right = p;
        fixChilds(p);
        fixChilds(q);
        fixheight(p);
        fixheight(q);
        return q;
    }

    node *rotateleft(node *q) {
        node *p = q->right;
        q->right = p->left;
        p->left = q;
        fixChilds(q);
        fixChilds(p);
        fixheight(q);
        fixheight(p);
        return p;
    }

    node *balance(node *p) {
        fixheight(p);
        if (bfactor(p) == 2) {
            if (bfactor(p->right) < 0)
                p->right = rotateright(p->right);
            return rotateleft(p);
        }
        if (bfactor(p) == -2) {
            if (bfactor(p->left) > 0)
                p->left = rotateleft(p->left);
            return rotateright(p);
        }
        return p;
    }

    node *insert(node *p, T value, size_t &position) {
        if (!p) {
            return new node(value);
        }
        ++(p->childs);
        if (cmp(value, p->key)) {
            position += childs(p->right) + 1;
            p->left = insert(p->left, value, position);
        } else
            p->right = insert(p->right, value, position);
        return balance(p);
    }

    node *findmin(node *p) {
        return p->left ? findmin(p->left) : p;
    }

    node *removemin_and_insert(node** min_element, node *p) {
        if (!p->left) {
            *min_element = p;
            return p->right;
        }
        p->left = removemin_and_insert(min_element,p->left);
        --p->childs;

        return balance(p);
    }


    node *remove(node *root, const T &position) {
        if (!root) {
            return nullptr;
        }
        if (!cmp(position, root->childs)) {
            return root;
        }

        size_t steps = 0;
        std::stack<node *> path;


        while (true) {
            const size_t right_childs = childs(root->right);
            if (cmp(right_childs, position - steps)) {
                path.push(root);
                root = root->left;
                steps += right_childs + 1;
            } else if (cmp(position - steps, right_childs)) {
                if (root->right) {
                    path.push(root);
                    root = root->right;
                } else {
                    break;
                }
            } else {                // ==
                node *left = root->left;
                node *right = root->right;
                T key = root->key;

                delete root;
                --root->childs;

                if (!right) {
                    if (!left) {
                        if (!path.empty()) {
                            return nullptr;
                        } else {
                            root = path.top();
                            path.pop();
                            if (root->key > key) {
                                root->left = nullptr;
                            } else {
                                root->right = nullptr;
                            }
                            --root->childs;
                        }
                    } else {
                        root = left;
                    }
                } else {
                    node *min;

                    right = removemin_and_insert(&min, right);
                    min->right = right;
                    min->left = left;
                    fixChilds(min);
                    root = balance(min);
                }
                break;
            }
        }

        while (!path.empty()) {
            node *node = path.top();
            --node->childs;

            if (node->key > root->key) {
                node->left = root;
            } else {
                node->right = root;
            }

            root = balance(node);
            path.pop();
        }

        return root;
    }
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree<T, Compare>() {
    deleteNode(root);
};

template<class T, class Compare>
void BinaryTree<T, Compare>::deleteNode(node *node) {
    if (node) {
        deleteNode(node->left);
        deleteNode(node->right);
        delete node;
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::Add(const T &key, size_t &position) {
    root = insert(root, key, position);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::Remove(const T &key) {
    root = remove(root, key);
}

void AVLProcessing(size_t n, std::vector<int> &out) {
    size_t operation = 0;
    int value = 0;
    CompareDefault<int> cmp;
    BinaryTree<int, CompareDefault<int>> tree(cmp);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> operation >> value;
        size_t position = 0;
        if (operation == 1) {
            tree.Add(value, position);
            out.push_back(position);
        } else {
            tree.Remove(value);
        }
    }
}

int main() {
    size_t n = 0;
    std::cin >> n;
    std::vector<int> out;
    AVLProcessing(n, out);
    for (auto i : out) {
        std::cout << i << " ";
    }
}
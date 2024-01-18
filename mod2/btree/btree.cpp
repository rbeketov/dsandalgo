#include <vector>
#include <iostream>
#include <queue>
#include <cassert>

template <typename Key>
class BTree
{
public:
    struct Node
    {
        Node(bool leaf)
        : leaf(leaf) {}
 
        ~Node() {
            for (Node* child: children) {
                delete child;
            }
        }
 
        bool leaf;
        std::vector<Key> keys; // 2t-1 , t-1, в корне от 1 до 2t-1
        std::vector<Node*> children;
    };
 
    BTree(size_t min_degree)
    : root(nullptr), t(min_degree)
    {
        assert(min_degree >= 2);
    }
 
    ~BTree()
    {
        if (root)
            delete root;
    }
 
    void insert(const Key &key)
    {
        if (!root)
            root = new Node(true);
        
        if (is_node_full(root))
        {
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            split(root, 0);
        }
 
        insert_non_full(root, key);
    }
 
    void print_tree() {
        if (!root) {
            return;
        }
    
        std::queue<Node*> que;

        que.push(root);
        while (!que.empty()) {
            Node* node = que.front();
            que.pop();
            for (auto it : node->keys) {
                std::cout << it << " ";
            }
            
            if (!node->leaf) {
                for (auto it : node->children) {
                    que.push(it);
                }
            }
        }
        std::cout << std::endl;
    }

    bool find(const Key &key)
    {
        return find_aux(root, key);
    }
 
private:
    bool is_node_full(Node *node)
    {
        return node->keys.size() == 2*t - 1;
    }
 
    bool find_aux(Node* node, const Key &key)
    {
        int i = 0;
        while (i < node->keys.size() && key > node->keys[i])
            i++;
 
        if (i < node->keys.size() && key == node->keys[i])
            return true;
        else if (node->leaf)
            return false;
        else
            return find_aux(node->children[i], key);
    }
 
    void split(Node *node, size_t index) {
        Node* left_node = new Node(node->children[index]->leaf);
        Node* right_node = new Node(node->children[index]->leaf);


        for (size_t i = 0; i < t; ++i) {
            left_node->keys.push_back(node->children[index]->keys[i]);
            right_node->keys.push_back(node->children[index]->keys[i + 1 + t]);
        }
        if (!node->children[index]->leaf) {
            for (size_t i = 0; i < t - 1; ++i) {
                left_node->children.push_back(node->children[index]->children[i]);
                right_node->children.push_back(node->children[index]->children[i + t]);
            }
        }
    
        node->keys.resize(node->keys.size() + 1);
        node->children.resize(node->children.size() + 1);

        for (size_t i = node->keys.size()-1; i > index; --i) {
            node->keys[i] = node->keys[i-1];
            node->children[i] = node->children[i-1];
        }
        node->keys[index] = node->children[index]->keys[t];
        
        delete node->children[index];

        node->children[index] = left_node;
        node->children[index+1] = right_node;
    }
 
 
    void insert_non_full(Node *node, const Key &key)
    {
        int pos = node->keys.size() - 1;
        if (node->leaf)
        {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && key < node->keys[pos])
            {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        }
        else
        {
            while (pos >= 0 && key < node->keys[pos])
            {
                pos--;
            }
 
            if (is_node_full(node->children[pos + 1]))
            {
                split(node, pos + 1);
                if (key > node->keys[pos + 1])
                {
                    pos++;
                }
            }
            insert_non_full(node->children[pos + 1], key);
        }
    }
 
 
    Node *root;
    size_t t; // minimum degree
  
};

void solution() {
    int weight_layer = 0;
 
    std::cin >> weight_layer;
    BTree<int> tree(weight_layer);

    int num = 0;
    while(std::cin >> num) {
        tree.insert(num);
    }

    tree.print_tree();
}


int main() {
    solution();
    return 0;
}
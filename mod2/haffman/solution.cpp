#include "Huffman.h"

#include <set>
#include <unordered_map>
#include <algorithm>
#include <cmath>


class BitReader {
public:
    BitReader(IInputStream& in_stream) : in(in_stream), buf(0), pos(0) {
    }

    bool read_bit(byte& value) {
        if (pos == 0) {
            bool res = in.Read(buf);
            if (!res) {
                return false;
            }
        }

        value = (buf >> (7 - pos++)) & 1;
        if (pos == 8) {
            pos = 0;
        }
        return true;
    }
private:
    IInputStream& in;
    byte buf;
    int pos;

};


class BitWriter {
public:
    BitWriter(IOutputStream& out_stream) : out(out_stream), buf(0), pos(0) {
    }

    void write_bit(byte value) {
        value &= 1;

        buf = buf | (value << (7 - pos++));
        if (pos == 8) {
            flush();
        }
    }

    void flush() {
        if (pos > 0) {
            out.Write(buf);
            buf = 0;
            pos = 0;
        }
    }
private:
    IOutputStream& out;
    byte buf;
    byte pos;
};


namespace haffman {

struct Node {
public:
    Node(size_t freq, byte val, Node* left_, Node* right_)
        : frequency(freq), value(val), left(left_), right(right_) {}
    
    bool operator==(const Node& other) const {
        return frequency == other.frequency;
    }
    bool operator<(const Node& other) const {
        return frequency < other.frequency;
    }

    size_t frequency;
    byte value;
    Node* left;
    Node* right;

};

struct NodePtrLess{
    bool operator()(const Node* l, const Node* r) const {
        return l->frequency < r->frequency;
    }
};

using huffTreeType = std::multiset<Node*, NodePtrLess>;

void preOrderCreateTable(Node* node, std::unordered_map<byte, std::vector<byte> >& code_table, std::vector<byte> curr_bytes);
void preOrderHuffmanToCode(Node* node, std::vector<byte>& result);
void decodeHaffmanTreeAux(Node* node, size_t& it, int& curr_cnt, int cnt_symbol, const std::vector<byte>& input_vec);
byte codeToChar(size_t& it, const std::vector<byte>& input_vec);
int toDec(const std::vector<byte>& buffer);
std::vector<byte> readInputCompressed(IInputStream& compressed);
std::vector<byte> readInput(IInputStream& input);
std::unordered_map<byte, size_t> countFrequency(const std::vector<byte>& input);
huffTreeType createHuffmanTree(const std::unordered_map<byte, size_t>& table_freq);
std::unordered_map<byte, std::vector<byte> > createCodeTable(const huffTreeType& huff_tree, size_t size_cnt);
std::vector<byte> haffmanTreeToVec(const huffTreeType& huff_tree);
std::vector<byte> decodeMainMessage(size_t& it, Node* root, const std::vector<byte>& input_vec, size_t upper_bound);
Node* decodeHaffmanTree(size_t& it, int cnt_symbol, const std::vector<byte>& input_vec);

template <class T>
std::vector<byte> toBinary(const T value);
std::vector<byte> writeOtputVec(const std::vector<byte>& input_vec, std::unordered_map<byte, std::vector<byte> >& code_table);


void inorder(Node* node) {
    if (!node) {
        return;
    }
    
    inorder(node->left);

    if (!node->left && !node->right) {
        std::cout << node->value << std::endl;
    }

    inorder(node->right);

}



} // namespace haffman;

void Encode(IInputStream& original, IOutputStream& compressed) {
	auto input_vector = haffman::readInput(original);

    auto cnt_table = haffman::countFrequency(input_vector);
    auto huffman_tree = haffman::createHuffmanTree(cnt_table);
    auto cnt_char = cnt_table.size();
  
    auto code_table = haffman::createCodeTable(huffman_tree, cnt_char);

    auto output_vector = haffman::writeOtputVec(input_vector, code_table);
    
    auto haffman_tree_vec = haffman::haffmanTreeToVec(huffman_tree);
    
    auto uneven_bits = 3 + output_vector.size() + (haffman_tree_vec.size() - cnt_char);
    
    auto left_bit = (8 - (uneven_bits % 8)) % 8;

    auto binary_left_bit = haffman::toBinary(left_bit);
    auto binary_cnt_char = haffman::toBinary(cnt_char);

    BitWriter bw(compressed);
    for (int i = 0; i < 8 - binary_cnt_char.size(); ++i) {
        bw.write_bit(0);
    }
    for (auto it : binary_cnt_char) {
        bw.write_bit(it);
    }

    for (int i = 0; i < 3 - binary_left_bit.size(); ++i) {
        bw.write_bit(0);
    }
    
    for (auto it : binary_left_bit) {
        bw.write_bit(it);
    }

    bool flag_zero_find = false;
    for (auto it : haffman_tree_vec) {
        if (flag_zero_find == false) {
            bw.write_bit(it);
            if (it == 0) {
                flag_zero_find = true;
            }
        } else {
            auto char_binary = haffman::toBinary((int)it);
            for (int i = 0; i < 8 - char_binary.size(); ++i) {
                bw.write_bit(0);
            }
            for ( auto jt : char_binary ) {
                bw.write_bit(jt);
            }
            flag_zero_find = false;
        }
    }
    for (auto it : output_vector) {
        bw.write_bit(it);
    }
    bw.flush();

}


void Decode(IInputStream& compressed, IOutputStream& original) { 
    auto input_vec = haffman::readInputCompressed(compressed);
 
    size_t it = 0;
    std::vector<byte> buffer;

    for (; it < 8; ++it) {
        buffer.push_back(input_vec[it]);
    }
    int cnt_symbol = haffman::toDec(buffer);
  
    buffer.clear();
    for (; it < 11; ++it) {
        buffer.push_back(input_vec[it]);
    }
    int cnt_less_bit = haffman::toDec(buffer);

    haffman::Node* root_haffman_tree = haffman::decodeHaffmanTree(it, cnt_symbol, input_vec);
    size_t upper_bound = input_vec.size() - cnt_less_bit;
    auto vector_output = haffman::decodeMainMessage(it, root_haffman_tree, input_vec, upper_bound);



    for ( auto it : vector_output) {
        original.Write(it);
    }

}

namespace haffman {
std::vector<byte> readInput(IInputStream& input) {
    std::vector<byte> input_vector;
    byte value;
    while (input.Read(value)) { 
        input_vector.push_back(value);
	}
    return input_vector;
}


std::unordered_map<byte, size_t> countFrequency(const std::vector<byte>& input) {
    std::unordered_map<byte, size_t> counter_freq;
	for (auto it : input) { 
        ++counter_freq[it];
	}
    return counter_freq;
}



std::unordered_map<byte, std::vector<byte> > createCodeTable(const huffTreeType& huff_tree, size_t size_cnt) {
    std::unordered_map<byte, std::vector<byte> > code_table;
    std::vector<byte> code;
    if (size_cnt == 1) {
        code.push_back(1);
        code_table[(*huff_tree.begin())->value] = code;
        return code_table;
    }
    preOrderCreateTable(*huff_tree.begin(), code_table, code);
    return code_table;
}

void preOrderCreateTable(Node* node, std::unordered_map<byte, std::vector<byte> >& code_table, std::vector<byte> curr_bytes) {
    if (!node) {
        return;
    }
    if (!node->left && !node->right) {
        code_table[node->value] = curr_bytes;
        return;
    }
    auto curr_left = curr_bytes;
    auto curr_right = curr_bytes;
    curr_left.push_back(0);
    preOrderCreateTable(node->left, code_table, curr_left);

    curr_right.push_back(1);
    preOrderCreateTable(node->right, code_table, curr_right);

}


huffTreeType createHuffmanTree(const std::unordered_map<byte, size_t>& table_freq) {
    huffTreeType huffman_tree;
    for (auto [symb, freq] : table_freq) {
        huffman_tree.insert(new Node{freq, symb, nullptr, nullptr});
    }
    while (huffman_tree.size() != 1) {
        Node* left_value = huffman_tree.extract(huffman_tree.begin()).value();
        Node* right_value = huffman_tree.extract(huffman_tree.begin()).value();
        huffman_tree.insert(new Node{left_value->frequency + right_value->frequency,
                             static_cast<byte>(left_value->value + right_value->value),
                             left_value,
                             right_value});
    }
    return huffman_tree;
}


std::vector<byte> writeOtputVec(const std::vector<byte>& input_vec, std::unordered_map<byte, std::vector<byte> >& code_table) {
    std::vector<byte> output_vec;
    for (auto i : input_vec) {
        for(auto j : code_table[i]) {
            output_vec.push_back(j);
        }
    }
    return output_vec;

}

template <class T>
std::vector<byte> toBinary(const T value) {
    int curr = static_cast<int>(value);
    std::vector<byte> result;
    while (curr > 0) {
        result.push_back(static_cast<byte>(curr & 1));
        curr = curr >> 1;
    }
    if (result.size() == 0) {
        result.push_back(0);
    }
    std::reverse(result.begin(), result.end());
    return result;
}


std::vector<byte> haffmanTreeToVec(const huffTreeType& huff_tree) {
    std::vector<byte> code;
    preOrderHuffmanToCode(*huff_tree.begin(), code);
    return code;

}

void preOrderHuffmanToCode(Node* node, std::vector<byte>& result) {

    if (!node->left && !node->right) {
        result.push_back(0);
        result.push_back(node->value);
        return;
    }
    result.push_back(1);
    preOrderHuffmanToCode(node->left, result);

    result.push_back(1);
    preOrderHuffmanToCode(node->right,result);

}


std::vector<byte> readInputCompressed(IInputStream& compressed) {
    std::vector<byte> result;
    BitReader br(compressed);
    byte value = 0;
    while (br.read_bit(value)) {
        result.push_back(value);
    }
    return result;

}

int toDec(const std::vector<byte>& buffer) {
    int result = 0;
    for (int i = 0; i < buffer.size(); ++i) {
        int byte_ = static_cast<int>(buffer[buffer.size() - 1 - i]);
        result += static_cast<int>(byte_ * std::pow(2, i));
    }
    return result;
}



Node* decodeHaffmanTree(size_t& it, int cnt_symbol, const std::vector<byte>& input_vec) {
    Node* root = new Node(1, '&', nullptr, nullptr);
    int curr_cnt_symbol = 0;
    decodeHaffmanTreeAux(root, it, curr_cnt_symbol, cnt_symbol, input_vec);
    return root;
}

void decodeHaffmanTreeAux(Node* node, size_t& it, int& curr_cnt, int cnt_symbol, const std::vector<byte>& input_vec) {
    if (curr_cnt == cnt_symbol) {
        return;
    }
    if (input_vec[it] == 0) {
        ++it;
        node->value = codeToChar(it, input_vec);
        ++curr_cnt;
        return;
    }
    ++it;
    node->left = new Node(1, '&', nullptr, nullptr);
    decodeHaffmanTreeAux(node->left, it, curr_cnt, cnt_symbol, input_vec);
    ++it;
    node->right = new Node(1, '&', nullptr, nullptr);
    decodeHaffmanTreeAux(node->right, it, curr_cnt, cnt_symbol, input_vec);
}

byte codeToChar(size_t& it, const std::vector<byte>& input_vec) {
    std::vector<byte> buffer;
    size_t upper_bound = it+8;
    for (; it < upper_bound; ++it) {
        buffer.push_back(input_vec[it]);
    }
    byte symbol = static_cast<byte>(toDec(buffer));
    return symbol;
};

std::vector<byte> decodeMainMessage(size_t& it, Node* root, const std::vector<byte>& input_vec, size_t upper_bound) {
    std::vector<byte> result;
    
    Node* curr = root;
    for (; it < upper_bound; ++it) {
        if (input_vec[it] == 1 && curr->right) {
            curr = curr->right;
        } else if (input_vec[it] == 0 && curr->left) {
            curr = curr->left;
        }
        if (!curr->right && !curr->left) {
            result.push_back(curr->value);
            curr = root;
        }

    }
    return result;
}

} // namespace haffman

// int main() {
//     std::vector<byte> vec{'a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a','a'};
//     for ( auto it : vec ) {
//         std::cout << it;
//     }
//     std::cout << std::endl;
//     VectorInput vi(&vec);

//     std::vector<byte> vec_output;
//     VectorOutput vo(&vec_output);

//     Encode(vi, vo);

//     //std::cout << vec_output.size() << std::endl;
    
//     // for (auto it : vec_output) {
//     //     std::cout << (byte)it;
//     // }
//     // std::cout << std::endl;

//     VectorInput vir(&vec_output);
//     std::vector<byte> vec_decode;
//     VectorOutput vor(&vec_decode);
    
//     Decode(vir, vor);
    
//     //std::cout << vec_decode.size()  << std::endl;

//     for (auto it : vec_decode) {
//         std::cout << it;
//     }
//     std::cout << std::endl;
//     return 0;
// }
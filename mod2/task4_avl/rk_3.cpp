#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <ctime>


static constexpr size_t CONST_A_PRIME = 13;

size_t strHash(const std::string& value) {
    size_t hash = 0;
    for (char i : value) {
        hash = hash * CONST_A_PRIME + i;
    }
    return hash;
}




template<class Key> struct Hash;

template<> struct Hash<std::string> {
    size_t operator()(const std::string& value) const {
        return strHash(value);
    }
};

static constexpr size_t INITIAL_SIZE = 8;

static constexpr double LOAD_BOUND = 0.75;

template< class Key, class Hasher = Hash<Key> >
class HashSet {
 public:    
    HashSet(Hasher hasher = Hasher()) : hasher(hasher), buckets(nullptr), sizeBuckets(0), currSizeBuckets(0) {}
    ~HashSet() {
        delete[] buckets;
    };
    HashSet(const HashSet&) = delete;
    HashSet(HashSet&&) = delete;
    HashSet& operator=(const HashSet&) = delete;
    HashSet& operator=(HashSet&&) = delete;


    Key* find(const Key& key) {
        if (!buckets) {
            return nullptr;
        }
        size_t hash = hasher(key);
        size_t bucketIdx = hash % sizeBuckets;

        int cntIter = 0;
        while (cntIter < sizeBuckets) {
            if (!buckets[bucketIdx].flagEmpty && buckets[bucketIdx].valueKey == key) { // Нашли ключ
                return &buckets[bucketIdx].valueKey;
            }
            if (buckets[bucketIdx].flagEmpty && !buckets[bucketIdx].flagErase) { // Нашли пустую ячейку, ERASE не было
                return nullptr;
            }
            bucketIdx = (bucketIdx + cntIter) % sizeBuckets; // квадратичное пробирование 
            cntIter++;
        }
        return nullptr;
    }

    bool insert(const Key& key) {

        if (currSizeBuckets  >= LOAD_BOUND * sizeBuckets) {
            grow();
        }

        size_t hash = hasher(key);
        size_t bucketIdx = hash % sizeBuckets;

        size_t cntIter = 0;
        Bucket* findFirstEmpty = nullptr;
        while (cntIter < sizeBuckets) {
            if (!buckets[bucketIdx].flagEmpty && buckets[bucketIdx].valueKey == key) { // Нашли ключ
                return false;
            }
            if (buckets[bucketIdx].flagEmpty && !buckets[bucketIdx].flagErase) { // Нашли пустую ячейку, ERASE не было
                // Перевод в состояние KEY
                buckets[bucketIdx].valueKey = key;
                buckets[bucketIdx].flagEmpty = false;
                buckets[bucketIdx].flagErase = false;
                currSizeBuckets++;
                return true;
            }
            if (buckets[bucketIdx].flagEmpty && buckets[bucketIdx].flagErase && !findFirstEmpty) { // Нашли пустую ячейку, ERASE было
                findFirstEmpty = &buckets[bucketIdx];
            }
            bucketIdx = (bucketIdx + cntIter) % sizeBuckets; // квадратичное пробирование 
            cntIter++;
        }
        if (!findFirstEmpty) {
            throw std::runtime_error("Error in insert");
        }
        // Перевод в состояние KEY
        findFirstEmpty->valueKey = key;
        findFirstEmpty->flagEmpty = false;
        findFirstEmpty->flagErase = false;
        currSizeBuckets++;
        return true;
    }

    bool erase(const Key& key) {
        if (!buckets) {
            return false;
        }
        
        size_t hash = hasher(key);
        size_t bucketIdx = hash % sizeBuckets;

        size_t cntIter = 0;
        while (cntIter < sizeBuckets) {
            
            if (!buckets[bucketIdx].flagEmpty && buckets[bucketIdx].valueKey == key) { // Нашли ключ
                // Перевод в состояние ERASE
                buckets[bucketIdx].flagEmpty = true;
                buckets[bucketIdx].flagErase = true;
                currSizeBuckets--;
                return true;
            }
            if (buckets[bucketIdx].flagEmpty && !buckets[bucketIdx].flagErase) { // Нашли пустую ячейку, ERASE не было
                return false;
            }
            bucketIdx = (bucketIdx + cntIter) % sizeBuckets; // квадратичное пробирование 
            cntIter++;
        }
        return false;
    }



 private:
    void grow() {
        size_t newBucketsSize = std::max(INITIAL_SIZE, sizeBuckets * 2);
        Bucket* newBuckets = new Bucket[newBucketsSize];

        for (size_t i = 0; i < sizeBuckets; ++i) {
            if (!buckets[i].flagEmpty) {
                size_t hash = hasher(buckets[i].valueKey);
                size_t newBucketIdx = hash % newBucketsSize;
                
                size_t cntIter = 0;
                while (cntIter < newBucketsSize) { // Нашли пустую ячейку, ERASE не было, т.к. новая таблица
                    if (newBuckets[newBucketIdx].flagEmpty) {
                        newBuckets[newBucketIdx].valueKey = buckets[i].valueKey;
                        newBuckets[newBucketIdx].flagEmpty = false;
                        break;
                    }
                    newBucketIdx = (newBucketIdx + cntIter) % newBucketsSize; // квадратичное пробирование 
                    cntIter++;
                }
                if (cntIter == newBucketsSize) {
                    throw std::runtime_error("Error in grow");
                }
                
            }
        }

        sizeBuckets = newBucketsSize;
        delete[] buckets;
        buckets = newBuckets;
    }

    
    struct Bucket {
     public:
        Bucket() : flagEmpty(true), flagErase(false), valueKey() {} // Состояние EMPTY
    
        bool flagEmpty;
        bool flagErase;
        Key valueKey;
    };

    Hasher hasher;
    Bucket* buckets;
    size_t sizeBuckets;
    size_t currSizeBuckets;

};

enum operators {
    INSERT = '+',
    ERASE = '-',
    FIND = '?',
};




struct Node {
public:
    Node() {}
    Node(const std::string& pn, int c, int s) : product_name(pn), color(c), size(s) {}
    std::string product_name;
    int color;
    int size;

    bool operator==(const Node& v) const {
        return (product_name == v.product_name && color == v.color && size == v.size);
    }
};

template<> struct Hash<Node> {
    size_t operator()(const Node& v) const {
        return strHash(v.product_name) + v.color + v.size;
    }
};



void solution() {
   HashSet<Node> set;

    char operator_ = '\0';
    std::string p_name;
    int color_ = 0;
    int size_ = 0;
    while (std::cin >> operator_ >> p_name >> color_ >> size_) {
        Node node(p_name, color_, size_);
        switch (operator_) {
            case INSERT: {
                std::cout << (set.insert(node) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case ERASE: {
                std::cout << (set.erase(node) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case FIND: {
                std::cout << (set.find(node) ? "OK" : "FAIL") << std::endl;
                break;
            }
            default:
                break;
        }
    }
}


int main() {
 
    solution();
 

    return 0;
}
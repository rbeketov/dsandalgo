// Реализуйте структуру данных типа “множество строк” на
// основе динамической хеш-таблицы с открытой адресацией. 
// Хранимые строки непустые и состоят из строчных латинских букв.
// Хеш-функция строки должна быть реализована с помощью вычисления
// значения многочлена методом Горнера.
// Начальный размер таблицы должен быть равным 8-ми.
// Перехеширование выполняйте при добавлении элементов в случае,
// когда коэффициент заполнения таблицы достигает 3/4.
// Структура данных должна поддерживать операции добавления строки
// в множество, удаления строки из множества и проверки принадлежности
// данной строки множеству.
// 1_1. Для разрешения коллизий используйте квадратичное пробирование.
// i-ая проба
// g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.

#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <ctime>


// 1 состояние: flagEmty = false and flagErase = false - bucket KEY
// 2 состояние: flagEmpty = true and flagErase = false  - bucket EMPTY
// 3 cостояние flagEmty = true and flagErase = true - bucket ERASE



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
        Bucket() : flagEmpty(true), flagErase(false) {} // Состояние EMPTY
    
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



void solution() {
   HashSet<std::string> set;

    char operator_ = '\0';
    std::string value;
    while (std::cin >> operator_ >> value) {
        switch (operator_) {
            case INSERT: {
                std::cout << (set.insert(value) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case ERASE: {
                std::cout << (set.erase(value) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case FIND: {
                std::cout << (set.find(value) ? "OK" : "FAIL") << std::endl;
                break;
            }
            default:
                break;
        }
    }
}

void tests() {
    HashSet<std::string> set;
    std::string str = "a";
    std::string tmp = "";
    for (size_t i = 0; i < 100; i++) {
        tmp += str;
        set.insert(tmp);
    }


    tmp = "aaaaaa";
    for (size_t i = 0; i < 20; i++) {
        tmp += str;
        set.find(tmp);
    }

    tmp = "";
    for (size_t i = 0; i < 100; i++) {
        tmp += str;
        set.erase(tmp);
    }
}

void tests2() {
    HashSet<std::string> set;
    set.insert("test1");
    set.insert("testtest2");
    set.insert("testtesttest3");

    set.find("test1");
}

void test3() {
    HashSet<std::string> set;
    for (size_t i = 1; i < 20000; ++i) {
        set.insert(std::to_string(i));
    }
    for (size_t i = 1; i < 20000; ++i) {
        set.find(std::to_string(i));
    }
    for (size_t i = 1; i < 20000; ++i) {
        set.erase(std::to_string(i));
    }
}

void test4() {
    HashSet<std::string> set;
    char operator_ = '\0';
    std::string value;
    std::ifstream fs("006.uu");
    while (fs >> operator_ >> value) {
        switch (operator_) {
            case INSERT: {
                std::cout << (set.insert(value) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case ERASE: {
                std::cout << (set.erase(value) ? "OK" : "FAIL") << std::endl;
                break;
            }
            case FIND: {
                std::cout << (set.find(value) ? "OK" : "FAIL") << std::endl;
                break;
            }
            default:
                break;
        }
    }
}

int main() {
 
    solution();
    // unsigned int start_time = clock();
    // test4();
    // unsigned int end_time = clock();
    // std::cout << "time: " << (end_time - start_time) / CLOCKS_PER_SEC << std::endl;

    return 0;
}
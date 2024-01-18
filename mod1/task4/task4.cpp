// 4.1 Слияние массивов.
// Напишите программу, которая использует кучу для слияния K отсортированных 
// массивов суммарной длиной N.
// Требования: время работы O(N * logK). Ограничение на размер кучи O(K)..
// Формат входных данных: Сначала вводится количество массивов K. 
// Затем по очереди размер каждого массива и элементы массива. 
// Каждый массив упорядочен по возрастанию.
// Формат выходных данных: Итоговый отсортированный массив.

#include <cassert>
#include <iostream>
#include <functional>


template<typename T>
class DefaultComparator;

namespace heap {

static const int DefaultInitialSize = 1;

template<typename T>
class DinArray {
 public:
    DinArray() : buffer( 0 ), bufferSize( 0 ), realSize( 0 ) {}
    DinArray(const DinArray& array) {
        for (int i = 0; i < array.size(); ++i) {
            add(array[i]);
        }
    }
    ~DinArray() { 
        delete[] buffer;
    }

    int size() const { 
        return realSize; 
    }

    T& getValue(int index) const {
        assert(0 <= index && index < realSize && buffer != 0);
        return buffer[index];
    }

    T operator[](int index) const {
        return getValue(index);
    }
    T& operator[](int index) {
        return getValue(index);
    }

    T& last() {
        return getValue(size()-1);
    }

    bool isEmpty() {
        return realSize == 0;
    }

    void deleteLast() {
        assert(!isEmpty());
        realSize--;
    }

    void add(T element) {
        if (realSize == bufferSize) {
            grow();
        }
        assert(realSize < bufferSize && buffer != 0 );
        buffer[realSize++] = element;
    }



 private:
    T* buffer;
    int bufferSize;
    int realSize;

    void grow() {
        int newBufferSize = std::max(bufferSize * 2, DefaultInitialSize);
        T* newBuffer = new T[newBufferSize];

        for( int i = 0; i < realSize; ++i) {
            newBuffer[i] = buffer[i];
        }
        delete[] buffer;
        buffer = newBuffer;
        bufferSize = newBufferSize;
    }
};


template<typename T, typename Comparator = DefaultComparator<T> >
class Heap {
 public:
    Heap(Comparator comp = Comparator()) : _comp(comp) {}
    Heap(const DinArray<T>& _arr, Comparator comp) : _comp(comp) {
        arr(_arr);
        buildHeap();
    }
    ~Heap() {}

    void insert(T element) {
        arr.add(element);
        siftUp(arr.size() - 1);
    }

    T pop() {
        assert(!arr.isEmpty());
        T tmp = arr[0];
        arr[0] = arr.last();

        arr.deleteLast();

        if (!arr.isEmpty()) {
            siftDown(0);
        }
        return tmp;
    }

    const T& top() {
        assert(!arr.isEmpty());
        return arr[0];
    }

    bool isEmpty() {
        return arr.isEmpty();
    }

 private:
    DinArray<T> arr;
    Comparator _comp;

    void siftDown(int index) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        int smallest = index;
        if (left < arr.size() && _comp(arr[left], arr[index])) {
            smallest = left;
        }
        
        if (right < arr.size() && _comp(arr[right], arr[smallest])) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(arr[index], arr[smallest]);
            siftDown(smallest);
        }
    }

    void siftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (!_comp(arr[index], arr[parent])) {
                return;
            }
            std::swap(arr[index], arr[parent]);
            index = parent;
        }
    }

    void buildHeap() {
        for (int i = arr.size() / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
    }

};

}  // namespace heap

template<typename T>
class DefaultComparator {
 public:
    bool operator()(const T& l, const T& r) const {
        return l < r;
    }
};

template<typename T>
struct Descriptor {
    T* _array;
    int _size;
    int curr;
};




void mergeArray() {
    unsigned countArrays = 0;
    std::cin >> countArrays;

    heap::Heap<Descriptor<int>, std::function<bool(Descriptor<int>, Descriptor<int>)> > heap([](Descriptor<int> l, Descriptor<int> r) {
                                                                                                    return l._array[l.curr] < r._array[r.curr];
                                                                                                });

    int** arrays = new int*[countArrays];

    for (unsigned i = 0; i < countArrays; ++i) {
        unsigned sizeArray = 0;
        std::cin >> sizeArray;
        arrays[i] = new int[sizeArray];
        for (unsigned j = 0; j < sizeArray; ++j) {
            std::cin >> arrays[i][j];
        }
        Descriptor<int> tmp;
        tmp._array = arrays[i];
        tmp.curr = 0;
        tmp._size = sizeArray;

        heap.insert(std::move(tmp));
    }

    while (!heap.isEmpty()) {
        auto minDescr = heap.pop();
        std::cout << minDescr._array[minDescr.curr] << " ";
        minDescr.curr++;
        if (minDescr.curr != minDescr._size) {
            heap.insert(std::move(minDescr));
        }
    }

    for (unsigned i = 0; i < countArrays; ++i) {
        delete arrays[i];
    }
    delete[] arrays;

}


int main() {

    mergeArray();
    return 0;
}

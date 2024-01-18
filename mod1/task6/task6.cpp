// 6_1. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
// Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

#include <iostream>
#include <vector>


template<class T, class Comparator>
size_t getMedian(T* array, size_t left, size_t right, Comparator comp) {
    size_t first = left;
    size_t last = right-1;
    size_t mid = left + (right - left) / 2;
 
    T max_ = array[first];
    size_t maxIndex = first;
    T min_ = array[first];
    size_t minIndex = first;

    if (comp(array[mid], max_)) {
        max_ = array[mid];
        maxIndex = mid;
    }
    if (comp(array[last], max_)) {
        max_ = array[last];
        maxIndex = last;
    }

    if (!comp(array[mid], min_)) {
        min_ = array[mid];
        minIndex = mid;
    }
    if (!comp(array[last], min_)) {
        min_ = array[last];
        minIndex = last;
    }
    size_t result = static_cast<size_t>(first + last + mid - minIndex - maxIndex);
    return result;
}




template<class T, class Comparator>
size_t partitionFromStartToEnd(T* array, size_t left, size_t right, Comparator comp) {
    size_t pivotIdx = getMedian(array, left, right, comp);
    T pivot = array[pivotIdx];

    std::swap(array[pivotIdx], array[right-1]);

    size_t i = left;
    size_t j = left;
    while (j < right-1) {
        if (comp(array[j], pivot)) { // array[j] > pivot
            j++;
        } else if (i != j) {
            std::swap(array[i], array[j]);
            i++;
            j++;
        } else {
            i++;
            j++;
        }
    }
    std::swap(array[i], array[right-1]);
    return i;
}

template<class T, class Comparator>
void kthElement(T* array, size_t size, size_t k, Comparator comp) {
    size_t left = 0;
    size_t right = size;
    while (left < right) {
        size_t returnIndex = partitionFromStartToEnd(array, left, right, comp);
        if (returnIndex == k) {
            return;
        } else if (returnIndex < k) {
            left = returnIndex + 1;
        } else {
            right = returnIndex;
        }
    }
}


int main() {
    size_t sizeArr = 0;
    std::cin >> sizeArr;
    int* array = new int[sizeArr];
    for (size_t i = 0; i < sizeArr; ++i) {
        std::cin >> array[i];
    }

    auto isLess = [](const int l, const int r) { return l > r; };
    
    std::vector<size_t> values = {static_cast<size_t>(sizeArr*0.1), static_cast<size_t>(sizeArr*0.5), static_cast<size_t>(sizeArr*0.9)};

    for (auto it : values) {
        kthElement(array, sizeArr, it, isLess);
        std::cout << array[it] << std::endl;
    }

    
    delete[] array;
    return 0;
}
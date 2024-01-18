// 2_4. Дан отсортированный массив различных целых чисел A[0..n-1] 
// и массив целых чисел B[0..m-1]. Для каждого элемента массива B[i] 
// найдите минимальный индекс элемента массива A[k], ближайшего по значению к B[i].
// Требования: Время работы поиска для каждого элемента B[i]: O(log(k)). 
// Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон
// для бинарного поиска размером порядка k с помощью экспоненциального поиска, 
// а потом уже в нем делать бинарный поиск.

#include <iostream>

int* createArrayFromCin(unsigned sizeArray) {
    int* array = new int[sizeArray];
    for (unsigned i = 0; i < sizeArray; ++i) {
        std::cin >> array[i];
    }
    return array;
}

// вывод массива
void printArray(int* array, unsigned sizeArray) {
    for (unsigned i = 0; i < sizeArray-1; ++i) {
        std::cout << array[i] << " ";
    }
}

// exp поиск
unsigned expSearch(const int* array, unsigned sizeArray, int element) {
    unsigned pointer = 2;
    while (pointer < sizeArray && array[pointer] < element) {
        pointer *= 2;
    }
    return pointer;
}

unsigned binSearch(const int* array, unsigned count, int element) {
    unsigned first = 0;
    unsigned last = count;
    while (first < last) {
        unsigned middle = (first + last) / 2;
        if (array[middle] < element) {
            first = middle + 1;
        } else {
            last = middle;
        }
    }
    return first == count ? first - 1: first;
}


void mainLogic() {
    unsigned sizeArraySource = 0;
    std::cin >> sizeArraySource;
    int* arraySource = createArrayFromCin(sizeArraySource);
    unsigned sizeArraySearch = 0;
    std::cin >> sizeArraySearch;
    int* arraySearch = createArrayFromCin(sizeArraySearch);

    int* resultArray = new int[sizeArraySearch];

    for (unsigned i = 0; i < sizeArraySearch; ++i) {
        unsigned tmpPosEnd = expSearch(arraySource, sizeArraySource, arraySearch[i]);
        //std::cout << "tmpPosEnd: " << tmpPosEnd << std::endl;
        unsigned end =  (tmpPosEnd < sizeArraySource ? tmpPosEnd + 1 : sizeArraySource);
        unsigned begin = (tmpPosEnd == 2 ? 0 : tmpPosEnd / 2);
        unsigned position = binSearch(arraySource+begin, end-begin, arraySearch[i]);
        position += begin;
        //std::cout << "middle: " << position << std::endl;
        if (position == 0) {
            resultArray[i] = position;
        } else if (arraySearch[i] - arraySource[position-1] <= arraySource[position] - arraySearch[i]) {  
            resultArray[i] = position - 1;
        } else {
            resultArray[i] = position;
        }
    }

    printArray(resultArray, sizeArraySearch);

    delete[] resultArray;
    delete[] arraySource;
    delete[] arraySearch;
}

int main() {
    
    mainLogic();
    
    return 0;
}

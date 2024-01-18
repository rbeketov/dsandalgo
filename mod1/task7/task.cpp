// 7_2. LSD для long long.
// Дан массив неотрицательных целых 64-битных чисел. 
// Количество чисел не больше 10^6. 
// Отсортировать массив методом поразрядной сортировки LSD по байтам.

#include <iostream>
#include <cmath>

const int sizeAlp = 10;
const size_t maxNumDigit = 21;  // size_t От 0 до 18 446 744 073 709 551 615

void countingSort(size_t* inputArray, int sizeInputArr, int sizeAlphabet, size_t numDigit) {
    
    size_t mod = std::pow(10, numDigit);
    size_t div = std::pow(10,(numDigit-1));

    size_t* counterArray = new size_t[sizeAlphabet];
    for (int i = 0; i < sizeAlphabet; ++i) {
        counterArray[i] = 0;
    }
    for (int i = 0; i < sizeInputArr; ++i) {
        ++counterArray[(inputArray[i] % mod) / div];
    }
    for (int i = 1; i < sizeAlphabet; ++i) {
        counterArray[i] += counterArray[i - 1];
    }
    size_t* sortedArray = new size_t[sizeInputArr];
    for(int i = sizeInputArr - 1; i >= 0; --i) {
        sortedArray[--counterArray[(inputArray[i] % mod) / div]] = inputArray[i];
    }
    delete[] counterArray;
    for (int i = 0; i < sizeInputArr; ++i) {
        inputArray[i] = sortedArray[i];
    }
    delete[] sortedArray;
}


int main() {
    int sizeArr = 0;
    std::cin >> sizeArr;
    size_t* array = new size_t[sizeArr];

    for (int i = 0; i < sizeArr; ++i) {
        std::cin >> array[i];
    } 

    for (size_t i = 1; i < maxNumDigit; ++i) {
        countingSort(array, sizeArr, sizeAlp, i);
    }

    for (int i = 0; i < sizeArr; ++i) {
        std::cout << array[i] << " ";
    }

    delete[] array;
    return 0;
    
}
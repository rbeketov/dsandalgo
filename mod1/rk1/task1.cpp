#include <iostream>
#include <string>
#include <cassert>
#include <cmath>


int binSearch(const int* array, int count) {
    int first = 0;
    int last = count;
    while (first < last) {
        int middle = (first + last) / 2;
        if (array[middle-1] <= array[middle]) {
            first = middle + 1;
        } else {
            last = middle;
        }
    }
    return first;
}

int main() {
    int sizeArray = 0;
    std::cin >> sizeArray;

    int* array = new int[sizeArray];

    for (int i = 0; i < sizeArray; ++i) {
        std::cin >> array[i];
    }

    std::cout << binSearch(array, sizeArray) <<  std::endl;


    return 0;
}
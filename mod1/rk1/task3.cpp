#include <iostream>
#include <string>
#include <cassert>
#include <cmath>



struct Pair{
    int xValue;
    int yValue;
};
template <class Comparator>
void sort(Pair* array, int sizeArray, Comparator comp) {
    for (int i = 1; i < sizeArray; ++i) {
        Pair tmp = array[i];
        int j = i - 1;
        for( ; j >= 0 && comp(tmp, array[j]); --j ) {
            array[j + 1] = array[j];
        }
        array[j + 1] = tmp;
    }
}


int main() {

    int sizeArray = 0;
    std::cin >> sizeArray;

    Pair* array = new Pair[sizeArray];

    for (int i = 0; i < sizeArray; ++i) {
        std::cin >> array[i].xValue >> array[i].yValue;
    }

    sort(array, sizeArray, [] (const Pair& l, const Pair& r) {
                                if (l.xValue != r.xValue) {
                                    return l.xValue < r.xValue;
                                } else {
                                    return l.yValue < r.yValue;
                                }
                              });

    
    Pair minPoint = array[0];
    sort(array+1, sizeArray-1, [&minPoint] (const Pair& l, const Pair& r) {
                                if (l.xValue - minPoint.xValue > 0 &&  r.xValue - minPoint.xValue > 0) {
                                    return l.yValue - minPoint.yValue < r.yValue - minPoint.yValue;
                                } else if (l.xValue - minPoint.xValue < 0 &&  r.xValue - minPoint.xValue < 0) {
                                    return l.yValue - minPoint.yValue > r.yValue - minPoint.yValue;
                                } else if (l.xValue - minPoint.xValue > 0) {
                                    return true;
                                } else {
                                    return false;
                                }
                            });

    for (int i = 0; i < sizeArray; ++i) {
        std::cout << array[i].xValue << " " << array[i].yValue << std::endl;
    }

    return 0;
}
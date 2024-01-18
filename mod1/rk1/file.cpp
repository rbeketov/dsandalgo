int binSearch(const int* array, int count, int element) {
    int first = 0;
    int last = count;
    while (first < last) {
        int middle = (first + last) / 2;
        if (array[middle] < element) {
            first = middle + 1;
        } else {
            last = middle;
        }
    }
    return (first == count || array[first] != element) ? -1 : first;
}
#include <iostream>
#include <string>

const static int DefaultInitialSize = 2;

template<typename T>
class DinArray {
 public:
    DinArray() : buffer( 0 ), bufferSize( 0 ), realSize( 0 ) {}
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



template<typename T>
class Stack {
 public:
    Stack(): top(-1) {}

    ~Stack() {}

    void push(T value) {
        if (top + 1 == buffer.size()) {
            buffer.add(value);
            top++;
        } else {
            buffer[++top] = value;
        }
    }
    T pop() {
        assert(top != -1);
        return buffer[top--];
    }

    bool isEmpty() const {
        return top == -1;
    }

 private:
    DinArray<T> buffer;
    int top;
};


bool correctBrackets(char* array, int sizeArray) {
    if (sizeArray % 2 != 0) {
        return false;
    }
    Stack<int> counter;
    for (int i = 0; i < sizeArray; ++i) {
        if (array[i] == '(') {
            counter.push(1);
        } else if (!counter.isEmpty()) {
            counter.pop();
        } else {
            return false;
        }
    }
    if (!counter.isEmpty()) {
        return false;
    }
    return true;
}


int main() {

    std::string array;
    std::cin >> array;

    if (correctBrackets(&array[0], array.length())) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}
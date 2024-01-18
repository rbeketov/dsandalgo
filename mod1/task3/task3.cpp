// 3_3. Реализовать очередь с помощью двух стеков.
//Требования: Очередь должна быть реализована в виде класса.
//Стек тоже должен быть реализован в виде класса.

#include <cassert>
#include <iostream>

namespace queue {

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

template<typename T>
class Queue {
 public:
    void enQueue(int a) {
        frontStack.push(a);
    }
    int deQueue() {
        if (isEmpty()) {
            return -1;
        }

        if (backStack.isEmpty()) {
            while(!frontStack.isEmpty()) {
                backStack.push(frontStack.pop());
            }
        }
        return backStack.pop();
        
    }

    bool isEmpty() const { 
        return frontStack.isEmpty() && backStack.isEmpty();
    }

 private:
    Stack<T> frontStack;
    Stack<T> backStack;
};


}  // namespace queue


bool testCases(unsigned sizeTest) {
    
    enum COMMAND {
        DEQUEUE = 2,
        ENQUEUE = 3,
    };
    
    queue::Queue<int> queue;


    for (unsigned i = 0; i < sizeTest; ++i) {
        unsigned numCommand = 0;
        int value = 0;
        std::cin >> numCommand;
        std::cin >> value;
        switch (numCommand) {
            case DEQUEUE:
                if (queue.deQueue() != value) {
                    return false;
                }
                break;
            case ENQUEUE:
                queue.enQueue(value);
                break;
            default:
                return false;
        }
        
    }
    return true;

}


int main() {
    unsigned sizeTest = 0;
    std::cin >> sizeTest;
    if (testCases(sizeTest)) {
        std::cout << "YES" << std::endl;
    } else { 
        std::cout << "NO" << std::endl;
    }

    return 0;
}
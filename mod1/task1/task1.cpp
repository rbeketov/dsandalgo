// 1.1 Подсчитать кол-во единичных бит в входном числе , стоящих на четных позициях.
// Позиции битов нумеруются с 0.
// Входное число лежит в диапазоне 0..2^32 -1 и вводится в десятичном виде.

#include <iostream>

unsigned countEvenBit(unsigned number) {
    unsigned counterEvens = 0;
    while (number > 0) {
        (number & 1) == 1 ? counterEvens++ : 0;  // проверка на 1 
        number >>= 2;  // идём только по чётным битам
    }
    return counterEvens;
} 



int main() {
    unsigned number = 0;
    std::cin >> number;
    std::cout << countEvenBit(number) << std::endl;
    return 0;
}
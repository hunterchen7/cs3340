#include <iostream>

// lucas numbers
// l_n = l_n-1 + l_n-2, n>1; l_1 = 1, l_0 = 2

unsigned long long int lucasNum(unsigned long int i) {
    if (i == 0) return 2;
    if (i == 1) return 1;
    return lucasNum(i - 1) + lucasNum(i-2);
}

int main() {
    for (int i = 0; i <= 10; i++) {
        int mult = 5;
        std::cout << "L_" << i * mult << " = " << lucasNum(i * mult) << "\n";
    }
}
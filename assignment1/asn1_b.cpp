#include <iostream>

#define BIGINT_DIGS 512 // digits
class bigInt {
    public:
        char num[BIGINT_DIGS];
        bigInt(char i) { // initalized with single digit
            num[BIGINT_DIGS - 1] = char(i);
        }
        bigInt(char arr[BIGINT_DIGS]) { // initalized with array of digits
            for (int i = 0; i < sizeof(num); i++) num[i] = char(arr[i]);
        }
        friend std::ostream &operator<<(std::ostream &out, bigInt bi) {
            int i = 0; // skip leading zeroes
            for (i; i < BIGINT_DIGS && bi.num[i] == char(0); i++);
            for (i; i < BIGINT_DIGS; i++) {
                std::cout << int(bi.num[i]);
            }
            return out;
        }
        bigInt operator+(bigInt num) {
            bigInt res = bigInt(this->num);
            for (int i = sizeof(num.num) - 1; i >= 0; i--) {
                int sum = int(num.num[i]) + int(res.num[i]);
                if (sum > 9) res.num[i-1] += 1;
                res.num[i] = char(sum) % 10;
            }
            return res;
        }
};

bigInt *memo[1024] = {new bigInt(2), new bigInt(1)};
bigInt lucasNum(int i) {
    if (memo[i] != 0) return *memo[i];
    memo[i] = new bigInt(lucasNum(i - 1) + lucasNum(i - 2));
    return *memo[i];
}

int main() {
    for (int i = 0; i <= 25; i++) {
        int step = 20 * i;
        std::cout << "L_" << step << " = " << lucasNum(step) << "\n";
    }
}
// Given a number num and a value k. Print k multiple of num.

#include <iostream>
using namespace std;

void f(int num, int k) {
    // base case
    if (k <= 0) {
        return;
    }
    // assumption
    f(num, k - 1);  // the function correctly prints the first k - 1 multiples
    // self work
    cout << (num * k) << " ";
}

int main() {
    f(3, 4);
    cout << endl;  // 3 6 9 12

    return 0;
}
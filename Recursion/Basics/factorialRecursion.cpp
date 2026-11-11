#include <iostream>
using namespace std;

int factorial(int n) {
    // base case: 0! = 1 and 1! = 1
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main() {
    int result = factorial(5);
    cout << result << endl;  // 120

    cout << factorial(0) << endl;  // 1
    return 0;
}

// Space is used by the call stack, one frame per active call.

// Time complexity -> O(n)
// Space complexity -> O(n)

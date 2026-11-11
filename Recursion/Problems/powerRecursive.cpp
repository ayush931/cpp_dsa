// Given two numbers p and q, find the value of p ^ q using a recursive
// function.

#include <iostream>
using namespace std;

int f(int p, int q) {
    if (q == 0) {
        return 1;
    }
    return p * f(p, q - 1);
}

int main() {
    int result = f(3, 4);
    cout << result << endl;
    return 0;
}

// Time complexity -> O(q)
// Space complexity -> O(q)
// Given two numbers p and q, find the value of p ^ q using a recursive
// function.

#include <iostream>
using namespace std;

int f(int p, int q) {
    // base case
    if (q == 0) {
        return 1;
    }
    if (q % 2 == 0) {
        // if q is even
        int res = f(p, q / 2);
        return res * res;
    } else {
        // if q is odd
        int res = f(p, (q - 1) / 2);
        return p * res * res;
    }
}

int main() {
    int result = f(3, 4);
    cout << result << endl;
    return 0;
}

// Time complexity -> O(log q)
// Space complexity -> O(log q) for the recursion stack
#include <cmath>
#include <iostream>
using namespace std;

int main() {
    int n = 5;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < sqrt(n); j++) {
            cout << "*";
        }
        cout << endl;
    }

    return 0;
}

/*
 *  i = 0, j = sqrt(n) instructions
 *  i = 1, j = sqrt(n) instructions
 *  i = 2, j = sqrt(n) instructions
 *  i = 3, j = sqrt(n) instructions
 *  i = 4, j = sqrt(n) instructions
 *
 *
 *  i = n - 1, j = sqrt(n) instructions
 *
 *  Total -> nsqrt(n)
 *
 *  Worst case -> O(nsqrt(n))
 */
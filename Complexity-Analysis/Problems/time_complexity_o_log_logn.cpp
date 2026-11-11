#include <iostream>
using namespace std;

int main() {
    int N = 50;
    int value = 0;

    for (int i = 2; i <= N; i *= i) {
        value++;
    }

    cout << value << endl;

    return 0;
}

// Time complexity = O(log(logN))
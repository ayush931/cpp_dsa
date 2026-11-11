#include <iostream>
using namespace std;

int main() {
    int N = 10;
    int value = 0;

    for (int i = 1; i <= N; i++) {
        for (int j = N; j > i; j--) {
            value++;
        }
    }

    cout << value << endl;

    return 0;
}

// Time complexity -> O(N^2)
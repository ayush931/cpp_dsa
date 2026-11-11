#include <iostream>
using namespace std;

int main() {
    int N = 1024;
    int value = 0;

    for (int i = N; i > 0; i /= 2) {
        for (int j = 0; j < i; j++) {
            value++;
        }
    }

    cout << value << endl;

    return 0;
}

// Time complexity -> O(N)
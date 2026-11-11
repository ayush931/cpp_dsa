// Calculate the time complexity

#include <iostream>
using namespace std;

int main() {
    int N = 20;
    int value = 0;

    for (int i = 1; i <= N; i += i) {
        value++;
    }
    cout << value << endl;

    return 0;
}

// Time complexity -> O(logN)
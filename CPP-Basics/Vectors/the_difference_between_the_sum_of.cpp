// Find the difference between the sum of elements at even indices to the sum of
// elements at odd indices.

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int array[] = {1, 2, 3, 4, 5};

    int sum = 0;

    for (int i = 0; i < 5; i++) {
        if (i % 2 == 0) {
            sum = sum + array[i];
        } else {
            sum = sum - array[i];
        }
    }

    cout << sum << endl;

    return 0;
}
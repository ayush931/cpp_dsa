// Find the second largest element in the given Array. (using another method)

#include <climits>
#include <iostream>
using namespace std;

int secondLargestElement(int array[], int size) {
    int max = INT_MIN;
    int second_max = INT_MIN;

    for (int i = 0; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }

    for (int i = 0; i < size; i++) {
        if (array[i] > second_max && array[i] != max) {
            second_max = array[i];
        }
    }

    return second_max;
}

int main() {
    int array[] = {1, 2, 3, 4, 50, 66, 7, 70};
    int size = sizeof(array) / sizeof(array[0]);

    cout << secondLargestElement(array, size) << endl;

    return 0;
}
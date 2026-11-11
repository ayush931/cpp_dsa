// Find the second largest element in the given Array.

#include <iostream>
using namespace std;

int largestElementIndex(int array[], int size) {
    int max = array[0];

    int maxIndex = 0;

    for (int i = 0; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}

int main() {
    int array[] = {1, 2, 3, 4, 50, 66, 7, 66};
    int size = sizeof(array) / sizeof(array[0]);

    int indexOfLargest = largestElementIndex(array, size);

    cout << array[indexOfLargest] << endl;

    // array[indexOfLargest] = -1;

    int largestElement = array[indexOfLargest];

    for (int i = 0; i < size; i++) {
        if (array[i] == largestElement) {
            array[i] = -1;
        }
    }

    int indexOfSecondLargest = largestElementIndex(array, size);

    cout << array[indexOfSecondLargest] << endl;

    return 0;
}
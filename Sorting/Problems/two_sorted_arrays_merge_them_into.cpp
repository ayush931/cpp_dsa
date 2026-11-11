// Given two sorted arrays, merge them into one sorted array.
// The merge is stable: when the values are equal, the element from the first
// array is written first.

#include <iostream>
#include <vector>
using namespace std;

void merge(const int arr1[], int n1, const int arr2[], int n2, int arr3[]) {
    int a = 0;  // array 1
    int b = 0;  // array 2
    int c = 0;  // array 3

    while (a < n1 && b < n2) {
        if (arr1[a] <= arr2[b]) {
            arr3[c++] = arr1[a++];
        } else {
            arr3[c++] = arr2[b++];
        }
    }

    while (a < n1) {
        arr3[c++] = arr1[a++];
    }

    while (b < n2) {
        arr3[c++] = arr2[b++];
    }
}

int main() {
    int arr1[] = {5, 8, 10, 11, 12};
    int arr2[] = {2, 7, 8};

    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    int n3 = n1 + n2;
    vector<int> arr3(n3);

    merge(arr1, n1, arr2, n2, arr3.data());

    for (int i = 0; i < n3; i++) {
        cout << arr3[i] << " ";
    }
    cout << endl;
    // 2 5 7 8 8 10 11 12

    return 0;
}

// Time complexity -> O(n1 + n2)
// Space complexity -> O(n1 + n2) for the output array

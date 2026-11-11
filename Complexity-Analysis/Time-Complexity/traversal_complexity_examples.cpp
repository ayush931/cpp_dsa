// Time complexity when traversing 2 individual arrays of length M and N
// respectively.

#include <iostream>
using namespace std;

int main() {
    int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = 9;

    int arr2[] = {2, 9, 10, 12, 3, 4, 0, 8};
    int m = 8;

    int sum1 = 0;
    int sum2 = 0;

    for (int i = 0; i < n; i++) {
        sum1 = sum1 + arr1[i];
    }

    for (int i = 0; i < m; i++) {
        sum2 = sum2 + arr2[i];
    }

    cout << sum1 << " " << sum2 << endl;

    return 0;
}

// Time complexity -> O(n + m)
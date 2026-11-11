// Repeatedly take elements from the unsorted array and insert in sorted
// subarray.

#include <iostream>
#include <vector>
using namespace std;

void insertionSort(vector<int> &v) {
    int n = v.size();

    for (int i = 1; i < n; i++) {
        int currentElement = v[i];

        // Find the correct position for our current elements

        int j = i - 1;
        while (j >= 0 && v[j] > currentElement) {
            v[j + 1] = v[j];
            j--;
        }
        // insert current element
        v[j + 1] = currentElement;
    }
    return;
}

int main() {
    int n;
    cin >> n;

    vector<int> v(n);

    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    insertionSort(v);

    cout << "Sorted Array is: \n";

    for (int i = 0; i < n; i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    return 0;
}

// Time complexity -> O(n^2)
// Space complexity -> O(1)
// Best case time complexity -> Omega(n)
// If array is almost sorted or few unsorted element in an array.
// Stable sorting algorithm
// O(n log n)
//
// Many efficient algorithms sort the input first, or split it into halves and
// do linear work at every level. Merge sort is the classic example:
//   - log n levels (the array is halved each time)
//   - O(n) work per level (merging all the pieces)
//   - total O(n log n)
//
// O(n log n) is the lower bound for comparison-based sorting. It is slightly
// worse than O(n) but far better than O(n^2) for large inputs.

#include <iostream>
#include <vector>
using namespace std;

long long mergeComparisons = 0;

void merge(vector<int> &values, int left, int mid, int right,
           vector<int> &temp) {
    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right) {
        mergeComparisons++;

        if (values[i] <= values[j]) {
            temp[k++] = values[i++];
        } else {
            temp[k++] = values[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = values[i++];
    }
    while (j <= right) {
        temp[k++] = values[j++];
    }

    for (int index = left; index <= right; index++) {
        values[index] = temp[index];
    }
}

void mergeSort(vector<int> &values, int left, int right, vector<int> &temp) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSort(values, left, mid, temp);
    mergeSort(values, mid + 1, right, temp);
    merge(values, left, mid, right, temp);
}

void print(vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    vector<int> values = {38, 27, 43, 3, 9, 82, 10};
    vector<int> temp(values.size());

    mergeComparisons = 0;
    mergeSort(values, 0, values.size() - 1, temp);

    print(values);                     // 3 9 10 27 38 43 82
    cout << mergeComparisons << endl;  // about n log n comparisons

    // Compare the growth of n^2 and n log n.
    cout << "n\tn^2\tn log n" << endl;

    for (int n = 8; n <= 4096; n *= 2) {
        long long quadratic = (long long)n * n;

        // Integer log2 without <cmath>.
        int log = 0;
        for (int power = 1; power < n; power *= 2) {
            log++;
        }

        long long linearithmic = (long long)n * log;
        cout << n << "\t" << quadratic << "\t" << linearithmic << endl;
    }

    return 0;
}

// Time complexity -> O(n log n): log n levels, O(n) merging per level.
// Space complexity -> O(n) for the temporary array, plus O(log n) recursion.
//
// Special cases: an empty array, one element, already sorted input (still
// O(n log n)), reverse sorted input, duplicate values, linked lists (merge
// sort is a natural fit because merging needs no random access).
//
// Other O(n log n) algorithms: heap sort, quick sort (average), sorting with
// a balanced tree, and divide-and-conquer algorithms such as closest pair of
// points and fast Fourier transform.

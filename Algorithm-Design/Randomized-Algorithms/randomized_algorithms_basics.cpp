/**
 * Randomized algorithms -> use random numbers to make decisions
 * Las Vegas -> always correct, expected time is bounded
 * Monte Carlo -> may be wrong with small probability, time is bounded
 *
 * Randomized quickselect -> expected O(n) kth smallest element
 * Worst case -> O(n^2)
 * Reservoir sampling -> choose k random elements from a stream in O(n)
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

int partition(vector<int> &arr, int left, int right) {
    int randomIndex = left + rand() % (right - left + 1);
    swap(arr[randomIndex], arr[right]);

    int pivot = arr[right];
    int i = left;

    for (int j = left; j < right; j++) {
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]);
            i++;
        }
    }

    swap(arr[i], arr[right]);
    return i;
};

int quickSelect(vector<int> &arr, int left, int right, int k) {
    if (left == right) {
        return arr[left];
    }

    int pivotIndex = partition(arr, left, right);

    if (pivotIndex == k) {
        return arr[pivotIndex];
    }
    if (pivotIndex > k) {
        return quickSelect(arr, left, pivotIndex - 1, k);
    }
    return quickSelect(arr, pivotIndex + 1, right, k);
};

vector<int> reservoirSampling(vector<int> &stream, int k) {
    vector<int> reservoir(stream.begin(), stream.begin() + k);

    for (int i = k; i < stream.size(); i++) {
        int randomIndex = rand() % (i + 1);

        if (randomIndex < k) {
            reservoir[randomIndex] = stream[i];
        }
    }

    return reservoir;
};

int main() {
    vector<int> arr = {7, 10, 4, 3, 20, 15};
    int k = 2;

    cout << quickSelect(arr, 0, arr.size() - 1, k) << endl;  // 4

    vector<int> stream = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<int> sample = reservoirSampling(stream, 3);

    for (int value : sample) {
        cout << value << " ";
    }
    cout << endl;

    return 0;
}

// Special cases: empty array, k out of range, duplicate elements,
// k larger than the stream, adversarial input for quickselect

// Bucket sort distributes values into buckets, sorts each bucket, then
// concatenates the buckets. It works best when the input is spread evenly
// over a known range.

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void bucketSort(float arr[], int size) {
    if (size <= 0) {
        return;
    }

    vector<vector<float> > bucket(size, vector<float>());

    // finding range
    float max_ele = arr[0];
    float min_ele = arr[0];

    for (int i = 1; i < size; i++) {
        max_ele = max(max_ele, arr[i]);
        min_ele = min(min_ele, arr[i]);
    }

    float range = (max_ele - min_ele) / size;

    // all values are equal, so the array is already sorted
    if (range == 0) {
        return;
    }

    // step 2: inserting elements into the bucket
    for (int i = 0; i < size; i++) {
        int index = (int)((arr[i] - min_ele) / range);
        if (index >= size) {
            index = size - 1;  // the maximum maps to the last bucket
        }
        bucket[index].push_back(arr[i]);
    }

    // step 3: sorting the individual buckets
    for (int i = 0; i < size; i++) {
        if (!bucket[i].empty()) {
            sort(bucket[i].begin(), bucket[i].end());
        }
    }

    // step 4: combining elements from buckets
    int k = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < (int)bucket[i].size(); j++) {
            arr[k++] = bucket[i][j];
        }
    }
}

int main() {
    float arr[] = {0.13, 0.45, 0.12, 0.89, 0.75, 0.63, 0.85, 0.39};
    int size = sizeof(arr) / sizeof(arr[0]);

    bucketSort(arr, size);

    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    // 0.12 0.13 0.39 0.45 0.63 0.75 0.85 0.89

    // edge case: every value is equal, so range is 0
    float same[] = {0.5, 0.5, 0.5};
    bucketSort(same, 3);

    for (int i = 0; i < 3; i++) {
        cout << same[i] << " ";
    }
    cout << endl;
    // 0.5 0.5 0.5

    return 0;
}

// Time complexity -> average O(n + k) with evenly distributed input;
//                    worst O(n log n) here because each bucket uses std::sort
//                    (O(n^2) if a bucket is sorted with insertion sort)
// Space complexity -> O(n + k), where k is the number of buckets
//
// Special cases: empty input, one element, all equal values, values clustered
// into one bucket, and negative values (the range shift still works).

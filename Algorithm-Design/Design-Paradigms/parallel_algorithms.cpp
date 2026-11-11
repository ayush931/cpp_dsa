/**
 * Parallel algorithms
 *
 * Parallelism splits work across threads. Speedup is limited by the serial
 * part (Amdahl's law), communication overhead, and load imbalance.
 *
 * 1. Parallel sum
 *    Split the array into chunks, sum each chunk in a separate task, then add
 *    the partial sums. Work -> O(n), span -> O(n / p + log p).
 *
 * 2. Parallel merge sort
 *    Sort the two halves in parallel, then merge them. Work -> O(n log n),
 *    span -> O(log^2 n) with enough processors.
 *
 * Edge cases: empty input, one element, more threads than elements, hardware
 * concurrency reporting 0, and exceptions thrown inside a task.
 */

#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
using namespace std;

long long parallelSum(vector<int> &arr, int threads) {
    if (arr.empty()) {
        return 0;
    }

    threads = max(1, min(threads, (int)arr.size()));
    int chunk = (arr.size() + threads - 1) / threads;

    vector<future<long long>> tasks;

    for (int start = 0; start < (int)arr.size(); start += chunk) {
        int end = min((int)arr.size(), start + chunk);

        tasks.push_back(async(launch::async, [&arr, start, end]() {
            long long sum = 0;
            for (int i = start; i < end; i++) {
                sum += arr[i];
            }
            return sum;
        }));
    }

    long long total = 0;
    for (auto &task : tasks) {
        total += task.get();
    }

    return total;
}

void parallelMergeSort(vector<int> &arr, int left, int right, int depth) {
    if (left >= right) {
        return;
    }

    int middle = left + (right - left) / 2;

    if (depth > 0) {
        auto leftTask = async(launch::async, [&arr, left, middle, depth]() {
            parallelMergeSort(arr, left, middle, depth - 1);
        });

        parallelMergeSort(arr, middle + 1, right, depth - 1);
        leftTask.get();
    } else {
        parallelMergeSort(arr, left, middle, 0);
        parallelMergeSort(arr, middle + 1, right, 0);
    }

    vector<int> merged;
    merged.reserve(right - left + 1);

    int i = left;
    int j = middle + 1;

    while (i <= middle && j <= right) {
        if (arr[i] <= arr[j]) {
            merged.push_back(arr[i++]);
        } else {
            merged.push_back(arr[j++]);
        }
    }

    while (i <= middle) {
        merged.push_back(arr[i++]);
    }
    while (j <= right) {
        merged.push_back(arr[j++]);
    }

    for (int k = 0; k < (int)merged.size(); k++) {
        arr[left + k] = merged[k];
    }
}

int main() {
    vector<int> values(1000);
    iota(values.begin(), values.end(), 1);

    int threads = max(1u, thread::hardware_concurrency());
    cout << parallelSum(values, threads) << endl;  // 500500

    vector<int> empty;
    cout << parallelSum(empty, threads) << endl;  // 0

    vector<int> arr = {9, 3, 7, 1, 8, 2, 6, 4, 5, 0};
    parallelMergeSort(arr, 0, (int)arr.size() - 1, 3);

    for (int value : arr) {
        cout << value << " ";
    }
    cout << endl;  // 0 1 2 3 4 5 6 7 8 9

    return 0;
}

// Special cases:
// - empty array -> sum 0, sort does nothing
// - one element -> one task and no merge
// - more threads than elements -> clamp the thread count
// - hardware_concurrency may return 0 -> clamp to at least 1
// - exceptions inside async tasks are rethrown by future::get

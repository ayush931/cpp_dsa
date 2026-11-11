/**
 * Top K Frequent Elements
 *
 * Given an integer array nums and an integer k, return the k most frequent
 * elements. You may return the answer in any order.
 *
 * Example:
 * Input:  nums = [1, 1, 1, 2, 2, 3], k = 2
 * Output: [1, 2]
 *
 * Constraints: 1 <= nums.length <= 10^5, k is in the range
 * [1, number of distinct elements], values fit in int.
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

// Brute force: count frequencies, sort all distinct values -> O(n log n)
vector<int> topKFrequentBrute(vector<int> &nums, int k) {
    unordered_map<int, int> frequency;

    for (int value : nums) {
        frequency[value]++;
    }

    vector<pair<int, int>> entries;

    for (auto &entry : frequency) {
        entries.push_back({entry.second, entry.first});  // {count, value}
    }

    sort(entries.begin(), entries.end(), greater<pair<int, int>>());

    vector<int> result;

    for (int i = 0; i < k; i++) {
        result.push_back(entries[i].second);
    }

    return result;
}

// Optimized: keep only the k largest frequencies in a min-heap -> O(n log k)
vector<int> topKFrequent(vector<int> &nums, int k) {
    unordered_map<int, int> frequency;

    for (int value : nums) {
        frequency[value]++;
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>>
        minHeap;

    for (auto &entry : frequency) {
        minHeap.push({entry.second, entry.first});

        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }

    vector<int> result;

    while (!minHeap.empty()) {
        result.push_back(minHeap.top().second);
        minHeap.pop();
    }

    return result;
}

// Bucket sort: index = frequency -> O(n) time, O(n) space
vector<int> topKFrequentBucket(vector<int> &nums, int k) {
    unordered_map<int, int> frequency;

    for (int value : nums) {
        frequency[value]++;
    }

    vector<vector<int>> buckets(nums.size() + 1);

    for (auto &entry : frequency) {
        buckets[entry.second].push_back(entry.first);
    }

    vector<int> result;

    for (int count = buckets.size() - 1; count >= 0 && result.size() < k;
         count--) {
        for (int value : buckets[count]) {
            result.push_back(value);

            if (result.size() == k) {
                break;
            }
        }
    }

    return result;
}

void print(vector<int> &values) {
    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    vector<int> first = {1, 1, 1, 2, 2, 3};
    vector<int> second = {1};
    vector<int> third = {4, 4, 4, 5, 5, 6, 6, 6, 6};

    vector<int> brute = topKFrequentBrute(first, 2);
    print(brute);  // 1 2

    vector<int> optimized = topKFrequent(first, 2);
    print(optimized);  // 1 2

    vector<int> bucket = topKFrequentBucket(first, 2);
    print(bucket);  // 1 2

    vector<int> one = topKFrequent(second, 1);
    print(one);  // 1

    vector<int> all = topKFrequentBucket(third, 3);
    print(all);  // 6 4 5 (any order)

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n log n)
 *   min-heap -> O(n log k) time, O(n) space
 *   bucket sort -> O(n) time, O(n) space
 *
 * Special cases: k = 1, k = number of distinct elements, all values have the
 * same frequency, one distinct value, negative values, ties (any valid answer
 * is accepted).
 */

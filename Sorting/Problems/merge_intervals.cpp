/**
 * Merge Intervals
 *
 * Given an array of intervals where intervals[i] = [start, end], merge all
 * overlapping intervals and return an array of the non-overlapping intervals
 * that cover all the intervals in the input.
 *
 * Example:
 * Input:  [[1,3], [2,6], [8,10], [15,18]]
 * Output: [[1,6], [8,10], [15,18]]
 *
 * Constraints: 1 <= intervals.length <= 10^4,
 * 0 <= start <= end <= 10^4.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

bool overlaps(vector<int> &a, vector<int> &b) {
    // [1,4] and [4,5] touch, so they count as overlapping
    return a[0] <= b[1] && b[0] <= a[1];
}

// Brute force: repeatedly merge any overlapping pair -> O(n^2) or worse
vector<vector<int>> mergeBrute(vector<vector<int>> intervals) {
    bool didMerge = true;

    while (didMerge) {
        didMerge = false;

        for (int i = 0; i < intervals.size() && !didMerge; i++) {
            for (int j = i + 1; j < intervals.size(); j++) {
                if (!overlaps(intervals[i], intervals[j])) {
                    continue;
                }

                intervals[i][0] = min(intervals[i][0], intervals[j][0]);
                intervals[i][1] = max(intervals[i][1], intervals[j][1]);
                intervals.erase(intervals.begin() + j);
                didMerge = true;
                break;
            }
        }
    }

    return intervals;
}

// Optimized: sort by start, then merge in one pass -> O(n log n)
vector<vector<int>> merge(vector<vector<int>> &intervals) {
    if (intervals.empty()) {
        return {};
    }

    sort(intervals.begin(), intervals.end());

    vector<vector<int>> result;
    result.push_back(intervals[0]);

    for (int i = 1; i < intervals.size(); i++) {
        if (intervals[i][0] <= result.back()[1]) {
            // overlap: extend the end of the last merged interval
            result.back()[1] = max(result.back()[1], intervals[i][1]);
        } else {
            result.push_back(intervals[i]);
        }
    }

    return result;
}

void print(vector<vector<int>> &intervals) {
    for (vector<int> &interval : intervals) {
        cout << "[" << interval[0] << "," << interval[1] << "] ";
    }
    cout << endl;
}

int main() {
    vector<vector<int>> first = {{1, 3}, {2, 6}, {8, 10}, {15, 18}};
    vector<vector<int>> second = {{1, 4}, {4, 5}};
    vector<vector<int>> third = {{1, 4}, {0, 4}};
    vector<vector<int>> single = {{5, 7}};

    vector<vector<int>> brute = mergeBrute(first);
    print(brute);  // [1,6] [8,10] [15,18]

    vector<vector<int>> optimized = merge(first);
    print(optimized);  // [1,6] [8,10] [15,18]

    vector<vector<int>> touching = merge(second);
    print(touching);  // [1,5]

    vector<vector<int>> contained = merge(third);
    print(contained);  // [0,4]

    vector<vector<int>> one = merge(single);
    print(one);  // [5,7]

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2) comparisons, plus erasing elements
 *   optimized -> O(n log n) for sorting + O(n) for merging
 *
 * Special cases: empty input, one interval, intervals that only touch at an
 * endpoint, one interval completely inside another, unsorted input, duplicate
 * intervals, negative coordinates.
 */

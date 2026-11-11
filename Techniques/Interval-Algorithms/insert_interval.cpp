/**
 * Insert interval
 *
 * Given a sorted list of non-overlapping closed intervals and one new
 * interval, insert the new interval and merge everything it touches.
 *
 * Optimized approach (one pass):
 *   1. copy intervals that end before the new interval starts
 *   2. absorb every interval that starts before or at the new interval's end
 *   3. append the new interval
 *   4. copy the remaining intervals
 *
 * Brute force:
 *   Append the new interval, sort, and merge. That is O(n log n) and is used
 *   here to validate the linear solution.
 *
 * Time        -> O(n) optimized, O(n log n) brute force
 * Space       -> O(n) for the result
 *
 * Edge cases: empty input, new interval before all, after all, touching,
 * contained inside one interval, covering all intervals, duplicates, and
 * negative coordinates.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Interval {
    int start;
    int end;

    bool operator==(const Interval &other) const {
        return start == other.start && end == other.end;
    }
};

vector<Interval> insertBrute(vector<Interval> intervals, Interval added) {
    intervals.push_back(added);

    sort(intervals.begin(), intervals.end(),
         [](const Interval &a, const Interval &b) {
             if (a.start != b.start) {
                 return a.start < b.start;
             }
             return a.end < b.end;
         });

    vector<Interval> merged;

    for (const Interval &interval : intervals) {
        if (merged.empty() || interval.start > merged.back().end) {
            merged.push_back(interval);
        } else {
            merged.back().end = max(merged.back().end, interval.end);
        }
    }

    return merged;
}

vector<Interval> insertInterval(const vector<Interval> &intervals,
                                Interval added) {
    vector<Interval> result;
    int i = 0;
    int n = intervals.size();

    // Intervals completely before the new one.
    while (i < n && intervals[i].end < added.start) {
        result.push_back(intervals[i]);
        i++;
    }

    // Intervals that overlap or touch the new one.
    while (i < n && intervals[i].start <= added.end) {
        added.start = min(added.start, intervals[i].start);
        added.end = max(added.end, intervals[i].end);
        i++;
    }

    result.push_back(added);

    // Intervals completely after the new one.
    while (i < n) {
        result.push_back(intervals[i]);
        i++;
    }

    return result;
}

void print(const vector<Interval> &intervals) {
    cout << "[";

    for (int i = 0; i < (int)intervals.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << "[" << intervals[i].start << "," << intervals[i].end << "]";
    }

    cout << "]";
}

void runCase(const vector<Interval> &intervals, Interval added) {
    vector<Interval> optimized = insertInterval(intervals, added);
    vector<Interval> brute = insertBrute(intervals, added);

    print(optimized);
    cout << "  brute agrees: " << (optimized == brute ? "yes" : "no") << endl;
}

int main() {
    runCase({}, {5, 7});
    runCase({{1, 3}, {6, 9}}, {2, 5});
    runCase({{1, 3}, {6, 9}}, {10, 12});
    runCase({{1, 3}, {6, 9}}, {-2, 0});
    runCase({{1, 3}, {6, 9}}, {3, 6});
    runCase({{1, 10}}, {4, 6});
    runCase({{1, 2}, {3, 4}, {5, 6}}, {0, 7});
    runCase({{-5, -2}, {0, 3}}, {-3, 1});
    runCase({{1, 4}, {1, 4}}, {1, 4});

    return 0;
}

// Expected output:
// [[5,7]]  brute agrees: yes
// [[1,5], [6,9]]  brute agrees: yes
// [[1,3], [6,9], [10,12]]  brute agrees: yes
// [[-2,0], [1,3], [6,9]]  brute agrees: yes
// [[1,9]]  brute agrees: yes
// [[1,10]]  brute agrees: yes
// [[0,7]]  brute agrees: yes
// [[-5,3]]  brute agrees: yes
// [[1,4]]  brute agrees: yes
//
// Special cases: empty input, new interval before all, after all, touching,
// contained inside one interval, covering all intervals, duplicates, and
// negative coordinates.

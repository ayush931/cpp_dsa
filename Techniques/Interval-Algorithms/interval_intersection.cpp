/**
 * Intersection of two sorted interval lists
 *
 * Each input list is sorted by start and contains non-overlapping closed
 * intervals. The output is the sorted list of intersections.
 *
 * For one interval from each list, the intersection is:
 *
 *   [max(a.start, b.start), min(a.end, b.end)]
 *
 * which exists only when the low endpoint is <= the high endpoint. Touching
 * intervals therefore produce a single-point intersection such as [2, 2].
 *
 * Optimized approach:
 *   Two pointers. After recording an intersection, advance the interval that
 *   ends first, because it cannot intersect anything later in the other list.
 *
 * Brute force:
 *   Check every pair, collect the intersections, then sort.
 *
 * Time        -> O(n + m) optimized, O(n * m) brute force
 * Space       -> O(n + m) for the output
 *
 * Edge cases: empty input, one interval each, no overlap, touching endpoints,
 * full overlap, duplicates, and negative coordinates.
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

vector<Interval> intersectionBrute(const vector<Interval> &first,
                                   const vector<Interval> &second) {
    vector<Interval> result;

    for (const Interval &a : first) {
        for (const Interval &b : second) {
            int low = max(a.start, b.start);
            int high = min(a.end, b.end);

            if (low <= high) {
                result.push_back({low, high});
            }
        }
    }

    sort(result.begin(), result.end(),
         [](const Interval &a, const Interval &b) {
             if (a.start != b.start) {
                 return a.start < b.start;
             }
             return a.end < b.end;
         });

    return result;
}

vector<Interval> intersection(const vector<Interval> &first,
                              const vector<Interval> &second) {
    vector<Interval> result;
    int i = 0;
    int j = 0;

    while (i < (int)first.size() && j < (int)second.size()) {
        int low = max(first[i].start, second[j].start);
        int high = min(first[i].end, second[j].end);

        if (low <= high) {
            result.push_back({low, high});
        }

        if (first[i].end < second[j].end) {
            i++;
        } else if (second[j].end < first[i].end) {
            j++;
        } else {
            i++;
            j++;
        }
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

void runCase(const vector<Interval> &first, const vector<Interval> &second) {
    vector<Interval> optimized = intersection(first, second);
    vector<Interval> brute = intersectionBrute(first, second);

    print(optimized);
    cout << "  brute agrees: " << (optimized == brute ? "yes" : "no") << endl;
}

int main() {
    runCase({{0, 2}, {5, 10}, {13, 23}, {24, 25}},
            {{1, 5}, {8, 12}, {15, 24}, {25, 26}});

    runCase({}, {{1, 2}});
    runCase({{1, 2}}, {});
    runCase({{1, 2}}, {{3, 4}});
    runCase({{1, 2}}, {{2, 3}});
    runCase({{1, 10}}, {{2, 3}});
    runCase({{-5, -2}, {-1, 3}}, {{-4, -1}, {0, 5}});
    runCase({{1, 3}, {1, 3}}, {{2, 4}});

    return 0;
}

// Expected output:
// [[1,2], [5,5], [8,10], [15,23], [24,24], [25,25]]  brute agrees: yes
// []  brute agrees: yes
// []  brute agrees: yes
// []  brute agrees: yes
// [[2,2]]  brute agrees: yes
// [[2,3]]  brute agrees: yes
// [[-4,-2], [-1,-1], [0,3]]  brute agrees: yes
// [[2,3], [2,3]]  brute agrees: yes
//
// Special cases: empty input, one interval each, no overlap, touching
// endpoints, full overlap, duplicates, and negative coordinates.
//
// Duplicate intervals in an input produce duplicate intersections. That is
// expected because the output describes pairs, not a set of points. If unique
// output is required, merge the duplicates afterward.

/**
 * Minimum arrows to burst balloons
 *
 * Each balloon is a closed interval [start, end]. An arrow shot at position x
 * bursts every balloon whose interval contains x. Find the minimum number of
 * arrows needed to burst all balloons.
 *
 * Optimized greedy:
 *   Sort by end position. Shoot an arrow at the end of the first unburst
 *   balloon, which bursts every balloon that starts at or before that point.
 *   Repeat for the next unburst balloon.
 *
 * Brute force:
 *   Try every candidate arrow position (every start and end). For each
 *   uncovered balloon, recursively try each candidate inside it and take the
 *   best result. Memoization over the covered mask keeps the search small for
 *   the demo sizes.
 *
 * Time        -> O(n log n) greedy, exponential brute force (n is small)
 * Space       -> O(1) extra for the greedy, O(2^n) for the brute-force memo
 *
 * Edge cases: empty input, one balloon, touching intervals, duplicates,
 * negative coordinates, full overlap, and no overlap.
 */

#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

struct Interval {
    int start;
    int end;
};

int minArrowsBrute(const vector<Interval> &balloons) {
    int n = balloons.size();
    if (n == 0) {
        return 0;
    }

    vector<int> candidates;

    for (const Interval &balloon : balloons) {
        candidates.push_back(balloon.start);
        candidates.push_back(balloon.end);
    }

    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()),
                     candidates.end());

    vector<int> memo(1 << n, -1);

    function<int(int)> solve = [&](int mask) -> int {
        if (mask == 0) {
            return 0;
        }
        if (memo[mask] != -1) {
            return memo[mask];
        }

        int first = 0;
        while (!(mask & (1 << first))) {
            first++;
        }

        int best = INT_MAX;

        for (int point : candidates) {
            if (point < balloons[first].start || point > balloons[first].end) {
                continue;
            }

            int nextMask = mask;

            for (int i = 0; i < n; i++) {
                if ((nextMask & (1 << i)) && balloons[i].start <= point &&
                    point <= balloons[i].end) {
                    nextMask &= ~(1 << i);
                }
            }

            if (nextMask != mask) {
                best = min(best, 1 + solve(nextMask));
            }
        }

        return memo[mask] = best;
    };

    return solve((1 << n) - 1);
}

int minArrows(vector<Interval> balloons) {
    if (balloons.empty()) {
        return 0;
    }

    sort(balloons.begin(), balloons.end(),
         [](const Interval &a, const Interval &b) {
             if (a.end != b.end) {
                 return a.end < b.end;
             }
             return a.start < b.start;
         });

    int arrows = 0;
    long long lastArrow = LLONG_MIN;

    for (const Interval &balloon : balloons) {
        if ((long long)balloon.start > lastArrow) {
            arrows++;
            lastArrow = balloon.end;
        }
    }

    return arrows;
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

void runCase(const vector<Interval> &balloons) {
    int optimized = minArrows(balloons);
    int brute = minArrowsBrute(balloons);

    cout << "arrows: " << optimized << "  brute: " << brute
         << "  agree: " << (optimized == brute ? "yes" : "no") << endl;
}

int main() {
    runCase({});
    runCase({{7, 7}});
    runCase({{10, 16}, {2, 8}, {1, 6}, {7, 12}});
    runCase({{1, 2}, {3, 4}, {5, 6}, {7, 8}});
    runCase({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
    runCase({{1, 2}, {1, 2}, {1, 2}});
    runCase({{-5, -1}, {-2, 3}, {0, 4}, {5, 9}});
    runCase({{1, 10}, {2, 3}, {4, 5}});

    return 0;
}

// Expected output:
// arrows: 0  brute: 0  agree: yes
// arrows: 1  brute: 1  agree: yes
// arrows: 2  brute: 2  agree: yes
// arrows: 4  brute: 4  agree: yes
// arrows: 1  brute: 1  agree: yes
// arrows: 1  brute: 1  agree: yes
// arrows: 3  brute: 3  agree: yes
// arrows: 1  brute: 1  agree: yes
//
// Special cases: empty input, one balloon, touching intervals, duplicates,
// negative coordinates, full overlap, and no overlap.
//
// Shooting at the earliest ending balloon is safe: any arrow that bursts it
// must be at or before its end, and moving that arrow to its end can only burst
// more balloons that start later.

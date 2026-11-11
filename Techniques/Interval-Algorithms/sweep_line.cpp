/**
 * Sweep line: maximum overlapping intervals
 *
 * Each interval [start, end) produces two events:
 *
 *   start -> +1
 *   end   -> -1
 *
 * Sort the events by position. At the same position, process ends before
 * starts, because a half-open interval that ends at x does not cover x. This
 * "end before start" rule is the whole trick:
 *
 *   [1, 3] and [3, 5]  -> overlap 1, not 2
 *
 * Brute force:
 *   For every interval start, count how many intervals contain that point and
 *   take the maximum. The maximum overlap always occurs at some start point,
 *   so this is correct.
 *
 * Time        -> O(n log n) sweep, O(n^2) brute force
 * Space       -> O(n) for the events
 *
 * Edge cases: empty input, one interval, touching endpoints, duplicates,
 * negative coordinates, full overlap, and no overlap.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Interval {
    int start;
    int end;
};

struct Event {
    int position;
    int delta;
};

int maxOverlapBrute(const vector<Interval> &intervals) {
    int best = 0;

    for (const Interval &current : intervals) {
        int count = 0;

        for (const Interval &other : intervals) {
            if (other.start <= current.start && current.start < other.end) {
                count++;
            }
        }

        best = max(best, count);
    }

    return best;
}

int maxOverlap(const vector<Interval> &intervals) {
    vector<Event> events;

    for (const Interval &interval : intervals) {
        events.push_back({interval.start, +1});
        events.push_back({interval.end, -1});
    }

    sort(events.begin(), events.end(), [](const Event &a, const Event &b) {
        if (a.position != b.position) {
            return a.position < b.position;
        }
        return a.delta < b.delta;  // -1 (end) before +1 (start)
    });

    int current = 0;
    int best = 0;

    for (const Event &event : events) {
        current += event.delta;
        best = max(best, current);
    }

    return best;
}

// Used only to show why the tie order matters.
int maxOverlapStartFirst(const vector<Interval> &intervals) {
    vector<Event> events;

    for (const Interval &interval : intervals) {
        events.push_back({interval.start, +1});
        events.push_back({interval.end, -1});
    }

    sort(events.begin(), events.end(), [](const Event &a, const Event &b) {
        if (a.position != b.position) {
            return a.position < b.position;
        }
        return a.delta > b.delta;  // +1 (start) before -1 (end)
    });

    int current = 0;
    int best = 0;

    for (const Event &event : events) {
        current += event.delta;
        best = max(best, current);
    }

    return best;
}

void runCase(const vector<Interval> &intervals) {
    int optimized = maxOverlap(intervals);
    int brute = maxOverlapBrute(intervals);

    cout << "max overlap: " << optimized << "  brute: " << brute
         << "  agree: " << (optimized == brute ? "yes" : "no") << endl;
}

int main() {
    runCase({});
    runCase({{1, 5}});
    runCase({{1, 3}, {3, 5}});
    runCase({{1, 5}, {2, 6}, {4, 8}});
    runCase({{1, 5}, {1, 5}});
    runCase({{-5, -1}, {-3, 2}, {0, 4}});
    runCase({{1, 10}, {2, 3}, {4, 5}});
    runCase({{1, 2}, {3, 4}});

    vector<Interval> touching = {{1, 3}, {3, 5}};
    cout << "touching end-first: " << maxOverlap(touching)
         << ", start-first: " << maxOverlapStartFirst(touching) << endl;

    return 0;
}

// Expected output:
// max overlap: 0  brute: 0  agree: yes
// max overlap: 1  brute: 1  agree: yes
// max overlap: 1  brute: 1  agree: yes
// max overlap: 3  brute: 3  agree: yes
// max overlap: 2  brute: 2  agree: yes
// max overlap: 2  brute: 2  agree: yes
// max overlap: 2  brute: 2  agree: yes
// max overlap: 1  brute: 1  agree: yes
// touching end-first: 1, start-first: 2
//
// Special cases: empty input, one interval, touching endpoints, duplicates,
// negative coordinates, full overlap, and no overlap.
//
// The last line is the important one: with start-first ordering, the two
// touching intervals appear to overlap 2. End-first ordering correctly reports
// 1 for half-open intervals.

/**
 * Activity Selection
 *
 * Given N activities with their start and finish days, select the maximum
 * number of activities that one person can perform. A person can work on only
 * one activity at a time, and an activity's duration includes both its start
 * and finish day. Therefore the next activity must start after the previous
 * one ends.
 *
 * Input:  start[] = {1, 3, 2, 5}, end[] = {2, 4, 3, 6}
 * Output: 3
 * Explanation: choose activities (1,2), (3,4) and (5,6).
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Activity {
    int start;
    int end;
};

bool byFinishTime(const Activity &a, const Activity &b) {
    if (a.end != b.end) {
        return a.end < b.end;
    }
    return a.start < b.start;
}

// Brute force: try every subset and keep the largest compatible one.
// O(2^n * n) time, O(n) recursion space.
int maxActivitiesBrute(vector<Activity> &activities, int index, int lastEnd,
                       int chosen) {
    if (index == activities.size()) {
        return chosen;
    }

    // Skip this activity.
    int best = maxActivitiesBrute(activities, index + 1, lastEnd, chosen);

    // Take it only if it starts after the previous activity has ended.
    if (activities[index].start > lastEnd) {
        best = max(best, maxActivitiesBrute(activities, index + 1,
                                            activities[index].end, chosen + 1));
    }

    return best;
}

// Greedy: always take the compatible activity that finishes earliest.
// O(n log n) time for sorting, O(1) extra space.
int maxActivities(vector<Activity> &activities) {
    if (activities.empty()) {
        return 0;
    }

    sort(activities.begin(), activities.end(), byFinishTime);

    int count = 1;
    int lastEnd = activities[0].end;

    for (int i = 1; i < activities.size(); i++) {
        if (activities[i].start > lastEnd) {
            count++;
            lastEnd = activities[i].end;
        }
    }

    return count;
}

int main() {
    vector<Activity> activities = {{1, 2}, {3, 4}, {2, 3}, {5, 6}};

    vector<Activity> copy = activities;
    sort(copy.begin(), copy.end(), [](const Activity &a, const Activity &b) {
        return a.start < b.start;
    });

    cout << maxActivitiesBrute(copy, 0, -1, 0) << endl;  // 3
    cout << maxActivities(activities) << endl;           // 3

    vector<Activity> touching = {{1, 2}, {2, 3}, {3, 4}};
    cout << maxActivities(touching) << endl;  // 1 (days are shared)

    vector<Activity> single = {{7, 9}};
    cout << maxActivities(single) << endl;  // 1

    vector<Activity> none;
    cout << maxActivities(none) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute force -> O(2^n * n) to inspect every subset
 *   greedy -> O(n log n) to sort by finish time + O(n) scan
 * Space complexity:
 *   brute force -> O(n) recursion depth
 *   greedy -> O(1) extra after sorting
 *
 * Special cases: no activities, one activity, activities that touch on the
 * same day (they conflict because both endpoints are included), activities
 * with equal finish times, and activities that are already sorted.
 */

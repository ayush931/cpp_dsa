/**
 * Meeting Rooms II
 * Given meeting intervals [start, end), find the minimum number of rooms
 * required.
 *
 * Input: intervals = [[0, 30], [5, 10], [15, 20]]
 * Output: 2
 *
 * Brute force -> count overlapping meetings at every start time, O(n^2)
 * Optimized -> sort by start time and use a min-heap of end times, O(n log n)
 */

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int minRoomsBrute(vector<vector<int>> &intervals) {
    int rooms = 0;

    for (int i = 0; i < intervals.size(); i++) {
        int start = intervals[i][0];
        int overlapping = 0;

        for (int j = 0; j < intervals.size(); j++) {
            if (intervals[j][0] <= start && start < intervals[j][1]) {
                overlapping++;
            }
        }

        rooms = max(rooms, overlapping);
    }

    return rooms;
};

int minRooms(vector<vector<int>> &intervals) {
    if (intervals.empty()) {
        return 0;
    }

    sort(intervals.begin(), intervals.end());

    priority_queue<int, vector<int>, greater<int>> endTimes;

    for (auto &interval : intervals) {
        if (!endTimes.empty() && endTimes.top() <= interval[0]) {
            endTimes.pop();
        }

        endTimes.push(interval[1]);
    }

    return endTimes.size();
};

int main() {
    vector<vector<int>> intervals = {{0, 30}, {5, 10}, {15, 20}};

    cout << minRoomsBrute(intervals) << endl;  // 2
    cout << minRooms(intervals) << endl;       // 2

    return 0;
}

// Special cases: empty input, meetings that touch at endpoints,
// one meeting, all meetings overlapping, unsorted input

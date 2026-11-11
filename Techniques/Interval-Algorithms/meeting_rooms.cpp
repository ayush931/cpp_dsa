/**
 * Meeting rooms
 *
 * Two related questions for half-open intervals [start, end):
 *
 *   canAttendAll -> can one person attend every meeting?
 *   minRooms     -> how many rooms are needed for all meetings?
 *
 * Half-open intervals mean a meeting that ends at 10 and one that starts at 10
 * do not overlap, so they can share a room.
 *
 * Brute force:
 *   - canAttendAll checks every pair for overlap: O(n^2)
 *   - minRooms counts how many intervals contain each start time and takes the
 *     maximum: O(n^2)
 *
 * Optimized:
 *   - canAttendAll sorts by start and compares each start with the previous end
 *   - minRooms sorts starts and ends separately and sweeps with two pointers,
 *     releasing a room whenever an end is at or before the current start
 *
 * Time        -> O(n log n) optimized, O(n^2) brute force
 * Space       -> O(n) for the sorted arrays
 *
 * Edge cases: empty input, one meeting, touching endpoints, duplicate
 * meetings, negative coordinates, full overlap, and no overlap.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Interval {
    int start;
    int end;
};

bool canAttendAllBrute(const vector<Interval> &meetings) {
    for (int i = 0; i < (int)meetings.size(); i++) {
        for (int j = i + 1; j < (int)meetings.size(); j++) {
            bool overlap = max(meetings[i].start, meetings[j].start) <
                           min(meetings[i].end, meetings[j].end);

            if (overlap) {
                return false;
            }
        }
    }

    return true;
}

bool canAttendAll(vector<Interval> meetings) {
    if (meetings.empty()) {
        return true;
    }

    sort(meetings.begin(), meetings.end(),
         [](const Interval &a, const Interval &b) {
             if (a.start != b.start) {
                 return a.start < b.start;
             }
             return a.end < b.end;
         });

    for (int i = 1; i < (int)meetings.size(); i++) {
        if (meetings[i].start < meetings[i - 1].end) {
            return false;
        }
    }

    return true;
}

int minRoomsBrute(const vector<Interval> &meetings) {
    int best = 0;

    for (const Interval &current : meetings) {
        int overlapping = 0;

        for (const Interval &other : meetings) {
            if (other.start <= current.start && current.start < other.end) {
                overlapping++;
            }
        }

        best = max(best, overlapping);
    }

    return best;
}

int minRooms(const vector<Interval> &meetings) {
    int n = meetings.size();
    if (n == 0) {
        return 0;
    }

    vector<int> starts;
    vector<int> ends;

    for (const Interval &meeting : meetings) {
        starts.push_back(meeting.start);
        ends.push_back(meeting.end);
    }

    sort(starts.begin(), starts.end());
    sort(ends.begin(), ends.end());

    int rooms = 0;
    int best = 0;
    int endIndex = 0;

    for (int i = 0; i < n; i++) {
        // Release every room whose meeting has already ended.
        while (endIndex < n && ends[endIndex] <= starts[i]) {
            rooms--;
            endIndex++;
        }

        rooms++;
        best = max(best, rooms);
    }

    return best;
}

void runCase(const vector<Interval> &meetings) {
    bool attendOptimized = canAttendAll(meetings);
    bool attendBrute = canAttendAllBrute(meetings);
    int roomsOptimized = minRooms(meetings);
    int roomsBrute = minRoomsBrute(meetings);

    cout << "can attend: " << (attendOptimized ? "yes" : "no") << " (brute "
         << (attendBrute ? "yes" : "no") << ")"
         << ", rooms: " << roomsOptimized << " (brute " << roomsBrute << ")"
         << ", agree: "
         << (attendOptimized == attendBrute && roomsOptimized == roomsBrute
                 ? "yes"
                 : "no")
         << endl;
}

int main() {
    runCase({});
    runCase({{4, 9}});
    runCase({{7, 10}, {2, 4}});
    runCase({{1, 5}, {5, 10}});
    runCase({{1, 5}, {1, 5}});
    runCase({{0, 30}, {5, 10}, {15, 20}});
    runCase({{-5, 0}, {-2, 3}, {0, 5}});
    runCase({{1, 2}, {3, 4}, {5, 6}});

    return 0;
}

// Expected output:
// can attend: yes (brute yes), rooms: 0 (brute 0), agree: yes
// can attend: yes (brute yes), rooms: 1 (brute 1), agree: yes
// can attend: yes (brute yes), rooms: 1 (brute 1), agree: yes
// can attend: yes (brute yes), rooms: 1 (brute 1), agree: yes
// can attend: no (brute no), rooms: 2 (brute 2), agree: yes
// can attend: no (brute no), rooms: 2 (brute 2), agree: yes
// can attend: no (brute no), rooms: 2 (brute 2), agree: yes
// can attend: yes (brute yes), rooms: 1 (brute 1), agree: yes
//
// Special cases: empty input, one meeting, touching endpoints, duplicate
// meetings, negative coordinates, full overlap, and no overlap.
//
// minRooms is the maximum number of meetings in progress at the same time.
// Sorting starts and ends lets the sweep release rooms before adding a new
// meeting, which is exactly the half-open endpoint rule.

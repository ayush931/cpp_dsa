/**
 * There is one meeting room in a firm. There are N meetings in the form of
 * (S[i], F[i]) where S[i] is the start time of meeting i and F[i] is the finish
 * time of meeting i. The task os to find the maximum number of meetings that
 * can be accommodated in the meeting room. Print all meeting numbers. Input:
 * s[] = {1, 3, 0, 5, 8, 5}, f[] = {2, 4, 6, 7, 9, 9} Output: 1 2 4 5
 */

#include <bits/stdc++.h>
using namespace std;

struct meeting {
    int start;
    int end;
    int idx;
};

bool cmp(meeting m1, meeting m2) {
    return m1.end < m2.end;
};

void print_max_meeting(vector<meeting> &arr) {
    if (arr.empty()) {
        return;
    }

    sort(arr.begin(), arr.end(), cmp);

    meeting last = arr[0];
    cout << last.idx;

    for (int i = 1; i < arr.size(); i++) {
        // A meeting may start exactly when the previous one ends.
        if (arr[i].start >= last.end) {
            cout << " " << arr[i].idx;
            last = arr[i];
        }
    }

    cout << endl;
};

int main() {
    int n;
    cin >> n;
    vector<meeting> arr;
    int i = 0;

    while (n--) {
        int s, e;
        cin >> s >> e;
        i++;
        meeting m;
        m.start = s;
        m.end = e;
        m.idx = i;
        arr.push_back(m);
    }

    print_max_meeting(arr);
    return 0;
}
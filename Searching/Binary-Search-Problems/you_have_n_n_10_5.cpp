// You have 'n' (n <= 10^5) boxes of chocolates. Each box contains a[i]
// (a[i] <= 10000) chocolates. Distribute the boxes among 'm' students so that
// the maximum number of chocolates given to a single student is as small as
// possible.
// a. One box is allocated to exactly one student.
// b. All the boxes are allocated.
// c. Each student gets at least one box.
// d. Allotment is contiguous: a student cannot take box 1 and box 3 while
//    skipping box 2.
// Return that minimum possible maximum. It is always possible to distribute.
//
// Example:
//   boxes = [12, 34, 67, 90], students = 2 -> 113
//   (split as [12, 34, 67] and [90], or [12, 34] and [67, 90])

// The first line of input will contain the value of n, the number of boxes.
// The second line of input will contain the n numbers of denoting the number of
// chocolates in each box. The third line will contain the m, number of
// students.

#include <bits/stdc++.h>
using namespace std;

bool canDistChoc(vector<int> &arr, int mid, int s) {
    int n = arr.size();
    int studentsReqd = 1;
    int currSum = 0;

    for (int i = 0; i < n; i++) {
        if (arr[i] > mid) {
            return false;
        }
        if (currSum + arr[i] > mid) {
            studentsReqd++;
            currSum = arr[i];
            if (studentsReqd > s) {
                return false;
            }
        } else {
            currSum += arr[i];
        }
    }
    return true;
}

int distChoc(vector<int> &arr, int s) {
    int n = arr.size();
    if (n == 0 || s <= 0 || s > n) {
        return -1;
    }

    // The answer is at least the largest single box, because one student must
    // take that box whole.
    int lo = 0;
    int hi = 0;
    for (int i = 0; i < n; i++) {
        lo = max(lo, arr[i]);
        hi += arr[i];
    }

    int ans = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (canDistChoc(arr, mid, s)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}

int main() {
    int n;
    cin >> n;
    vector<int> v;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        v.push_back(x);
    }

    int s;
    cin >> s;
    cout << distChoc(v, s) << endl;
    return 0;
}

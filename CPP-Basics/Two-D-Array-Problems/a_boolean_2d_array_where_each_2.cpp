// Given a boolean 2D Array, where each row is sorted. Find the row with the
// maximum number of 1s.

#include <iostream>
#include <vector>
using namespace std;

int leftMostOneRow(vector<vector<int>> &V) {
    if (V.empty() || V[0].empty()) {
        return -1;
    }

    int leftMostOne = -1;
    int maxOnesRow = -1;

    int j = V[0].size() - 1;

    // finding leftmost one in 0th row

    while (j >= 0 && V[0][j] == 1) {
        leftMostOne = j;
        maxOnesRow = 0;
        j--;
    }

    // check in rest of the rows if we can find a one left to the leftMost

    for (int i = 1; i < V.size(); i++) {
        while (j >= 0 && V[i][j] == 1) {
            leftMostOne = j;
            j--;
            maxOnesRow = i;
        }
    }
    return maxOnesRow;
}

int main() {
    int n, m;

    cout << "Enter the no of rows in Matrix: ";
    cin >> n;

    cout << "Enter the no of columns in Matrix: ";
    cin >> m;

    vector<vector<int>> vec(n, vector<int>(m));

    cout << "Enter the elements of Matrix: " << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> vec[i][j];
        }
    }

    int res = leftMostOneRow(vec);

    cout << "Index having maximum number of 1s in row is: " << res << endl;

    return 0;
}
// Given a boolean 2D Array, where each row is sorted. Find the row with the
// maximum number of 1s.

#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int maximumOnesRow(vector<vector<int>> &V) {
    if (V.empty() || V[0].empty()) {
        return -1;
    }

    int maxOnes = INT_MIN;
    int maxOnesRow = -1;
    int columns = V[0].size();

    for (int i = 0; i < V.size(); i++) {
        for (int j = 0; j < V[i].size(); j++) {
            if (V[i][j] == 1) {
                int numberOfOnes = columns - j;
                if (numberOfOnes > maxOnes) {
                    maxOnes = numberOfOnes;
                    maxOnesRow = i;
                }
                break;
            }
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

    int res = maximumOnesRow(vec);

    cout << "Index having maximum number of 1s in row is: " << res << endl;

    return 0;
}
// Given a square matrix, turn it by 90 degress in a clockwise direction without
// using any extra space.

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

void rotateArray(vector<vector<int>> &vec) {
    int n = vec.size();

    // transpose

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            swap(vec[i][j], vec[j][i]);
        }
    }

    // reverse every row

    for (int i = 0; i < n; i++) {
        reverse(vec[i].begin(), vec[i].end());
    }
}

int main() {
    int n;

    cout << "Enter the no of rows and column in square matrix: ";
    cin >> n;

    vector<vector<int>> vec(n, vector<int>(n));

    cout << "Enter the elements of square matrix: " << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> vec[i][j];
        }
    }

    rotateArray(vec);

    cout << "90 Degree rotated matrix is: " << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
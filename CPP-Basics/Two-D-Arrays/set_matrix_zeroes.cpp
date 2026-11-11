// Set matrix zeroes
//
// If an element of an m x n matrix is 0, set its entire row and column to 0.
// The update must be based on the original zeros, not on zeros created while
// updating, otherwise the whole matrix can become zero.
//
// Example:
//   1 1 1        1 0 1
//   1 0 1   ->   0 0 0
//   1 1 1        1 0 1
//
// Brute force: remember the coordinates of every original zero, then set the
// affected rows and columns. O(k * (m + n)) time and O(k) space for k zeros.
//
// Optimized: use the first row and first column as marker arrays.
//   1. remember whether the first row or first column itself contains a zero
//   2. for every zero at (i, j) with i > 0 and j > 0, mark row i and column j
//      by writing 0 at matrix[i][0] and matrix[0][j]
//   3. use the markers to zero the interior
//   4. finally zero the first row and first column if needed
// O(m * n) time and O(1) extra space.
//
// Edge cases:
//   - empty matrix or a matrix with zero columns: nothing to do
//   - one row or one column: the marker logic still works
//   - a zero only in the first row or first column
//   - multiple zeros in the same row or column
//   - no zeros at all
//
// Complexity:
//   brute     -> O(k * (m + n)) time, O(k) space
//   optimized -> O(m * n) time, O(1) auxiliary space

#include <iostream>
#include <utility>
#include <vector>
using namespace std;

void setZeroesBrute(vector<vector<int>> &matrix) {
    vector<pair<int, int>> zeroPositions;

    for (int row = 0; row < static_cast<int>(matrix.size()); row++) {
        for (int column = 0; column < static_cast<int>(matrix[row].size());
             column++) {
            if (matrix[row][column] == 0) {
                zeroPositions.push_back({row, column});
            }
        }
    }

    for (const auto &position : zeroPositions) {
        int zeroRow = position.first;
        int zeroColumn = position.second;

        for (int column = 0; column < static_cast<int>(matrix[zeroRow].size());
             column++) {
            matrix[zeroRow][column] = 0;
        }

        for (int row = 0; row < static_cast<int>(matrix.size()); row++) {
            matrix[row][zeroColumn] = 0;
        }
    }
}

void setZeroes(vector<vector<int>> &matrix) {
    int rows = static_cast<int>(matrix.size());

    if (rows == 0 || matrix[0].empty()) {
        return;
    }

    int columns = static_cast<int>(matrix[0].size());
    bool firstRowHasZero = false;
    bool firstColumnHasZero = false;

    for (int column = 0; column < columns; column++) {
        if (matrix[0][column] == 0) {
            firstRowHasZero = true;
        }
    }

    for (int row = 0; row < rows; row++) {
        if (matrix[row][0] == 0) {
            firstColumnHasZero = true;
        }
    }

    for (int row = 1; row < rows; row++) {
        for (int column = 1; column < columns; column++) {
            if (matrix[row][column] == 0) {
                matrix[row][0] = 0;
                matrix[0][column] = 0;
            }
        }
    }

    for (int row = 1; row < rows; row++) {
        for (int column = 1; column < columns; column++) {
            if (matrix[row][0] == 0 || matrix[0][column] == 0) {
                matrix[row][column] = 0;
            }
        }
    }

    if (firstRowHasZero) {
        for (int column = 0; column < columns; column++) {
            matrix[0][column] = 0;
        }
    }

    if (firstColumnHasZero) {
        for (int row = 0; row < rows; row++) {
            matrix[row][0] = 0;
        }
    }
}

void print(const vector<vector<int>> &matrix) {
    for (const vector<int> &row : matrix) {
        for (int value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
}

int main() {
    vector<vector<int>> first = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    setZeroes(first);
    print(first);

    cout << endl;

    vector<vector<int>> second = {{0, 1, 2, 0}, {3, 4, 5, 2}, {1, 3, 1, 5}};
    setZeroes(second);
    print(second);

    cout << endl;

    vector<vector<int>> third = {{1, 2}, {0, 4}};
    setZeroesBrute(third);
    print(third);

    vector<vector<int>> empty;
    setZeroes(empty);
    print(empty);

    return 0;
}

// Output:
// 1 0 1
// 0 0 0
// 1 0 1
//
// 0 0 0 0
// 0 4 5 0
// 0 3 1 0
//
// 0 2
// 0 0
//
// The optimized version is the standard constant-space solution. The marker
// row and column are safe because their original zero state is saved first.

/**
 * Largest all-1 square in a rectangular binary matrix; return its AREA.
 * side[r][c] = 1 + min(top,left,diagonal) when the cell is 1, otherwise 0.
 * A rolling row plus saved old diagonal reduces storage to O(columns).
 * O(rows*columns) time. Unlike maximal rectangle, no histogram is needed.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

long long maximalSquare(const vector<string> &matrix) {
    if (matrix.empty()) {
        return 0;
    }
    size_t columns = matrix[0].size();
    vector<int> side(columns + 1);
    int largestSide = 0;
    for (const auto &row : matrix) {
        if (row.size() != columns) {
            throw invalid_argument("rectangular matrix required");
        }
        int diagonal = 0;
        for (size_t column = 1; column <= columns; ++column) {
            int oldTop = side[column];
            if (row[column - 1] == '1') {
                side[column] = 1 + min({oldTop, side[column - 1], diagonal});
            } else if (row[column - 1] == '0') {
                side[column] = 0;
            } else {
                throw invalid_argument("binary matrix required");
            }
            diagonal = oldTop;
            largestSide = max(largestSide, side[column]);
        }
    }
    return static_cast<long long>(largestSide) * largestSide;
}

int main() {
    assert(maximalSquare({"10100", "10111", "11111", "10010"}) == 4);
    assert(maximalSquare({"11", "11"}) == 4);
    assert(maximalSquare({"0"}) == 0);
    assert(maximalSquare({}) == 0);
    assert(maximalSquare({""}) == 0);
    cout << maximalSquare({"10100", "10111", "11111", "10010"}) << '\n';  // 4
}

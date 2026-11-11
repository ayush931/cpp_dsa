/**
 * Minimum Cost to Cut a Board into Squares
 *
 * A board has M rows and N columns. Cutting along a horizontal line costs one
 * of the values in X (there are M - 1 such cuts), and cutting along a vertical
 * line costs one of the values in Y (there are N - 1 such cuts).
 *
 * A cut must pass through every current piece along that line, so its cost is
 * multiplied by the number of pieces it crosses. Perform the most expensive
 * cuts first so that they cross as few pieces as possible.
 *
 * Input:  M = 6, N = 4
 *         X (horizontal) = {2, 1, 3, 1, 4}
 *         Y (vertical)   = {4, 1, 2}
 * Output: 42
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

long long minCostToCut(int rows, int cols, vector<long long> &horizontal,
                       vector<long long> &vertical) {
    sort(horizontal.begin(), horizontal.end(), greater<long long>());
    sort(vertical.begin(), vertical.end(), greater<long long>());

    long long horizontalPieces = 1;  // current number of rows
    long long verticalPieces = 1;    // current number of columns
    long long totalCost = 0;

    int h = 0;
    int v = 0;

    while (h < horizontal.size() && v < vertical.size()) {
        if (horizontal[h] >= vertical[v]) {
            // A horizontal cut crosses every vertical piece.
            totalCost += horizontal[h] * verticalPieces;
            horizontalPieces++;
            h++;
        } else {
            // A vertical cut crosses every horizontal piece.
            totalCost += vertical[v] * horizontalPieces;
            verticalPieces++;
            v++;
        }
    }

    while (h < horizontal.size()) {
        totalCost += horizontal[h] * verticalPieces;
        horizontalPieces++;
        h++;
    }

    while (v < vertical.size()) {
        totalCost += vertical[v] * horizontalPieces;
        verticalPieces++;
        v++;
    }

    return totalCost;
}

int main() {
    int rows, cols;
    cin >> rows >> cols;

    vector<long long> horizontal(rows - 1);
    vector<long long> vertical(cols - 1);

    for (long long &cost : horizontal) {
        cin >> cost;
    }
    for (long long &cost : vertical) {
        cin >> cost;
    }

    cout << minCostToCut(rows, cols, horizontal, vertical) << endl;

    return 0;
}

/**
 * Input used to verify the program:
 *   6 4
 *   2 1 3 1 4
 *   4 1 2
 * Output: 42
 *
 * Time complexity -> O(M log M + N log N) for sorting both cut lists
 * Space complexity -> O(1) extra beyond the input vectors
 *
 * Special cases: a 1 x 1 board (no cuts, cost 0), only horizontal cuts, only
 * vertical cuts, equal cut costs (ties can be processed in either order), and
 * large costs (use long long).
 */

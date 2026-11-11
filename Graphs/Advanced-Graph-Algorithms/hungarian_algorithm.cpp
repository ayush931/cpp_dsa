/**
 * Hungarian Algorithm for the Assignment Problem
 *
 * Given an n x n cost matrix, assign exactly one worker to each job so that
 * the total cost is minimum. The Hungarian algorithm (Kuhn-Munkres) runs in
 * O(n^3) time using dual potentials and alternating augmenting paths.
 *
 * The implementation below uses the shortest augmenting path formulation with
 * potentials u (rows) and v (columns). It supports negative costs as long as
 * the matrix is finite.
 *
 * A brute-force permutation search is included for n <= 8 and is used to
 * verify every test case.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
#include <vector>
using namespace std;

const long long INF = numeric_limits<long long>::max() / 4;

long long hungarian(const vector<vector<long long>> &cost,
                    vector<int> &assignment) {
    int n = static_cast<int>(cost.size());
    assignment.assign(n, -1);

    if (n == 0) {
        return 0;
    }

    vector<long long> rowPotential(n + 1, 0);
    vector<long long> columnPotential(n + 1, 0);
    vector<int> matchedRow(n + 1, 0);
    vector<int> previousColumn(n + 1, 0);

    for (int row = 1; row <= n; row++) {
        matchedRow[0] = row;
        int currentColumn = 0;
        vector<long long> minimumReducedCost(n + 1, INF);
        vector<bool> used(n + 1, false);

        do {
            used[currentColumn] = true;
            int currentRow = matchedRow[currentColumn];
            long long delta = INF;
            int nextColumn = 0;

            for (int column = 1; column <= n; column++) {
                if (used[column]) {
                    continue;
                }

                long long reducedCost = cost[currentRow - 1][column - 1] -
                                        rowPotential[currentRow] -
                                        columnPotential[column];

                if (reducedCost < minimumReducedCost[column]) {
                    minimumReducedCost[column] = reducedCost;
                    previousColumn[column] = currentColumn;
                }

                if (minimumReducedCost[column] < delta) {
                    delta = minimumReducedCost[column];
                    nextColumn = column;
                }
            }

            for (int column = 0; column <= n; column++) {
                if (used[column]) {
                    rowPotential[matchedRow[column]] += delta;
                    columnPotential[column] -= delta;
                } else {
                    minimumReducedCost[column] -= delta;
                }
            }

            currentColumn = nextColumn;
        } while (matchedRow[currentColumn] != 0);

        do {
            int nextColumn = previousColumn[currentColumn];
            matchedRow[currentColumn] = matchedRow[nextColumn];
            currentColumn = nextColumn;
        } while (currentColumn != 0);
    }

    long long total = 0;

    for (int column = 1; column <= n; column++) {
        int row = matchedRow[column] - 1;
        assignment[row] = column - 1;
        total += cost[row][column - 1];
    }

    return total;
}

long long bruteForceAssignment(const vector<vector<long long>> &cost) {
    int n = static_cast<int>(cost.size());
    if (n == 0) {
        return 0;
    }

    vector<int> permutation(n);
    iota(permutation.begin(), permutation.end(), 0);

    long long best = INF;

    do {
        long long total = 0;
        for (int row = 0; row < n; row++) {
            total += cost[row][permutation[row]];
        }
        best = min(best, total);
    } while (next_permutation(permutation.begin(), permutation.end()));

    return best;
}

void printAssignment(const vector<int> &assignment) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(assignment.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << "worker " << i << " -> job " << assignment[i];
    }

    cout << "]";
}

void runTest(const vector<vector<long long>> &cost) {
    vector<int> assignment;
    long long actual = hungarian(cost, assignment);
    long long expected = bruteForceAssignment(cost);

    cout << "cost: " << actual << ", brute force: " << expected
         << ", matches: " << (actual == expected ? "yes" : "no") << endl;
    cout << "assignment: ";
    printAssignment(assignment);
    cout << endl;
    cout << "---" << endl;
}

int main() {
    vector<vector<long long>> classic = {
        {9, 2, 7, 8}, {6, 4, 3, 7}, {5, 8, 1, 8}, {7, 6, 9, 4}};

    vector<vector<long long>> negative = {
        {-5, -1, -3}, {-2, -8, -4}, {-6, -7, -9}};

    vector<vector<long long>> rectangularLike = {
        {1, 2, 3}, {2, 4, 6}, {3, 6, 9}};

    runTest(classic);
    runTest(negative);
    runTest(rectangularLike);
    runTest({{42}});
    runTest({});

    return 0;
}

// Expected output (abridged):
// cost: 13, brute force: 13, matches: yes
// assignment: [worker 0 -> job 1, worker 1 -> job 0, worker 2 -> job 2,
//              worker 3 -> job 3]
// ---
// cost: -22, brute force: -22, matches: yes
// ---
// cost: 10, brute force: 10, matches: yes
// ---
//
// Special cases: empty matrix, 1 x 1 matrix, negative costs, duplicate costs,
// and multiple optimal assignments (only the total cost is compared).

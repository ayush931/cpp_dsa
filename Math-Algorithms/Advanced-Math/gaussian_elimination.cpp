/**
 * Gaussian Elimination
 *
 * Solve a system of linear equations
 *   A * x = b
 * and compute the rank and determinant of A.
 *
 * The algorithm applies elementary row operations to the augmented matrix
 * [A | b] until it is in reduced row echelon form:
 *   - swap rows to put the largest absolute pivot in the current row
 *     (partial pivoting, which keeps the division stable)
 *   - divide the pivot row by the pivot
 *   - subtract a multiple of the pivot row from every other row
 *
 * After elimination:
 *   - a row [0 ... 0 | nonzero] means the system is inconsistent -> 0 solutions
 *   - otherwise, rank < number of variables means there are free variables ->
 *     infinitely many solutions
 *   - otherwise, rank == number of variables means exactly one solution
 *
 * Complexity -> O(rows * columns * min(rows, columns)) time, O(rows * columns)
 * space. For a square n x n matrix that is O(n^3).
 *
 * Determinant:
 *   - the product of the pivots before normalization
 *   - every row swap flips the sign
 *   - a zero pivot means the matrix is singular -> determinant 0
 *
 * Rank:
 *   - the number of nonzero pivots in row echelon form
 *
 * The code uses double and an epsilon. For exact arithmetic, use rational
 * numbers, a modular prime, or a symbolic library. For very ill-conditioned
 * matrices, use QR or SVD instead of normal equations.
 */

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

const double EPSILON = 1e-9;

struct LinearSystemResult {
    int solutions;            // 0 = none, 1 = unique, 2 = infinite
    vector<double> solution;  // one solution when solutions != 0
    int rank;
};

LinearSystemResult solveLinearSystem(vector<vector<double>> coefficients,
                                     vector<double> constants) {
    int rows = coefficients.size();
    int columns = rows > 0 ? coefficients[0].size() : 0;

    vector<vector<double>> matrix(rows, vector<double>(columns + 1, 0));

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            matrix[row][column] = coefficients[row][column];
        }

        matrix[row][columns] = constants[row];
    }

    vector<int> pivotColumn;
    int pivotRow = 0;

    for (int column = 0; column < columns && pivotRow < rows; column++) {
        int bestRow = pivotRow;

        for (int row = pivotRow + 1; row < rows; row++) {
            if (abs(matrix[row][column]) > abs(matrix[bestRow][column])) {
                bestRow = row;
            }
        }

        if (abs(matrix[bestRow][column]) < EPSILON) {
            continue;  // free variable
        }

        swap(matrix[pivotRow], matrix[bestRow]);

        double pivot = matrix[pivotRow][column];

        for (int c = column; c <= columns; c++) {
            matrix[pivotRow][c] /= pivot;
        }

        // Eliminate this column from every other row, not just the rows below.
        for (int row = 0; row < rows; row++) {
            if (row == pivotRow) {
                continue;
            }

            double factor = matrix[row][column];
            if (abs(factor) < EPSILON) {
                continue;
            }

            for (int c = column; c <= columns; c++) {
                matrix[row][c] -= factor * matrix[pivotRow][c];
            }
        }

        pivotColumn.push_back(column);
        pivotRow++;
    }

    int rank = pivotRow;

    // A remaining row with a nonzero right-hand side is a contradiction.
    for (int row = rank; row < rows; row++) {
        bool allZero = true;

        for (int column = 0; column < columns; column++) {
            if (abs(matrix[row][column]) > EPSILON) {
                allZero = false;
                break;
            }
        }

        if (allZero && abs(matrix[row][columns]) > EPSILON) {
            return {0, {}, rank};
        }
    }

    vector<double> solution(columns, 0);

    for (int row = 0; row < rank; row++) {
        solution[pivotColumn[row]] = matrix[row][columns];
    }

    if (rank < columns) {
        return {2, solution, rank};  // free variables were set to zero
    }

    return {1, solution, rank};
}

double determinant(vector<vector<double>> matrix) {
    int n = matrix.size();
    double result = 1;

    for (int column = 0; column < n; column++) {
        int bestRow = column;

        for (int row = column + 1; row < n; row++) {
            if (abs(matrix[row][column]) > abs(matrix[bestRow][column])) {
                bestRow = row;
            }
        }

        if (abs(matrix[bestRow][column]) < EPSILON) {
            return 0;
        }

        if (bestRow != column) {
            swap(matrix[bestRow], matrix[column]);
            result = -result;
        }

        result *= matrix[column][column];

        for (int row = column + 1; row < n; row++) {
            double factor = matrix[row][column] / matrix[column][column];

            for (int c = column; c < n; c++) {
                matrix[row][c] -= factor * matrix[column][c];
            }
        }
    }

    return result;
}

int matrixRank(vector<vector<double>> matrix) {
    int rows = matrix.size();
    if (rows == 0) {
        return 0;
    }

    int columns = matrix[0].size();
    int rank = 0;

    for (int column = 0; column < columns && rank < rows; column++) {
        int bestRow = rank;

        for (int row = rank + 1; row < rows; row++) {
            if (abs(matrix[row][column]) > abs(matrix[bestRow][column])) {
                bestRow = row;
            }
        }

        if (abs(matrix[bestRow][column]) < EPSILON) {
            continue;
        }

        swap(matrix[rank], matrix[bestRow]);

        double pivot = matrix[rank][column];

        for (int c = column; c < columns; c++) {
            matrix[rank][c] /= pivot;
        }

        for (int row = rank + 1; row < rows; row++) {
            double factor = matrix[row][column];

            for (int c = column; c < columns; c++) {
                matrix[row][c] -= factor * matrix[rank][c];
            }
        }

        rank++;
    }

    return rank;
}

void printSolution(const LinearSystemResult &result) {
    cout << result.solutions << " rank=" << result.rank << " x=";

    for (double value : result.solution) {
        cout << value << " ";
    }

    cout << endl;
}

int main() {
    cout << fixed << setprecision(6);

    // Unique solution: x = 1, y = 2, z = 3.
    vector<vector<double>> coefficients = {{1, 1, 1}, {2, -1, 1}, {1, 2, -1}};

    vector<double> constants = {6, 3, 2};

    LinearSystemResult unique = solveLinearSystem(coefficients, constants);
    printSolution(unique);
    // 1 rank=3 x=1.000000 2.000000 3.000000

    cout << determinant(coefficients) << endl;  // 7.000000
    cout << matrixRank(coefficients) << endl;   // 3

    // Infinitely many solutions: x + y = 2 and 2x + 2y = 4 are the same line.
    LinearSystemResult infinite = solveLinearSystem({{1, 1}, {2, 2}}, {2, 4});

    printSolution(infinite);
    // 2 rank=1 x=2.000000 0.000000

    // No solution: x + y = 2 and x + y = 3 are parallel lines.
    LinearSystemResult none = solveLinearSystem({{1, 1}, {1, 1}}, {2, 3});

    printSolution(none);
    // 0 rank=1 x=

    // A singular matrix has determinant 0.
    cout << determinant({{1, 2}, {2, 4}}) << endl;  // 0.000000
    cout << matrixRank({{1, 2}, {2, 4}}) << endl;   // 1

    // The zero matrix has rank 0.
    cout << matrixRank({{0, 0}, {0, 0}}) << endl;  // 0

    // An overdetermined but consistent system.
    LinearSystemResult overdetermined =
        solveLinearSystem({{1, 1}, {1, -1}, {2, 0}}, {4, 0, 4});

    printSolution(overdetermined);
    // 1 rank=2 x=2.000000 2.000000

    // An overdetermined and inconsistent system.
    LinearSystemResult inconsistent =
        solveLinearSystem({{1, 1}, {1, -1}, {2, 0}}, {4, 0, 5});

    printSolution(inconsistent);
    // 0 rank=2 x=

    // A system with more variables than equations: two free variables.
    LinearSystemResult underdetermined = solveLinearSystem({{1, 1, 1}}, {6});

    printSolution(underdetermined);
    // 2 rank=1 x=6.000000 0.000000 0.000000

    return 0;
}

/**
 * Time complexity:
 *   solve -> O(rows * columns * min(rows, columns)), O(n^3) for square n x n
 *   determinant -> O(n^3)
 *   rank -> O(rows * columns * min(rows, columns))
 * Space complexity -> O(rows * columns) for the augmented matrix
 *
 * Special cases:
 *   - an empty system: rows = 0; handle it in the caller
 *   - a zero row with a zero right-hand side: harmless, does not add rank
 *   - a zero row with a nonzero right-hand side: inconsistent
 *   - rank < variables: infinitely many solutions, free variables set to 0
 *   - rank == variables: unique solution
 *   - a singular matrix: determinant 0, rank < n
 *   - a nearly singular matrix: a tiny pivot can amplify rounding error; use
 *     a tolerance, scaled partial pivoting, or a more stable factorization
 *   - a matrix with more rows than columns: an overdetermined system may still
 *     have a unique solution if the extra equations are consistent
 *
 * Pivoting is not optional in floating-point code. Without it, a zero pivot
 * can appear even when the matrix is nonsingular, and small pivots can produce
 * huge intermediate values. Partial pivoting chooses the largest available
 * pivot in the column, which avoids dividing by a tiny pivot. The Gauss-Jordan
 * step that clears the column above the pivot can still use multipliers larger
 * than 1.
 *
 * For exact solutions, replace double with a rational type or run the same
 * elimination modulo a prime. For least-squares problems, form the normal
 * equations only as a last resort; QR decomposition is more stable.
 */

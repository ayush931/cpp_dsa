/**
 * Divide and Conquer Multiplication: Karatsuba and Strassen
 *
 * Karatsuba multiplies two n-digit numbers with three recursive products
 * instead of four:
 *   x = a * 10^h + b, y = c * 10^h + d
 *   x * y = ac * 10^(2h) + ((a + b)(c + d) - ac - bd) * 10^h + bd
 *
 * Strassen multiplies two n x n matrices with seven recursive products
 * instead of eight, using additions and subtractions to form the blocks.
 *
 * Example:
 *   1234 * 5678 = 7006652
 *   123456 * 654321 = 80779853376
 *
 *   {{1, 2}, {3, 4}} * {{5, 6}, {7, 8}} = {{19, 22}, {43, 50}}
 *
 * Time complexity:
 *   Karatsuba -> O(n^log2(3)) = O(n^1.585)
 *   Strassen  -> O(n^log2(7)) = O(n^2.807)
 * Space complexity -> O(n) recursion and temporary blocks
 *
 * The long long version below is a teaching implementation. Real big-integer
 * code stores decimal digits in vectors (see
 * 96_Math_Algorithms/09_big_integer.cpp).
 */

#include <iostream>
#include <vector>
using namespace std;

int digitCount(long long value) {
    if (value == 0) {
        return 1;
    }

    int digits = 0;
    while (value > 0) {
        digits++;
        value /= 10;
    }

    return digits;
}

long long powerOfTen(int exponent) {
    long long result = 1;

    for (int i = 0; i < exponent; i++) {
        result *= 10;
    }

    return result;
}

long long schoolbook(long long x, long long y) {
    return x * y;
}

long long karatsuba(long long x, long long y) {
    if (x < 10 || y < 10) {
        return x * y;
    }

    int digits = max(digitCount(x), digitCount(y));
    int half = digits / 2;
    long long split = powerOfTen(half);

    long long a = x / split;
    long long b = x % split;
    long long c = y / split;
    long long d = y % split;

    long long ac = karatsuba(a, c);
    long long bd = karatsuba(b, d);
    long long middle = karatsuba(a + b, c + d) - ac - bd;

    return ac * powerOfTen(2 * half) + middle * split + bd;
}

using Matrix = vector<vector<long long>>;

Matrix addMatrix(const Matrix &a, const Matrix &b) {
    int n = a.size();
    Matrix result(n, vector<long long>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }

    return result;
}

Matrix subtractMatrix(const Matrix &a, const Matrix &b) {
    int n = a.size();
    Matrix result(n, vector<long long>(n));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }

    return result;
}

Matrix standardMultiply(const Matrix &a, const Matrix &b,
                        long long &multiplications) {
    int n = a.size();
    Matrix result(n, vector<long long>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                result[i][j] += a[i][k] * b[k][j];
                multiplications++;
            }
        }
    }

    return result;
}

Matrix strassen(const Matrix &a, const Matrix &b, long long &multiplications) {
    int n = a.size();

    if (n == 1) {
        multiplications++;
        return {{a[0][0] * b[0][0]}};
    }

    int half = n / 2;

    Matrix a11(half, vector<long long>(half));
    Matrix a12(half, vector<long long>(half));
    Matrix a21(half, vector<long long>(half));
    Matrix a22(half, vector<long long>(half));
    Matrix b11(half, vector<long long>(half));
    Matrix b12(half, vector<long long>(half));
    Matrix b21(half, vector<long long>(half));
    Matrix b22(half, vector<long long>(half));

    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            a11[i][j] = a[i][j];
            a12[i][j] = a[i][j + half];
            a21[i][j] = a[i + half][j];
            a22[i][j] = a[i + half][j + half];

            b11[i][j] = b[i][j];
            b12[i][j] = b[i][j + half];
            b21[i][j] = b[i + half][j];
            b22[i][j] = b[i + half][j + half];
        }
    }

    Matrix m1 =
        strassen(addMatrix(a11, a22), addMatrix(b11, b22), multiplications);
    Matrix m2 = strassen(addMatrix(a21, a22), b11, multiplications);
    Matrix m3 = strassen(a11, subtractMatrix(b12, b22), multiplications);
    Matrix m4 = strassen(a22, subtractMatrix(b21, b11), multiplications);
    Matrix m5 = strassen(addMatrix(a11, a12), b22, multiplications);
    Matrix m6 = strassen(subtractMatrix(a21, a11), addMatrix(b11, b12),
                         multiplications);
    Matrix m7 = strassen(subtractMatrix(a12, a22), addMatrix(b21, b22),
                         multiplications);

    Matrix c11 = addMatrix(subtractMatrix(addMatrix(m1, m4), m5), m7);
    Matrix c12 = addMatrix(m3, m5);
    Matrix c21 = addMatrix(m2, m4);
    Matrix c22 = addMatrix(subtractMatrix(addMatrix(m1, m3), m2), m6);

    Matrix result(n, vector<long long>(n));

    for (int i = 0; i < half; i++) {
        for (int j = 0; j < half; j++) {
            result[i][j] = c11[i][j];
            result[i][j + half] = c12[i][j];
            result[i + half][j] = c21[i][j];
            result[i + half][j + half] = c22[i][j];
        }
    }

    return result;
}

Matrix padToPowerOfTwo(const Matrix &matrix) {
    int n = matrix.size();
    int size = 1;

    while (size < n) {
        size *= 2;
    }

    Matrix padded(size, vector<long long>(size, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            padded[i][j] = matrix[i][j];
        }
    }

    return padded;
}

void printMatrix(const Matrix &matrix) {
    for (const auto &row : matrix) {
        for (long long value : row) {
            cout << value << " ";
        }
        cout << endl;
    }
}

int main() {
    cout << schoolbook(1234, 5678) << endl;  // 7006652
    cout << karatsuba(1234, 5678) << endl;   // 7006652

    cout << schoolbook(123456, 654321) << endl;  // 80779853376
    cout << karatsuba(123456, 654321) << endl;   // 80779853376

    Matrix a = {{1, 2}, {3, 4}};
    Matrix b = {{5, 6}, {7, 8}};

    long long standardCount = 0;
    Matrix standard = standardMultiply(a, b, standardCount);
    printMatrix(standard);  // 19 22 / 43 50

    long long strassenCount = 0;
    Matrix fast = strassen(a, b, strassenCount);
    printMatrix(fast);  // 19 22 / 43 50

    cout << standardCount << " " << strassenCount << endl;  // 8 7

    Matrix largeA = {
        {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};

    Matrix largeB = {{1, 0, 0, 1}, {0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 1, 1}};

    long long standardLargeCount = 0;
    Matrix standardLarge = standardMultiply(largeA, largeB, standardLargeCount);

    long long strassenLargeCount = 0;
    Matrix strassenLarge = strassen(largeA, largeB, strassenLargeCount);

    printMatrix(standardLarge);
    printMatrix(strassenLarge);

    cout << standardLargeCount << " " << strassenLargeCount << endl;  // 64 49

    Matrix padded = padToPowerOfTwo({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    cout << padded.size() << endl;  // 4

    return 0;
}

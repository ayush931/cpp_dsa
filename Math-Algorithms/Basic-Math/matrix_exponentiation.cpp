/**
 * Matrix exponentiation -> computes M^n in O(k^3 log n) for a k x k matrix
 * Used when a recurrence can be written as a matrix multiplication
 * Fibonacci -> [F(n+1), F(n)] = [[1, 1], [1, 0]]^n * [F(1), F(0)]
 */

#include <iostream>
#include <vector>
using namespace std;

const long long MOD = 1e9 + 7;

vector<vector<long long>> multiply(vector<vector<long long>> &a,
                                   vector<vector<long long>> &b) {
    int n = a.size();
    vector<vector<long long>> result(n, vector<long long>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                result[i][j] = (result[i][j] + a[i][k] * b[k][j]) % MOD;
            }
        }
    }

    return result;
};

vector<vector<long long>> matrixPower(vector<vector<long long>> matrix,
                                      long long exp) {
    int n = matrix.size();

    vector<vector<long long>> result(n, vector<long long>(n, 0));
    for (int i = 0; i < n; i++) {
        result[i][i] = 1;  // identity matrix
    }

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = multiply(result, matrix);
        }
        matrix = multiply(matrix, matrix);
        exp /= 2;
    }

    return result;
};

long long fibonacci(long long n) {
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    vector<vector<long long>> base = {{1, 1}, {1, 0}};
    vector<vector<long long>> power = matrixPower(base, n - 1);

    return power[0][0];
};

int main() {
    cout << fibonacci(10) << endl;  // 55
    cout << fibonacci(50) << endl;  // 12586269025 % MOD
    cout << fibonacci(100) << endl;

    return 0;
}

// Time complexity -> O(k^3 log n)
// Space complexity -> O(k^2)

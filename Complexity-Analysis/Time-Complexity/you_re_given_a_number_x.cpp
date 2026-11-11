// You're given a number x and y (1 <= x <= 10^5, x <= y <= 10^8). Calculate the
// sum of all the numbers in the range [x, y].

#include <iostream>
using namespace std;

long long sum_in_range(int x, int y) {
    long long result = 0;

    for (int i = x; i <= y; i++) {
        result = result + i;
    }

    return result;
}

long long sum_in_range_optimized(int x, int y) {
    long long n = (long long)y - x + 1;
    long long a = x;

    long long result = (n * (2 * a + (n - 1))) / 2;

    return result;
}

int main() {
    cout << sum_in_range_optimized(2, 6) << endl;
    cout << sum_in_range(2, 6) << endl;
    cout << sum_in_range_optimized(1, 100000000) << endl;

    return 0;
}

// Given a number n. Find the sum of natural number till n but with alternate
// signs.

#include <iostream>
using namespace std;

int f(int n) {
    // base case: no terms left
    if (n <= 0) {
        return 0;
    }
    // assumption
    return f(n - 1) +
           ((n % 2 == 0)
                ? (-n)
                : (n));  // calculate sum of first n - 1 natural number with
                         // alternate, and we will add contribution of n
}

int main() {
    cout << f(5) << "\n";

    return 0;
}
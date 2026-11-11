/**
 * Recurrences and loop invariants. Prerequisite: loops and functions.
 * For T(n)=a*T(n/b)+f(n), compare f(n) with n^(log_b(a)).
 * Master theorem (a>=1, b>1), with polynomial gaps epsilon>0:
 * 1. f(n)=O(n^(log_b(a)-epsilon)): Theta(n^(log_b(a))).
 * 2. f(n)=Theta(n^(log_b(a))*log^k(n)), k>=0:
 *    Theta(n^(log_b(a))*log^(k+1)(n)).
 * 3. f(n)=Omega(n^(log_b(a)+epsilon)), plus regularity
 *    a*f(n/b)<=c*f(n) for some c<1 eventually: Theta(f(n)).
 * Examples: binary search Theta(log n); merge sort Theta(n log n);
 * T(n)=2T(n/2)+1 is Theta(n). T(n)=T(n-1)+n is outside this form:
 * expansion gives n+(n-1)+...+1=Theta(n^2).
 * Read this analysis now; revisit its recursive examples after Recursion.
 */

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

long long sum(const vector<int> &values) {
    long long total = 0;
    // Invariant: before iteration i, total equals the sum of values[0..i).
    // Initialization: empty prefix sums to 0.
    // Maintenance: adding values[i] extends the processed prefix by one.
    // Termination: prefix is the whole array, so total is the required sum.
    for (int value : values) {
        total += value;
    }
    return total;
}

int main() {
    assert(sum({}) == 0);
    assert(sum({3, -5, 7}) == 5);
    cout << sum({3, -5, 7}) << '\n';  // 5; O(n) time, O(1) extra space
}

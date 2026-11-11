/**
 * Branch prediction and [[likely]] / [[unlikely]]
 *
 * Modern CPUs execute instructions ahead of time and guess which way a branch
 * will go. A correct guess is almost free; a wrong guess flushes the pipeline
 * and can cost tens of cycles.
 *
 * C++20 attributes:
 *   if (condition) [[likely]] { ... }
 *   if (condition) [[unlikely]] { ... }
 *
 * They are hints, not guarantees. The compiler may ignore them, and a wrong
 * hint can make performance worse. Use them only after profiling shows a hot
 * branch with a strong bias.
 *
 * GCC/Clang also provide __builtin_expect(expression, expectedValue), which is
 * what the attributes usually lower to.
 *
 * Other ways to reduce branch cost:
 *   - sort data so that the common case is grouped together
 *   - use branchless arithmetic (min/max, masks, conditional moves)
 *   - move the rare case out of the hot loop
 *   - use a lookup table instead of a chain of if/else
 */

#include <iostream>
#include <vector>
using namespace std;

int classify(int value) {
    if (value >= 0) [[likely]] {
        return value * 2;  // the common case
    } else [[unlikely]] {
        return -1;  // an error or exceptional input
    }
}

int classifyBuiltin(int value) {
    if (__builtin_expect(value >= 0, 1)) {
        return value * 2;
    }

    return -1;
}

// Branchless absolute value for a 32-bit integer.
// The mask is all ones when value is negative, all zeros otherwise.
// INT_MIN is not supported: its absolute value is not representable in int.
int absoluteValueBranchless(int value) {
    int mask = value >> 31;
    return (value ^ mask) - mask;
}

// A branchless clamp.
int clampBranchless(int value, int low, int high) {
    int maskLow = value < low ? -1 : 0;
    value = (value & ~maskLow) | (low & maskLow);

    int maskHigh = value > high ? -1 : 0;
    value = (value & ~maskHigh) | (high & maskHigh);

    return value;
}

int main() {
    cout << classify(10) << endl;        // 20
    cout << classify(-5) << endl;        // -1
    cout << classifyBuiltin(7) << endl;  // 14

    cout << absoluteValueBranchless(-42) << endl;  // 42
    cout << absoluteValueBranchless(42) << endl;   // 42
    cout << absoluteValueBranchless(0) << endl;    // 0

    cout << clampBranchless(5, 0, 10) << endl;   // 5
    cout << clampBranchless(-5, 0, 10) << endl;  // 0
    cout << clampBranchless(15, 0, 10) << endl;  // 10

    // A sorted input often makes a filtering branch more predictable.
    vector<int> values = {1, 3, 5, 7, 9, 11, 13, 15};

    int evenCount = 0;

    for (int value : values) {
        if (value % 2 == 0) [[unlikely]] {
            evenCount++;
        }
    }

    cout << evenCount << endl;  // 0

    return 0;
}

/**
 * Time complexity -> O(1) per branch; the hint does not change the algorithm
 * Space complexity -> O(1)
 *
 * Special cases:
 *   - [[likely]] on both branches is meaningless; pick the dominant one
 *   - a branch that is 50/50 has no useful hint
 *   - branchless code can be slower when the operation is expensive or when
 *     the compiler already emits a conditional move
 *   - signed right shift of a negative value is implementation-defined before
 *     C++20; C++20 requires arithmetic shift for signed integers
 *   - the attributes are C++20; use __builtin_expect for older standards
 *
 * Measure before optimizing. A hint that helps one CPU or one data distribution
 * can hurt another. The best branch optimization is usually a better algorithm
 * or a data layout that removes the branch entirely.
 */

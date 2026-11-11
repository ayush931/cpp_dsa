// Space-time tradeoff
//
// You can often make an algorithm faster by using more memory, or save memory
// by doing more work. Choosing between the two is a core design decision.
//
// Examples:
//   - duplicate detection: sort (O(1) extra, O(n log n) time) vs hash set
//     (O(n) extra, O(n) time)
//   - range sum queries: scan the range (O(1) extra, O(n) per query) vs prefix
//     sums (O(n) extra, O(1) per query)
//   - Fibonacci: naive recursion (O(1) extra, O(2^n) time) vs memoization
//     (O(n) extra, O(n) time) vs two variables (O(1) extra, O(n) time)

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
using namespace std;

// O(n log n) time, O(1) auxiliary (the copy is the input).
bool containsDuplicateSorted(vector<int> values) {
    sort(values.begin(), values.end());

    for (int i = 1; i < values.size(); i++) {
        if (values[i] == values[i - 1]) {
            return true;
        }
    }

    return false;
}

// O(n) time, O(n) auxiliary.
bool containsDuplicateHashed(vector<int> &values) {
    unordered_set<int> seen;

    for (int value : values) {
        if (seen.count(value)) {
            return true;
        }
        seen.insert(value);
    }

    return false;
}

// O(n) extra, O(1) per query.
class PrefixSums {
    vector<long long> prefix;

   public:
    PrefixSums(vector<int> &values) {
        prefix.assign(values.size() + 1, 0);

        for (int i = 0; i < values.size(); i++) {
            prefix[i + 1] = prefix[i] + values[i];
        }
    }

    long long rangeSum(int left, int right) {
        return prefix[right + 1] - prefix[left];
    }
};

// O(1) extra, O(n) per query.
long long rangeSumSlow(vector<int> &values, int left, int right) {
    long long total = 0;

    for (int i = left; i <= right; i++) {
        total += values[i];
    }

    return total;
}

long long fibonacciTwoVariables(int n) {
    if (n <= 1) {
        return n;
    }

    long long previous = 0;
    long long current = 1;

    for (int i = 2; i <= n; i++) {
        long long next = previous + current;
        previous = current;
        current = next;
    }

    return current;
}

int main() {
    vector<int> values = {4, 2, 7, 2, 9};

    cout << containsDuplicateSorted(values) << endl;  // 1
    cout << containsDuplicateHashed(values) << endl;  // 1

    PrefixSums sums(values);
    cout << sums.rangeSum(1, 3) << endl;         // 11 (2 + 7 + 2)
    cout << rangeSumSlow(values, 1, 3) << endl;  // 11

    cout << fibonacciTwoVariables(50) << endl;  // 12586269025

    // The prefix-sum object costs O(n) memory once and then answers each query
    // in O(1). With q queries, the total is O(n + q) instead of O(n * q).
    return 0;
}

// Time complexity:
//   sort -> O(n log n), hash -> O(n) average
//   prefix build -> O(n), prefix query -> O(1)
//   slow range query -> O(n)
//   Fibonacci with two variables -> O(n) time, O(1) space
// Space complexity:
//   sort -> O(1) auxiliary, hash -> O(n), prefix -> O(n)
//
// Special cases: an empty range (left > right), a single-element range, large
// sums (use long long), duplicate values, a query at the first or last index,
// and memory limits that rule out the O(n) structure.
//
// There is no universal answer. Use the extra memory when queries are frequent
// or the input is large; save it when memory is tight or the input is small.

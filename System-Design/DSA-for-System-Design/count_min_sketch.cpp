/**
 * Count-Min Sketch -> approximate frequency of items in a stream
 * A 2D table of counters, d rows and w columns
 * Every item updates one cell per row, chosen by a different hash per row
 * Query -> minimum of the d counters
 *
 * Never underestimates (counters only increase)
 * May overestimate because of collisions
 * With the required independent hash-family assumptions, width ceil(e/epsilon)
 * and depth ceil(log(1/delta)) give additive error <= epsilon*total with
 * probability >=1-delta. std::hash plus row salts here is a demonstration;
 * it does not establish those independence assumptions.
 *
 * Memory -> O(d * w), independent of the number of distinct items
 * Update / query -> O(d)
 */

#include <climits>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class CountMinSketch {
    int rows;
    int columns;
    vector<vector<long long>> table;

    unsigned int hashValue(string &item, int row) {
        hash<string> hasher;
        return hasher(item + "#" + to_string(row)) % columns;
    }

   public:
    CountMinSketch(int d, int w) {
        if (d <= 0 || w <= 0) {
            throw invalid_argument("sketch dimensions must be positive");
        }
        rows = d;
        columns = w;
        table.assign(rows, vector<long long>(columns, 0));
    }

    void add(string item, long long count = 1) {
        if (count < 0) {
            throw invalid_argument(
                "this sketch accepts non-negative updates only");
        }
        for (int row = 0; row < rows; row++) {
            if (table[row][hashValue(item, row)] >
                numeric_limits<long long>::max() - count) {
                throw overflow_error("sketch counter overflow");
            }
        }
        for (int row = 0; row < rows; row++) {
            table[row][hashValue(item, row)] += count;
        }
    }

    long long estimate(string item) {
        long long minimum = numeric_limits<long long>::max();

        for (int row = 0; row < rows; row++) {
            minimum = min(minimum, table[row][hashValue(item, row)]);
        }

        return minimum;
    }
};

int main() {
    vector<string> stream = {"apple",  "banana", "apple", "cherry", "apple",
                             "banana", "date",   "apple", "cherry", "banana"};

    CountMinSketch sketch(4, 100);

    for (string &item : stream) {
        sketch.add(item);
    }

    cout << sketch.estimate("apple") << endl;   // 4
    cout << sketch.estimate("banana") << endl;  // 3
    cout << sketch.estimate("cherry") << endl;  // 2
    cout << sketch.estimate("grape") << endl;   // 0 or a small overestimate

    return 0;
}

// Special cases: empty stream, item never seen (may be a false positive),
// heavy hitters, rejected negative updates, too few columns (large error).
// Signed storage alone does not preserve the no-underestimate guarantee for
// arbitrary negative updates; that requires a different update model.
// Use cases: heavy hitters, network traffic, trending searches, ad analytics
// Related: Misra-Gries / Space-Saving for frequent items, HyperLogLog for
// distinct count, t-digest for quantiles

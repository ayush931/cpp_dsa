/**
 * Heavy Hitters (Misra-Gries Summary)
 *
 * Given a stream of items, find the items that occur more than n / k times.
 * Storing the exact count of every item needs O(distinct) memory. The
 * Misra-Gries summary uses only k - 1 counters and still guarantees that
 * every true heavy hitter is reported.
 *
 * Algorithm:
 *   - if the item has a counter, increment it
 *   - else if there is a free counter, add the item with count 1
 *   - else decrement every counter by 1 and remove zeros
 *
 * Guarantee: an item with true frequency > n / k is always in the summary.
 * The reported count is a lower bound; it can undercount by at most n / k.
 *
 * Brute force -> exact hash map of all frequencies -> O(distinct) memory
 * Misra-Gries -> O(k) memory, one pass
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class MisraGries {
    int counterLimit;
    unordered_map<string, int> counters;

   public:
    MisraGries(int k) {
        counterLimit = (k > 1) ? k - 1 : 1;
    }

    void add(const string &item) {
        if (counters.count(item)) {
            counters[item]++;
            return;
        }

        if (counters.size() < counterLimit) {
            counters[item] = 1;
            return;
        }

        // No free counter: decrease all counters, then drop zeros.
        for (auto it = counters.begin(); it != counters.end();) {
            it->second--;

            if (it->second == 0) {
                it = counters.erase(it);
            } else {
                ++it;
            }
        }
    }

    vector<pair<string, int>> summary() {
        vector<pair<string, int>> result;

        for (auto &entry : counters) {
            result.push_back(entry);
        }

        return result;
    }
};

vector<pair<string, int>> exactCounts(vector<string> &stream) {
    unordered_map<string, int> counts;

    for (string &item : stream) {
        counts[item]++;
    }

    vector<pair<string, int>> result;

    for (auto &entry : counts) {
        result.push_back(entry);
    }

    return result;
}

void print(const string &label, vector<pair<string, int>> &values) {
    cout << label << ": ";

    for (auto &entry : values) {
        cout << entry.first << "=" << entry.second << " ";
    }

    cout << endl;
}

int main() {
    vector<string> stream = {"a", "a", "a", "b", "b", "c", "c", "c",
                             "c", "d", "d", "e", "e", "e", "e", "e"};

    int k = 4;  // heavy hitter threshold = n / k = 16 / 4 = 4

    MisraGries summary(k);

    for (string &item : stream) {
        summary.add(item);
    }

    vector<pair<string, int>> exact = exactCounts(stream);
    vector<pair<string, int>> approximate = summary.summary();

    print("exact", exact);
    print("misra-gries", approximate);

    // Every item with true frequency > 4 must appear above.
    // Counts in the summary are lower bounds, not exact frequencies.

    return 0;
}

/**
 * Time complexity -> O(1) amortized per item (at most k counters)
 * Space complexity -> O(k)
 *
 * Special cases: k = 1 (one counter), k larger than the number of distinct
 * items (exact counts), an empty stream, all items distinct (the summary may
 * be empty at the end), one item dominates the stream, ties at the threshold.
 *
 * Variants: Space-Saving keeps an upper bound instead of a lower bound;
 * Count-Min Sketch handles a much larger universe with hashing; both are used
 * for trending topics and network heavy hitters.
 */

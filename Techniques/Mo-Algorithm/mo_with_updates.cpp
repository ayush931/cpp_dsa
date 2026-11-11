/**
 * Mo's algorithm with point updates
 *
 * Each query is (left, right, time), where time is the number of updates that
 * have been applied. Point updates are stored as (index, previousValue,
 * newValue).
 *
 * Sort queries by:
 *   1. block of left endpoint
 *   2. block of right endpoint (alternating direction)
 *   3. time (alternating direction)
 *
 * Block size for updates is about n^(2/3), which gives
 * O((n + q) * n^(2/3)) total movement.
 *
 * Current state:
 *   - frequency[value] -> how many times value appears in [currentLeft,
 * currentRight]
 *   - currentSum -> sum of values in the current range
 *
 * Applying an update inside the current range removes the old value and adds
 * the new value; undoing an update swaps the two values.
 *
 * Edge cases: no updates, updates outside the current range, repeated updates
 * to the same index, negative values, duplicate values.
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

struct Query {
    int left;
    int right;
    int time;
    int index;
};

struct Update {
    int index;
    int previousValue;
    int newValue;
};

class MoWithUpdates {
    vector<int> arr;
    vector<Query> queries;
    vector<Update> updates;
    vector<long long> answers;

    int blockSize;
    int currentLeft = 0;
    int currentRight = -1;
    int currentTime = 0;
    long long currentSum = 0;
    map<int, int> frequency;

    void add(int index) {
        int value = arr[index];
        frequency[value]++;
        currentSum += value;
    }

    void remove(int index) {
        int value = arr[index];
        frequency[value]--;
        if (frequency[value] == 0) {
            frequency.erase(value);
        }
        currentSum -= value;
    }

    void applyUpdate(int time) {
        Update &update = updates[time];
        int index = update.index;

        if (index >= currentLeft && index <= currentRight) {
            remove(index);
        }

        arr[index] = update.newValue;

        if (index >= currentLeft && index <= currentRight) {
            add(index);
        }
    }

    void undoUpdate(int time) {
        Update &update = updates[time];
        int index = update.index;

        if (index >= currentLeft && index <= currentRight) {
            remove(index);
        }

        arr[index] = update.previousValue;

        if (index >= currentLeft && index <= currentRight) {
            add(index);
        }
    }

   public:
    MoWithUpdates(vector<int> &input, vector<Query> &inputQueries,
                  vector<Update> &inputUpdates) {
        arr = input;
        queries = inputQueries;
        updates = inputUpdates;

        int n = arr.size();
        blockSize = max(1, (int)pow(n, 2.0 / 3.0));

        sort(queries.begin(), queries.end(), [this](Query &a, Query &b) {
            int blockA = a.left / blockSize;
            int blockB = b.left / blockSize;
            if (blockA != blockB) {
                return blockA < blockB;
            }

            int rightBlockA = a.right / blockSize;
            int rightBlockB = b.right / blockSize;
            if (rightBlockA != rightBlockB) {
                return (blockA % 2 == 0) ? rightBlockA < rightBlockB
                                         : rightBlockA > rightBlockB;
            }

            return (rightBlockA % 2 == 0) ? a.time < b.time : a.time > b.time;
        });

        answers.assign(queries.size(), 0);
    }

    vector<long long> solve() {
        for (Query &query : queries) {
            while (currentTime < query.time) {
                applyUpdate(currentTime++);
            }
            while (currentTime > query.time) {
                undoUpdate(--currentTime);
            }

            while (currentLeft > query.left) {
                add(--currentLeft);
            }
            while (currentRight < query.right) {
                add(++currentRight);
            }
            while (currentLeft < query.left) {
                remove(currentLeft++);
            }
            while (currentRight > query.right) {
                remove(currentRight--);
            }

            answers[query.index] = currentSum;
        }

        return answers;
    }
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // (left, right, time, originalIndex)
    vector<Query> queries = {
        {0, 4, 0, 0}, {2, 7, 1, 1}, {1, 3, 2, 2}, {0, 9, 3, 3}};

    // Each update is (index, previousValue, newValue).
    vector<Update> updates = {{2, 3, 30}, {5, 6, 60}, {2, 30, 300}};

    MoWithUpdates solver(arr, queries, updates);
    vector<long long> answers = solver.solve();

    cout << answers[0] << endl;  // 1+2+3+4+5 = 15
    cout << answers[1]
         << endl;  // 30+4+5+6+7+8 = 60 (index 2 becomes 30 after update 0)
    cout << answers[2]
         << endl;  // 2+30+4 = 36 (update 2 changes index 2 to 300)
    cout << answers[3] << endl;  // 1+2+300+4+5+60+7+8+9+10 = 406

    return 0;
}

// Special cases:
// - time = 0 -> original array
// - an update outside [left, right] changes the array but not the current sum
// - multiple updates to the same index are applied and undone in order
// - block size n^(2/3) is a common choice when updates are present

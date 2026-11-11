/**
 * Binary search on the answer
 *
 * Some problems ask for the smallest value that satisfies a monotone
 * condition:
 *
 *   canFinish(speed)        -> true for every speed >= answer
 *   canShip(capacity)       -> true for every capacity >= answer
 *   canSplit(limit)         -> true for every limit >= answer
 *
 * When the predicate changes from false to true exactly once, binary search
 * finds the boundary in O(log range) checks. The range is over the answer, not
 * over an array index.
 *
 * Three problems are solved here:
 *   1. Koko eating bananas: minimum eating speed in h hours
 *   2. Ship packages within D days: minimum ship capacity
 *   3. Split array largest sum: minimum possible largest piece with m pieces
 *
 * Each problem has a brute-force version that scans the answer space and an
 * optimized binary-search version. The brute-force versions are intentionally
 * simple and are only used to validate the optimized answers.
 *
 * Time        -> brute force O(range * n), optimized O(n log range)
 * Space       -> O(1) extra for each solution
 */

#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// ---------------------------------------------------------------- Koko

long long hoursNeeded(const vector<int> &piles, int speed) {
    long long hours = 0;

    for (int pile : piles) {
        hours += (pile + speed - 1) / speed;
    }

    return hours;
}

int minEatingSpeedBrute(const vector<int> &piles, int hours) {
    if (piles.empty()) {
        return 0;
    }

    int maximum = *max_element(piles.begin(), piles.end());

    for (int speed = 1; speed <= maximum; speed++) {
        if (hoursNeeded(piles, speed) <= hours) {
            return speed;
        }
    }

    return maximum;
}

int minEatingSpeed(const vector<int> &piles, int hours) {
    if (piles.empty()) {
        return 0;
    }

    int low = 1;
    int high = *max_element(piles.begin(), piles.end());

    while (low < high) {
        int middle = low + (high - low) / 2;

        if (hoursNeeded(piles, middle) <= hours) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    return low;
}

// ------------------------------------------------------- Ship packages

long long daysNeeded(const vector<int> &weights, int capacity) {
    if (capacity <= 0) {
        return LLONG_MAX;
    }

    long long days = 1;
    long long load = 0;

    for (int weight : weights) {
        if (load + weight > capacity) {
            days++;
            load = 0;
        }

        load += weight;
    }

    return days;
}

int shipWithinDaysBrute(const vector<int> &weights, int days) {
    if (weights.empty()) {
        return 0;
    }

    int low = *max_element(weights.begin(), weights.end());
    int high = accumulate(weights.begin(), weights.end(), 0);

    for (int capacity = low; capacity <= high; capacity++) {
        if (daysNeeded(weights, capacity) <= days) {
            return capacity;
        }
    }

    return -1;
}

int shipWithinDays(const vector<int> &weights, int days) {
    if (weights.empty()) {
        return 0;
    }

    int low = *max_element(weights.begin(), weights.end());
    int high = accumulate(weights.begin(), weights.end(), 0);

    while (low < high) {
        int middle = low + (high - low) / 2;

        if (daysNeeded(weights, middle) <= days) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    return low;
}

// ------------------------------------------------ Split array largest sum

bool canSplit(const vector<int> &numbers, long long limit, int pieces) {
    int used = 1;
    long long sum = 0;

    for (int number : numbers) {
        if (sum + number > limit) {
            used++;
            sum = 0;
        }

        sum += number;

        if (used > pieces) {
            return false;
        }
    }

    return true;
}

long long splitArrayBrute(const vector<int> &numbers, int pieces) {
    int n = numbers.size();

    if (n == 0) {
        return 0;
    }
    if (pieces <= 0 || pieces > n) {
        return -1;
    }

    long long best = LLONG_MAX;

    function<void(int, int, long long)> search = [&](int index, int piecesLeft,
                                                     long long largest) {
        if (piecesLeft == 1) {
            long long rest = 0;

            for (int i = index; i < n; i++) {
                rest += numbers[i];
            }

            best = min(best, max(largest, rest));
            return;
        }

        long long sum = 0;

        // Leave at least one element for every remaining piece.
        for (int i = index; i <= n - piecesLeft; i++) {
            sum += numbers[i];
            search(i + 1, piecesLeft - 1, max(largest, sum));
        }
    };

    search(0, pieces, 0);
    return best;
}

long long splitArray(const vector<int> &numbers, int pieces) {
    int n = numbers.size();

    if (n == 0) {
        return 0;
    }
    if (pieces <= 0 || pieces > n) {
        return -1;
    }

    long long low = *max_element(numbers.begin(), numbers.end());
    long long high = accumulate(numbers.begin(), numbers.end(), 0LL);

    while (low < high) {
        long long middle = low + (high - low) / 2;

        if (canSplit(numbers, middle, pieces)) {
            high = middle;
        } else {
            low = middle + 1;
        }
    }

    return low;
}

int main() {
    vector<vector<int>> pileTests = {
        {3, 6, 7, 11}, {30, 11, 23, 4, 20}, {1, 1, 1, 1}, {}};
    vector<int> hourLimits = {8, 5, 4, 10};

    for (int i = 0; i < (int)pileTests.size(); i++) {
        cout << "koko " << i + 1
             << ": brute = " << minEatingSpeedBrute(pileTests[i], hourLimits[i])
             << ", binary = " << minEatingSpeed(pileTests[i], hourLimits[i])
             << endl;
    }

    vector<vector<int>> weightTests = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, {3, 2, 2, 4, 1, 4}, {5}, {}};
    vector<int> dayLimits = {5, 3, 1, 2};

    for (int i = 0; i < (int)weightTests.size(); i++) {
        cout << "ship " << i + 1 << ": brute = "
             << shipWithinDaysBrute(weightTests[i], dayLimits[i])
             << ", binary = " << shipWithinDays(weightTests[i], dayLimits[i])
             << endl;
    }

    vector<vector<int>> splitTests = {
        {7, 2, 5, 10, 8}, {1, 2, 3, 4, 5}, {1, 4, 4}, {}};
    vector<int> pieceCounts = {2, 2, 3, 1};

    for (int i = 0; i < (int)splitTests.size(); i++) {
        cout << "split " << i + 1
             << ": brute = " << splitArrayBrute(splitTests[i], pieceCounts[i])
             << ", binary = " << splitArray(splitTests[i], pieceCounts[i])
             << endl;
    }

    cout << "split invalid pieces: " << splitArray({1, 2, 3}, 0) << endl;
    cout << "split too many pieces: " << splitArray({1, 2, 3}, 4) << endl;

    return 0;
}

// Expected output:
// koko 1: brute = 4, binary = 4
// koko 2: brute = 30, binary = 30
// koko 3: brute = 1, binary = 1
// koko 4: brute = 0, binary = 0
// ship 1: brute = 15, binary = 15
// ship 2: brute = 6, binary = 6
// ship 3: brute = 5, binary = 5
// ship 4: brute = 0, binary = 0
// split 1: brute = 18, binary = 18
// split 2: brute = 9, binary = 9
// split 3: brute = 4, binary = 4
// split 4: brute = 0, binary = 0
// split invalid pieces: -1
// split too many pieces: -1
//
// Special cases: empty input, one element, all equal values, pieces equal to
// the array length, and invalid piece counts.
//
// Why binary search works:
//   - Koko: if speed s finishes in time, every speed greater than s also does
//   - Ship: if capacity c works, every larger capacity also works
//   - Split: if limit L works, every larger limit also works
//
// The lower bound is the largest single element because no capacity or piece
// can be smaller than the largest item. The upper bound is the total sum, which
// always works as one piece.

/**
 * Circular tour (gas station)
 *
 * There are n gas stations on a circle. gas[i] is the fuel available at
 * station i and cost[i] is the fuel needed to drive to station i + 1. Return
 * the starting station index that lets you complete the circuit, or -1 if no
 * such station exists. The answer is unique when it exists.
 *
 * Example:
 * gas  = [1, 2, 3, 4, 5]
 * cost = [3, 4, 5, 1, 2]
 * Output: 3
 *
 * Brute force: try every start and simulate the full circle -> O(n^2)
 * Optimized: one pass with a running tank -> O(n) time, O(1) space
 */

#include <iostream>
#include <vector>
using namespace std;

int circularTourBrute(const vector<int> &gas, const vector<int> &cost) {
    int n = gas.size();
    if (n == 0 || cost.size() != gas.size()) {
        return -1;
    }

    for (int start = 0; start < n; start++) {
        long long tank = 0;
        bool possible = true;

        for (int step = 0; step < n; step++) {
            int station = (start + step) % n;
            tank += gas[station] - cost[station];

            if (tank < 0) {
                possible = false;
                break;
            }
        }

        if (possible) {
            return start;
        }
    }

    return -1;
}

int circularTour(const vector<int> &gas, const vector<int> &cost) {
    int n = gas.size();
    if (n == 0 || cost.size() != gas.size()) {
        return -1;
    }

    long long total = 0;
    long long tank = 0;
    int start = 0;

    for (int i = 0; i < n; i++) {
        long long balance = (long long)gas[i] - cost[i];
        total += balance;
        tank += balance;

        // This start cannot reach i + 1, so no station up to i can either.
        if (tank < 0) {
            start = i + 1;
            tank = 0;
        }
    }

    return total >= 0 ? start : -1;
}

int main() {
    vector<int> gas1 = {1, 2, 3, 4, 5};
    vector<int> cost1 = {3, 4, 5, 1, 2};

    vector<int> gas2 = {2, 3, 4};
    vector<int> cost2 = {3, 4, 3};

    vector<int> gas3 = {5};
    vector<int> cost3 = {3};

    vector<int> gas4 = {3};
    vector<int> cost4 = {5};

    cout << circularTourBrute(gas1, cost1) << " " << circularTour(gas1, cost1)
         << endl;  // 3 3

    cout << circularTourBrute(gas2, cost2) << " " << circularTour(gas2, cost2)
         << endl;  // -1 -1

    cout << circularTour(gas3, cost3) << endl;  // 0
    cout << circularTour(gas4, cost4) << endl;  // -1

    cout << circularTour(vector<int>{}, vector<int>{}) << endl;  // -1

    return 0;
}

/**
 * If the total gas is less than the total cost, no tour exists. Otherwise a
 * valid start must exist. Whenever the tank becomes negative at station i,
 * every station from the current start through i fails, so the next possible
 * start is i + 1.
 *
 * Special cases: empty input, one station with enough or too little gas,
 * equal total gas and cost, and a start near the end of the array.
 */

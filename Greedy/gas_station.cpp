/**
 * Gas Station
 *
 * There are n gas stations along a circular route. gas[i] is the amount of gas
 * at station i, and cost[i] is the gas needed to travel to the next station.
 * Return the starting station index if you can complete the circuit, or -1.
 * The answer is guaranteed to be unique when it exists.
 *
 * Example:
 * Input:  gas = [1, 2, 3, 4, 5], cost = [3, 4, 5, 1, 2]
 * Output: 3
 *
 * Constraints: 1 <= n <= 10^5, 0 <= gas[i], cost[i] <= 10^4.
 */

#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every starting station -> O(n^2)
int canCompleteCircuitBrute(vector<int> &gas, vector<int> &cost) {
    int n = gas.size();

    for (int start = 0; start < n; start++) {
        int tank = 0;
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

// Greedy: if the total gas is enough, the station after the last failure works
int canCompleteCircuit(vector<int> &gas, vector<int> &cost) {
    int total = 0;
    int tank = 0;
    int start = 0;

    for (int i = 0; i < gas.size(); i++) {
        int balance = gas[i] - cost[i];
        total += balance;
        tank += balance;

        if (tank < 0) {
            // No station from start through i can be the answer.
            start = i + 1;
            tank = 0;
        }
    }

    return (total >= 0) ? start : -1;
}

int main() {
    vector<int> gas1 = {1, 2, 3, 4, 5};
    vector<int> cost1 = {3, 4, 5, 1, 2};

    vector<int> gas2 = {2, 3, 4};
    vector<int> cost2 = {3, 4, 3};

    vector<int> gas3 = {5};
    vector<int> cost3 = {5};

    cout << canCompleteCircuitBrute(gas1, cost1) << " "
         << canCompleteCircuit(gas1, cost1) << endl;  // 3 3

    cout << canCompleteCircuitBrute(gas2, cost2) << " "
         << canCompleteCircuit(gas2, cost2) << endl;  // -1 -1

    cout << canCompleteCircuit(gas3, cost3) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   greedy -> O(n) time, O(1) space
 *
 * Special cases: one station, total gas < total cost (-1), total gas equals
 * total cost, the answer is station 0, a station with zero gas, a circular
 * route that only works after wrapping around.
 */

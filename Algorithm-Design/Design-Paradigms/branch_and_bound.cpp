/**
 * Branch and bound for 0/1 knapsack
 *
 * Branch and bound is backtracking with a bound. At each node:
 *   - include the current item if it fits
 *   - exclude the current item
 *   - compute an upper bound by filling the remaining capacity with items in
 *     decreasing value/weight order, allowing fractions
 *   - prune the branch when the bound cannot beat the best solution found
 *
 * Worst case -> O(2^n), but pruning often makes it much faster
 * Space -> O(n) recursion depth
 *
 * Edge cases: empty items, capacity 0, an item heavier than the capacity,
 * duplicate value/weight ratios, and all items fitting.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Item {
    int weight;
    int value;
};

class KnapsackBranchAndBound {
    vector<Item> items;
    int capacity;
    int bestValue;

    int bound(int index, int currentWeight, int currentValue) {
        if (currentWeight >= capacity) {
            return currentValue;
        }

        int remaining = capacity - currentWeight;
        int result = currentValue;

        for (int i = index; i < (int)items.size(); i++) {
            if (items[i].weight <= remaining) {
                remaining -= items[i].weight;
                result += items[i].value;
            } else {
                result += (items[i].value * remaining) / items[i].weight;
                break;
            }
        }

        return result;
    }

    void explore(int index, int currentWeight, int currentValue) {
        if (index == (int)items.size()) {
            bestValue = max(bestValue, currentValue);
            return;
        }

        if (bound(index, currentWeight, currentValue) <= bestValue) {
            return;
        }

        // Include the current item.
        if (currentWeight + items[index].weight <= capacity) {
            explore(index + 1, currentWeight + items[index].weight,
                    currentValue + items[index].value);
        }

        // Exclude the current item.
        explore(index + 1, currentWeight, currentValue);
    }

   public:
    KnapsackBranchAndBound(vector<Item> &input, int cap) {
        items = input;
        capacity = cap;
        bestValue = 0;

        sort(items.begin(), items.end(), [](Item &a, Item &b) {
            return (long long)a.value * b.weight >
                   (long long)b.value * a.weight;
        });
    }

    int solve() {
        explore(0, 0, 0);
        return bestValue;
    }
};

int knapsackBrute(vector<Item> &items, int capacity, int index) {
    if (index == (int)items.size()) {
        return 0;
    }

    int skip = knapsackBrute(items, capacity, index + 1);
    int take = 0;

    if (items[index].weight <= capacity) {
        take = items[index].value +
               knapsackBrute(items, capacity - items[index].weight, index + 1);
    }

    return max(skip, take);
}

int main() {
    vector<Item> items = {{2, 40}, {3, 50}, {4, 60}, {5, 70}};
    int capacity = 8;

    KnapsackBranchAndBound solver(items, capacity);
    cout << solver.solve() << endl;                     // 120 (items 3 and 5)
    cout << knapsackBrute(items, capacity, 0) << endl;  // 120

    vector<Item> heavy = {{10, 100}};
    KnapsackBranchAndBound second(heavy, 5);
    cout << second.solve() << endl;  // 0

    vector<Item> empty;
    KnapsackBranchAndBound third(empty, 10);
    cout << third.solve() << endl;  // 0

    return 0;
}

// Special cases:
// - capacity 0 -> 0
// - an item heavier than the capacity is skipped
// - fractional bounds are only an estimate; the final answer is integral
// - sorting by value/weight makes the bound tighter and prunes more branches

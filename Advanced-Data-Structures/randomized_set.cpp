/**
 * Insert/Delete/GetRandom in expected O(1): dense vector + value->index map.
 * Deleting swaps the last value into the removed slot and repairs its index.
 * Uniform index sampling gives uniform VALUE sampling because values are
 * unique. Never use rand()%size (modulo bias); this is not cryptographic RNG.
 * O(n) space. Vector growth is amortized; hashing has O(n) worst-case ops.
 */

#include <cassert>
#include <cstddef>
#include <iostream>
#include <random>
#include <stdexcept>
#include <unordered_map>
#include <vector>
using namespace std;

class RandomizedSet {
    vector<int> values;
    unordered_map<int, size_t> index;
    mt19937 generator;

   public:
    explicit RandomizedSet(unsigned seed = random_device{}())
        : generator(seed) {}

    bool insert(int value) {
        if (index.count(value)) {
            return false;
        }
        index[value] = values.size();
        values.push_back(value);
        return true;
    }

    bool remove(int value) {
        auto found = index.find(value);
        if (found == index.end()) {
            return false;
        }
        size_t position = found->second;
        int last = values.back();
        values[position] = last;
        index[last] = position;  // also works when value itself is last
        values.pop_back();
        index.erase(found);
        return true;
    }

    int getRandom() {
        if (values.empty()) {
            throw out_of_range("cannot sample an empty set");
        }
        uniform_int_distribution<size_t> distribution(0, values.size() - 1);
        return values[distribution(generator)];
    }
};

int main() {
    RandomizedSet numbers(42);  // deterministic seed for this demonstration
    assert(numbers.insert(10));
    assert(numbers.insert(20));
    assert(!numbers.insert(10));
    assert(numbers.remove(10));
    assert(numbers.getRandom() == 20);
    assert(numbers.remove(20));
    assert(!numbers.remove(20));
    bool rejected = false;
    try {
        numbers.getRandom();
    } catch (const out_of_range &) {
        rejected = true;
    }
    assert(rejected);
    cout << "Randomized set checks passed\n";
}

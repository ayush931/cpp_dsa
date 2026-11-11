/**
 * C++17: iterator/reference invalidation depends on container AND operation.
 * vector reallocation: all iterators/references; erase: erased and later
 * positions (including old end). reserve prevents reallocation only up to
 * capacity; it does not make erase/insert positions stable.
 * list/map/set: other elements survive insert/erase.
 * unordered_map rehash: iterators invalidated, references/pointers survive.
 * deque rules differ: do not assume vector or list rules apply to deque.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>
using namespace std;

int main() {
    vector<int> values{1, 2, 3, 4};
    for (auto it = values.begin(); it != values.end();) {
        if (*it % 2 == 0) {
            it = values.erase(it);  // use returned valid iterator
        } else {
            ++it;
        }
    }
    assert((values == vector<int>{1, 3}));
    // erase-remove idiom: remove compacts values; erase changes the size.
    values.erase(remove(values.begin(), values.end(), 1), values.end());
    assert((values == vector<int>{3}));
    list<int> nodes{10, 20};
    auto stable = nodes.begin();
    nodes.push_back(30);
    assert(*stable == 10);
    unordered_map<int, int> table{{1, 42}};
    int &reference = table.at(1);
    table.rehash(
        100);  // do not retain or use an iterator across this operation
    assert(reference == 42);
    cout << "Iterator lifetime checks passed\n";
}

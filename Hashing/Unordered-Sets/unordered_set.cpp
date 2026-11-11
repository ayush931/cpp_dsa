/**
 * Unordered set
 * Values are stored in unordered fashion
 * Unique values, values will be identified by itself, values cannot be modified
 inside set.

 * Insertion, deletion, search -> O(1) ->average time complexity
 */

/**
 * Insert:-
 * Single element -> O(1) average, O(n) -> worst (size >= capacity)
 * Multiple elements -> O(n) average -> n is number of element being inserted
 * O(n(n + 1)) -> worst -> size of unordered selection
 */

/**
 * Deletion:- erase(value), erase(iterator) -> O(1) average, erase(start_itr,
 * end_itr) -> O(n)
 */

/**
 * find() -> O(1) average, O(n) worst
 * count() -> O(1) average, O(n) worst
 * rehash(x):- sets the no of bracket to x or more -> O(n) average, O(n^2) worst
 */

/**
 * Multiset:-
 * It can store duplicate values
 * Ordered -> ascending or descending
 * Value will be identified by itself
 * Value cannot be modified in multiset
 */

/**
 * insert() -> O(logN)
 * Deletion:-
 * erase(value), erase(position) -> O(logN)
 * erase(start_pos, end_pos) -> O(n)
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    multiset<int> ms;  // multiset<int, greater<int>> ms for decreasing value;

    ms.insert(1);
    ms.insert(2);
    ms.insert(3);
    ms.insert(4);
    ms.insert(4);  // allow duplicate entry

    for (auto value : ms) {
        cout << value << " ";
    }
    cout << endl;

    return 0;
}
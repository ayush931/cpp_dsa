/**
 * Iterator -> an object that points to an element of a container
 * begin()  -> first element
 * end()    -> one past the last element
 * rbegin() -> last element (reverse)
 * rend()   -> one before the first element (reverse)
 *
 * Categories:-
 * input, output -> read / write a sequence once
 * forward -> forward_list
 * bidirectional -> list, set, map
 * random access -> vector, deque, array
 */

#include <iostream>
#include <iterator>
#include <list>
#include <vector>
using namespace std;

int main() {
    vector<int> v = {10, 20, 30, 40};

    for (auto it = v.begin(); it != v.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    auto it = v.begin();
    advance(it, 2);
    cout << *it << endl;                      // 30
    cout << *(it + 1) << endl;                // 40 -> random access
    cout << distance(v.begin(), it) << endl;  // 2
    cout << next(it) - v.begin() << endl;     // 3
    cout << prev(it) - v.begin() << endl;     // 1

    list<int> l = {1, 2, 3};
    auto lit = l.begin();
    advance(lit, 1);       // list iterator cannot use + 1
    cout << *lit << endl;  // 2

    return 0;
}

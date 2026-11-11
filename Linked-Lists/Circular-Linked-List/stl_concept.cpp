/**
 * STL -> Standard Template Library
 * Set of template classes for implementing commonly used data structures and
 * functions.
 */

// 3 major components of STL

/**
 * 1. Container:- used to hold the data of the same type
 * eg:-
 * a. vector:- class that defines dynamic array
 * b. list:- class for doubly linked list
 * c. map:- used to store the unique key-value pairs
 * d. set:- used to store unique value
 */

/**
 * 2. Iterator:- used to iterate / traverse the container
 * - pointer like entities
 */

/**
 * 3. Algorithms:- functions that are used to manipulate the data in the
 * containers. eg:- sort(), min(), max()
 */

/**
 * What is list:- template class for implementing the doubly linked list
 *
 * list.begin():- iterator for the first element
 * list.end():- iterator for the position after the last element
 * list.rbegin():- iterator for the first element in reverse iteration
 * list.rend():- iterator for the position after the last element in reverse
 * iteration advance(itr, n):- advances the iterator by n places
 */

/**
 * list.insert(itr, value):- insert value before the position of iterator
 * list.insert(itr, count, value):- insert value count number of times before
 * the iterator list.insert(itr, start_itr, end_itr):- insert values from start
 * iterator to end iterator before iterator
 */

/**
 * list.erase(itr):- delete the element pointed by the iterator
 * list.erase(start_itr, end_itr):- delete element from  start_itr to end_itr
 */

/**
 * push_front(itr):- insert value in the front of the list
 * pop_front(itr):- removes value from front of list
 * push_back(itr):- insert value in the back of the list
 * pop_back(itr):- remove value from the back of the list
 */

#include <iostream>
#include <iterator>
#include <list>
using namespace std;

int main() {
    list<int> l1 = {1, 2, 3, 4};
    auto itr = l1.begin();
    cout << *itr << endl;

    // l1.end() -> iterator pointing to the memory location after 4

    advance(itr, 2);
    cout << *itr << endl;

    // using range based loop
    for (auto num : l1) {
        cout << num << " ";
    }
    cout << endl;

    // using iterator
    for (auto itr = l1.begin(); itr != l1.end(); itr++) {
        cout << *itr << " ";
    }
    cout << endl;

    // reverse traversal
    for (auto itr = l1.rbegin(); itr != l1.rend(); itr++) {
        cout << *itr << " ";
    }
    cout << endl;

    l1.insert(itr, 5);

    for (auto num : l1) {
        cout << num << " ";
    }
    cout << endl;

    l1.insert(itr, 3, 5);

    for (auto num : l1) {
        cout << num << " ";
    }
    cout << endl;

    auto l = l1.begin();
    auto r = l1.begin();

    advance(r, 2);

    l1.insert(itr, l, r);

    for (auto num : l1) {
        cout << num << " ";
    }
    cout << endl;

    auto s_itr = l1.begin();
    advance(s_itr, 2);

    auto e_itr = l1.begin();
    advance(e_itr, 4);

    l1.erase(s_itr, e_itr);
    for (auto num : l1) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}

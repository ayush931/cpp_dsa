/**
 * Set is a STL container used to store unique values
 * Values are stored in ordered state (increasing / decreasing order)
 * No indexing, elements are identified by their own values
 * Once value is inserted in a set, it cannot be modified
 */

/**
 * Advantages of sets:-
 * unique values
 * ordered
 * dynamic size, no overflowing errors
 * faster
 * insertion, deletion, search O(logN)
 */

/**
 * Disadvantages of sets:-
 * cannot access elements using indexing
 * uses more memory than array
 * not suitable for large data sets
 */

/**
 * Member function of a set container
 * size(), begin(), end(), rbegin(), rend()
 * max_size():- maximum number of element a set can hold
 * empty():- returns true if set is empty otherwise false
 * clear():- removes all elements from set
 * find():- returns position of element if present, else returns end iterator
 * count():- returns number of occurances of an element
 * lower_bound():- returns element if present, otherwise returns just greater
 * element upper_bound():- returns the next greater value
 */

#include <iostream>
#include <set>
using namespace std;

int main() {
    set<int>
        st;  //  set<int, greater<int>> st gives the value in decreasing order
    st.insert(1);
    st.insert(2);
    st.insert(3);
    st.insert(4);
    st.insert(5);

    cout << st.size() << endl;

    st.insert(3);

    cout << st.size() << endl;  // size remains same because it added duplicate
                                // value, set contains unique value

    set<int>::iterator itr;

    // traversing a set
    // gives the values in increasing order
    for (itr = st.begin(); itr != st.end(); itr++) {
        cout << *itr << " ";
    }
    cout << endl;

    // using for each loop

    for (auto value : st) {
        cout << value << " ";
    }
    cout << endl;

    // deletion of elements

    st.erase(4);  // O(logn)

    for (auto value : st) {
        cout << value << " ";
    }
    cout << endl;

    auto itr1 = st.begin();
    advance(itr1, 3);  // points to 5 (4 was already erased)
    st.erase(itr1);    // O(logn)

    for (auto value : st) {
        cout << value << " ";
    }
    cout << endl;

    auto start_itr = st.begin();
    start_itr++;  // points to 2

    auto end_itr = st.begin();
    advance(end_itr, 3);  // points to 5

    st.erase(start_itr, end_itr);  // removes 2 and 3, O(n)

    for (auto value : st) {
        cout << value << " ";
    }
    cout << endl;

    // search operations
    if (st.find(4) != st.end()) {
        cout << "Value is present" << endl;
    } else {
        cout << "Value is not present" << endl;
    }

    return 0;
}
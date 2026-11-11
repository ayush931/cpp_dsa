/**
 * Multimap:- STL container, stores key-value pair
 * Elements are stored in an order ascending / descending
 * Duplicate keys are allowed
 * Insert, Delete, Search -> O(logn)
 * Binary search tree
 */

/**
 * m.insert, m.erase -> O(logn)
 * m.erase(start_itr, end_itr) -> O(n)
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    multimap<string, int> directory;

    directory.insert(make_pair("ayush", 432345));
    directory.insert(make_pair("saghvi", 452362));
    directory.insert(make_pair("ayush", 784526));

    // directort["ayush"] = 741523  it will not work

    for (auto pair : directory) {
        cout << "Name - " << pair.first << endl;
        cout << "Phone no - " << pair.second << endl;
    }

    cout << directory.count("ayush") << endl;

    return 0;
}
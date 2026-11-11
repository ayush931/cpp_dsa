/**
 * Unordered Map:- STL container, stores key-value pair
 * Elements are not ordered
 * Keys will be unique
 * Insertion, Deletion, Retrival/search -> O(1)
 */

/**
 * Member function of unordered map
 * Insert, Erase, Find -> O(1) average case, O(n) worst case
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    unordered_map<int, string> record;
    // roll no, name

    record.insert(make_pair(3, "ria"));
    record[1] = "bali";
    record[2] = "animesh";
    record[3] = "payal";                  // update the new value
    record.insert(make_pair(3, "hema"));  // will do nothing

    for (auto pair : record) {
        cout << "Roll no - " << pair.first << endl;
        cout << "Name - " << pair.second << endl;
    }

    return 0;
}
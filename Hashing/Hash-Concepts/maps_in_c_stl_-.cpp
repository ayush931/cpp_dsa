/**
 * Maps in C++ stl:-
 * STL container which stores key-value pairs.
 * The elements are stored in ascending/descending order.
 * Maps cannot have duplicate keys
 * Implement through BST
 */

#include <iostream>
#include <map>
using namespace std;

int main() {
    map<string, int> directory;

    directory["naman"] = 3876;
    directory["animesh"] = 74529;
    directory["ritu"] = 265289;

    for (auto element : directory) {
        cout << "Name - " << element.first << endl;
        cout << "Phone no - " << element.second << endl;
    }

    directory["ritu"] = 741236;  // change the value of the ritu

    directory.insert(make_pair(
        "ritu", 254152));  // as the key already exists, won't do anything
    // Time -> O(logn)

    for (auto element : directory) {
        cout << "Name - " << element.first << endl;
        cout << "Phone no - " << element.second << endl;
    }

    map<string, int>::reverse_iterator itr;
    for (itr = directory.rbegin(); itr != directory.rend(); itr++) {
        cout << itr->first << " - " << itr->second << endl;
    }

    return 0;
}

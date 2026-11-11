// Vectors are dynamic arrays in which we can resize / delete the arrays.

// 1. Declaration:
/*
    #include<vector>
    vector <datatype> vector_name;
    vector <datatype> vector_name (size);
 */

// 2. Size

/*
    vector <int> v;
    v.size() -> length
 */

// 3. Resize

/*
    v.resize (new size);
    v.resize(8);
    v.size() = 8;
*/

// 4. Capacity

/*
    v.capacity()
    capacity >= size
    capacity may be increases with power of 2 which is compiler dependent.
*/

// 5. Add element

/*
    v.push_back(element)
    v.insert(position, element), position is related to position of first
   element v.begin() -> first element v.end() -> last element v.insert(v.begin()
   + 2, 5);
*/

// 6. Delete element

/*
    v.pop_back() -> remove last element
    v.erase(position), position related to first or last element.
*/

#include <iostream>
#include <vector>
using namespace std;

int main() {
    vector<int> v;

    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;

    v.push_back(1);
    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;

    v.push_back(2);
    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;

    v.push_back(3);
    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;

    v.push_back(4);
    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;

    v.push_back(5);
    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;

    v.resize(10);
    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;

    v.pop_back();
    v.pop_back();
    cout << "Size: " << v.size() << endl;
    cout << "Capacity: " << v.capacity() << endl;

    return 0;
}
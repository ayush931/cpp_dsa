// list -> doubly linked list
// forward_list -> singly linked list
// Both allow O(1) insertion and deletion at a known iterator
// Neither supports random access (no indexing)
// list supports bidirectional iterators, forward_list supports forward
// iterators

#include <forward_list>
#include <iostream>
#include <list>
using namespace std;

int main() {
    list<int> l = {1, 2, 3};
    l.push_back(4);
    l.push_front(0);

    auto it = l.begin();
    advance(it, 2);
    l.insert(it, 10);

    l.remove(2);  // removes every occurrence of 2
    l.sort();
    l.reverse();
    l.unique();  // removes consecutive duplicates

    for (int el : l) {
        cout << el << " ";
    }
    cout << endl;

    cout << l.front() << " " << l.back() << endl;

    forward_list<int> fl = {1, 2, 3};
    fl.push_front(0);
    fl.insert_after(fl.begin(), 5);
    fl.remove(2);

    for (int el : fl) {
        cout << el << " ";
    }
    cout << endl;

    return 0;
}

// list -> O(1) push_front, push_back, insert, erase
// forward_list -> O(1) push_front, insert_after, erase_after
// search in both -> O(n)

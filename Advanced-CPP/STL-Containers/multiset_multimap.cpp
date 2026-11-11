// multiset -> ordered set that allows duplicate values
// multimap -> ordered map that allows duplicate keys
// Both are usually implemented as balanced binary search trees
// Insertion, deletion and search -> O(log n)

#include <iostream>
#include <map>
#include <set>
using namespace std;

int main() {
    multiset<int> ms = {3, 1, 3, 2};

    ms.insert(3);
    cout << ms.count(3) << endl;  // 3

    auto it = ms.find(3);
    ms.erase(it);                 // removes only one occurrence
    cout << ms.count(3) << endl;  // 2

    // ms.erase(3); -> removes every occurrence

    for (int el : ms) {
        cout << el << " ";
    }
    cout << endl;

    multimap<string, int> mm;
    mm.insert({"ayush", 1});
    mm.insert({"ayush", 2});
    mm.insert({"riya", 3});

    for (auto &p : mm) {
        cout << p.first << " " << p.second << endl;
    }

    auto range = mm.equal_range("ayush");
    for (auto i = range.first; i != range.second; i++) {
        cout << i->second << " ";
    }
    cout << endl;

    cout << mm.count("ayush") << endl;

    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    set<int> mySet = {1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 4};

    for (auto value : mySet) {
        cout
            << value
            << " ";  // return only the unique element, remove duplicate element
    }
    cout << endl;

    return 0;
}
// Find the last occurrence of an element x in a given array.

#include <iostream>
#include <vector>
using namespace std;

int main() {
    cout << "Enter the element of the vector: " << endl;

    vector<int> v(6);

    for (int i = 0; i < 6; i++) {
        cin >> v[i];
    }

    cout << "Enter x: ";

    int x;
    cin >> x;

    int occurance = -1;

    for (int i = 0; i < v.size(); i++) {
        if (v[i] == x) {
            occurance = i;
        }
    }

    // other method -> traversing from last

    for (int i = v.size() - 1; i >= 0; i--) {
        if (v[i] == x) {
            occurance = i;
            break;
        }
    }

    cout << occurance << endl;

    return 0;
}
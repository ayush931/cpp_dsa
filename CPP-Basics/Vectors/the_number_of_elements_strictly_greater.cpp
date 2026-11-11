// Count the number of elements strictly greater than value x.

#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cout << "Number of element in vector: ";
    cin >> n;

    vector<int> v(n);

    cout << "Enter the element in vector: " << endl;

    for (int i = 0; i < v.size(); i++) {
        cin >> v[i];
    }

    cout << "Enter x: ";
    int x;
    cin >> x;

    int count = 0;

    for (int i = 0; i < v.size(); i++) {
        if (v[i] > x) {
            count++;
        }
    }

    cout << "Number of element greater than x in a vector: " << count << endl;

    return 0;
}
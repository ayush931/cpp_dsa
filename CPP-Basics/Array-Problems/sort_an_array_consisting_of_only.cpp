// Sort an array consisting of only 0s and 1s.

#include <iostream>
#include <vector>
using namespace std;

void sortZerosAndOnes(vector<int> &v) {
    int zeros_count = 0;

    // counting zeros
    for (int element : v) {
        if (element == 0) {
            zeros_count++;
        }
    }

    for (int i = 0; i < v.size(); i++) {
        if (i < zeros_count) {
            v[i] = 0;
        } else {
            v[i] = 1;
        }
    }
}

int main() {
    int size;
    cout << "Enter the size of vector: " << endl;
    cin >> size;

    vector<int> v;

    cout << "Enter the element of vector in 0s and 1s: " << endl;
    for (int i = 0; i < size; i++) {
        int element;
        cin >> element;
        v.push_back(element);
    }

    sortZerosAndOnes(v);

    cout << "Sorted vector is: " << endl;

    for (int i = 0; i < size; i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    return 0;
}
// Repeatedly swap two adjacent elements if in wrong order.

#include <iostream>
#include <vector>
using namespace std;

void bubbleSort(vector<int> &v) {
    int n = v.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
            }
        }
    }
    return;
}

int main() {
    int n;
    cin >> n;

    vector<int> v(n);

    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    bubbleSort(v);

    cout << "Sorted Array is: \n";

    for (int i = 0; i < n; i++) {
        cout << v[i] << " ";
    }
    cout << endl;

    return 0;
}

// Time complexity -> O(n^2)
// Space complexity -> O(1)
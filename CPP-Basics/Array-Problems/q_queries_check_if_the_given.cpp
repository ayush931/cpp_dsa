// Given Q queries, check if the given number is present in the array or not.
// Note:- Value of all the elements in the array is less than 10 to the power 5.

#include <iostream>
#include <vector>
using namespace std;

int main() {
    cout << "Enter the size of vector: " << endl;
    int size;
    cin >> size;

    vector<int> v(size);
    cout << "Enter the element of vector: " << endl;
    for (int i = 0; i < size; i++) {
        cin >> v[i];
    }

    const int N = 1e5 + 10;

    vector<int> freq(N, 0);
    for (int i = 0; i < size; i++) {
        freq[v[i]]++;
    }

    cout << "Enter the no of queries: " << endl;

    int q;
    cin >> q;

    cout << "Enter the queries: " << endl;

    while (q--) {
        int queryElement;
        cin >> queryElement;
        cout << "Frequency of the query: " << freq[queryElement] << endl;
    }

    return 0;
}
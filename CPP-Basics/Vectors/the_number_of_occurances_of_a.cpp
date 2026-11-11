// Count the number of occurances of a particular element x.

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

    int occurence = 0;

    for (int ele : v) {
        if (ele == x) {
            occurence++;
        }
    }

    cout << "Occurence of given x is: " << occurence << endl;
}
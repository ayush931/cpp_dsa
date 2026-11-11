// Taking input from user in an array

#include <iostream>
using namespace std;

int main() {
    char vowel[5];

    // for loop

    for (int idx = 0; idx < 5; idx++) {
        cin >> vowel[idx];
    }

    for (int idx = 0; idx < 5; idx++) {
        cout << vowel[idx] << " ";
    }

    cout << endl;

    // for each

    for (char &element : vowel) {
        cin >> element;
    }

    for (int idx = 0; idx < 5; idx++) {
        cout << vowel[idx] << " ";
    }

    return 0;
}
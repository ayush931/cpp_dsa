#include <iostream>
using namespace std;

int main() {
    int marks;
    cin >> marks;

    if (marks > 33) {
        cout << "Pass" << endl;
    } else {
        cout << "Fail" << endl;
    }

    marks > 33 ? cout << "Pass" << endl : cout << "Fail" << endl;

    return 0;
}
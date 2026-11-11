#include <iostream>
using namespace std;

void changeValue(int &z, int &y) {
    z = 100;
    y = 99;
}

int main() {
    int a = 5;
    int b = 6;

    changeValue(a, b);  // a and b are passed by reference, so the function
                        // changes the originals.
    cout << a << " " << b << endl;

    return 0;
}

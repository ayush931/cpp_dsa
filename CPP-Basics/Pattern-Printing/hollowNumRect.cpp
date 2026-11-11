#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Size of hollow numeric rectangle: ";
    cin >> n;
    for (int i = 1; i <= n - 1; i++) {
        for (int j = 1; j <= n; j++) {
            if (i == 1 || j == 1 || i == n - 1 || j == n) {
                cout << j;
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }

    return 0;
}

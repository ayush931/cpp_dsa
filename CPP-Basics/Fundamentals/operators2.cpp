#include <iostream>
using namespace std;

int main() {
    bool exp1 = true;
    bool exp2 = false;

    cout << (exp1 && exp2) << endl;  // 0 (false)
    cout << (exp1 || exp2) << endl;  // 1 (true)
    cout << (!exp1) << endl;         // 0 (false)
    cout << (!exp2) << endl;         // 1 (true)

    return 0;
}

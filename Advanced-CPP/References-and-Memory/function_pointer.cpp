// Function pointer -> stores the address of a function
// Used for callbacks, runtime selection of a function and generic operations
// Syntax -> returnType (*pointerName)(parameterTypes)

#include <iostream>
using namespace std;

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int operate(int a, int b, int (*operation)(int, int)) {
    return operation(a, b);
};

int main() {
    int (*fp)(int, int) = add;
    cout << fp(2, 3) << endl;                 // 5
    cout << operate(5, 3, subtract) << endl;  // 2

    int (*operations[3])(int, int) = {add, subtract, multiply};
    for (int i = 0; i < 3; i++) {
        cout << operations[i](6, 2) << " ";
    }
    cout << endl;

    auto fp2 = multiply;        // auto deduces the function pointer type
    cout << fp2(4, 5) << endl;  // 20

    return 0;
}

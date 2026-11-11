// How to declare a function

// return_type function_name(parameter1, parameter2,..) {
//  statements;
// }

// Differentiating factor of same function is the number of parameters and
// datatypes of the parameters.

#include <iostream>
using namespace std;

int add(int num1, int num2) {
    int sum = num1 + num2;
    return sum;
}

int add(int num1, int num2, int num3) {
    int sum = num1 + num2 + num3;
    return sum;
}

float add(float num1, float num2) {
    float num = num1 + num2;
    return num;
}

int main() {
    int a = 5;
    int b = 4;
    float c = 3.4;
    float d = 3.1;

    cout << add(a, b) << endl;  // Function your are calling should define above
                                // the main function.
    cout << add(a, b, 1) << endl;
    cout << add(c, d) << endl;

    return 0;
}

// If we want to define a function after the function call, we need to use the
// function prototype.
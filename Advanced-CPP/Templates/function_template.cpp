// Template -> allows writing generic code by passing a data type as a parameter
// Function template -> compiler generates a separate function for each used
// type The type is deduced from the arguments or passed explicitly

#include <iostream>
#include <string>
using namespace std;

template <typename T>
T maximum(T a, T b) {
    return (a > b) ? a : b;
};

template <typename T, typename U>
auto add(T a, U b) {
    return a + b;  // return type is deduced
};

template <typename T>
void printArray(T arr[], int n) {
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
};

int main() {
    cout << maximum(3, 7) << endl;      // int
    cout << maximum(2.5, 1.5) << endl;  // double
    cout << maximum<string>("abc", "xyz") << endl;

    cout << maximum<double>(3, 7) << endl;  // explicit type

    cout << add(2, 3.5) << endl;  // int + double

    int arr[] = {1, 2, 3, 4};
    printArray(arr, 4);

    return 0;
}

// Templates are expanded at compile time, so there is no runtime overhead
// The full definition must be visible where the template is used

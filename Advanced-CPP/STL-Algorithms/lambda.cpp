// Lambda -> an anonymous function defined inside an expression
// Syntax -> [capture](parameters) -> returnType { body }
// Capture list decides which outside variables are available
// [=] captures by value, [&] captures by reference, [x, &y] captures a mix

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    auto add = [](int a, int b) { return a + b; };
    cout << add(2, 3) << endl;

    int x = 10, y = 20;
    auto byValue = [x, y]() { return x + y; };
    cout << byValue() << endl;

    auto byReference = [&x, &y]() {
        x++;
        y++;
    };
    byReference();
    cout << x << " " << y << endl;

    auto mutableLambda = [x]() mutable {
        x++;
        return x;
    };
    cout << mutableLambda() << " " << x << endl;  // original x is unchanged

    vector<int> v = {5, 2, 8, 1};
    sort(v.begin(), v.end(), [](int a, int b) { return a > b; });
    for (int el : v) {
        cout << el << " ";
    }
    cout << endl;

    for_each(v.begin(), v.end(), [](int el) { cout << el * 2 << " "; });
    cout << endl;

    int threshold = 4;
    cout << count_if(v.begin(), v.end(), [threshold](int el) {
        return el > threshold;
    }) << endl;

    return 0;
}

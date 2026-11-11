// Namespace -> a declarative region that gives names a scope
// Used to avoid name collisions between libraries and user code
// :: is the scope resolution operator

#include <iostream>
using namespace std;

namespace Math {
int add(int a, int b) {
    return a + b;
}

int value = 100;

namespace Advanced {
int square(int x) {
    return x * x;
}
}  // namespace Advanced
}  // namespace Math

namespace Physics {
int value = 200;

int add(int a, int b) {
    return a + b;
}
}  // namespace Physics

int main() {
    cout << Math::add(2, 3) << endl;
    cout << Physics::add(2, 3) << endl;
    cout << Math::Advanced::square(5) << endl;

    cout << Math::value << " " << Physics::value << endl;

    using namespace Math;       // using directive
    cout << add(4, 5) << endl;  // Math::add
    cout << value << endl;      // ambiguous if Physics is also imported

    using Math::add;  // using declaration
    cout << add(1, 1) << endl;

    return 0;
}

// An unnamed namespace gives internal linkage to its members
// They can be used only inside the same file
namespace {
int privateValue = 5;
}

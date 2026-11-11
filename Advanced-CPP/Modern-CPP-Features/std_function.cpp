/**
 * std::function and Type Erasure
 *
 * `std::function<R(Args...)>` is a general-purpose polymorphic function
 * wrapper. It can store:
 *   - free functions
 *   - lambdas, including capturing lambdas
 *   - function objects with operator()
 *   - the result of std::bind
 *   - member function pointers (usually wrapped in a lambda or bind)
 *
 * Internally it uses type erasure: the concrete callable type is hidden behind
 * a fixed interface. This makes it easy to store heterogeneous callbacks in one
 * container, at the cost of a small indirection and possible heap allocation.
 *
 * An empty std::function throws std::bad_function_call when invoked. Check it
 * with `if (callback)` before calling.
 *
 * Templates are usually faster when the callable type is known at compile
 * time; std::function is the right tool when callbacks must be stored or
 * selected at runtime.
 */

#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

struct Subtract {
    int operator()(int a, int b) const {
        return a - b;
    }
};

class Button {
   private:
    string label;
    function<void()> onClick;

   public:
    Button(string text, function<void()> callback)
        : label(std::move(text)), onClick(std::move(callback)) {}

    void click() const {
        cout << "clicked " << label << ": ";
        onClick();
    }
};

class Counter {
   private:
    int value = 0;

   public:
    void increment() {
        value++;
    }

    int get() const {
        return value;
    }
};

function<int(int, int)> chooseOperation(char symbol) {
    if (symbol == '+') {
        return add;
    }
    if (symbol == '*') {
        return multiply;
    }
    if (symbol == '-') {
        return Subtract{};
    }
    return nullptr;
}

int main() {
    function<int(int, int)> operation = add;
    cout << "free function: " << operation(3, 4) << endl;

    operation = multiply;
    cout << "reassigned free function: " << operation(3, 4) << endl;

    int offset = 100;
    operation = [offset](int a, int b) { return a + b + offset; };
    cout << "capturing lambda: " << operation(3, 4) << endl;

    operation = Subtract{};
    cout << "function object: " << operation(10, 4) << endl;

    auto addTen = bind(add, 10, placeholders::_1);
    function<int(int)> bound = addTen;
    cout << "std::bind: " << bound(5) << endl;

    Counter counter;
    function<void()> increment = [&counter]() { counter.increment(); };
    increment();
    increment();
    cout << "member access through lambda: " << counter.get() << endl;

    function<int(const Counter &)> getValue = &Counter::get;
    cout << "member function pointer: " << getValue(counter) << endl;

    function<int(int)> factorial = [&factorial](int n) -> int {
        if (n <= 1) {
            return 1;
        }
        return n * factorial(n - 1);
    };
    cout << "recursive std::function: " << factorial(5) << endl;

    vector<function<int(int, int)>> operations = {add, multiply, Subtract{}};
    cout << "operations in a vector: ";

    for (const auto &current : operations) {
        cout << current(8, 2) << " ";
    }
    cout << endl;

    vector<function<void(const string &)>> listeners;
    listeners.push_back(
        [](const string &message) { cout << "logger: " << message << endl; });
    listeners.push_back([](const string &message) {
        cout << "counter: " << message.size() << " characters" << endl;
    });

    for (const auto &listener : listeners) {
        listener("callback");
    }

    Button button("Save", []() { cout << "saving file" << endl; });
    button.click();

    cout << "chooseOperation('+')(6, 7): " << chooseOperation('+')(6, 7)
         << endl;
    cout << "chooseOperation('*')(6, 7): " << chooseOperation('*')(6, 7)
         << endl;

    function<int(int, int)> empty;
    cout << "empty function is false: " << (empty ? "no" : "yes") << endl;

    try {
        empty(1, 2);
    } catch (const bad_function_call &error) {
        cout << "calling empty function threw bad_function_call" << endl;
    }

    function<int(int, int)> copy = add;
    function<int(int, int)> anotherCopy = copy;
    cout << "copied function: " << anotherCopy(2, 3) << endl;

    return 0;
}

// Expected output:
// free function: 7
// reassigned free function: 12
// capturing lambda: 107
// function object: 6
// std::bind: 15
// member access through lambda: 2
// member function pointer: 2
// recursive std::function: 120
// operations in a vector: 10 16 6
// logger: callback
// counter: 8 characters
// clicked Save: saving file
// chooseOperation('+')(6, 7): 13
// chooseOperation('*')(6, 7): 42
// empty function is false: yes
// calling empty function threw bad_function_call
// copied function: 5
//
// Special cases: empty std::function, reassignment, capturing lambdas,
// recursive lambdas, member functions, std::bind, and heterogeneous callables
// stored in one container.

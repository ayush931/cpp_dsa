/**
 * Ranges (C++20) -> composable, lazy algorithms over sequences
 *
 * A range is anything with begin() and end()
 * A view is a cheap, non-owning range that can be composed with |
 *
 * views::filter, views::transform, views::take, views::drop,
 * views::reverse, views::iota, views::keys, views::values
 *
 * Algorithms in std::ranges accept a range directly, no begin/end pair
 * Projections let you sort by a member without a lambda
 *
 * Lazy evaluation -> elements are produced only when iterated
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>
using namespace std;

struct Employee {
    string name;
    int salary;
    int age;
};

int main() {
    vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // filter + transform, no intermediate vector
    auto evenSquares = numbers |
                       views::filter([](int n) { return n % 2 == 0; }) |
                       views::transform([](int n) { return n * n; });

    for (int value : evenSquares) {
        cout << value << " ";  // 4 16 36 64 100
    }
    cout << endl;

    // take the first 3 elements of an infinite sequence
    for (int value : views::iota(1) | views::take(3)) {
        cout << value << " ";  // 1 2 3
    }
    cout << endl;

    // reverse a view
    for (int value : numbers | views::reverse | views::take(3)) {
        cout << value << " ";  // 10 9 8
    }
    cout << endl;

    // ranges algorithms take the whole range
    ranges::sort(numbers, greater<int>());
    cout << numbers.front() << endl;  // 10

    cout << ranges::count_if(numbers, [](int n) { return n > 5; })
         << endl;  // 5

    // projection: sort employees by salary without a custom comparator
    vector<Employee> employees = {
        {"alice", 90000, 30}, {"bob", 70000, 25}, {"carol", 120000, 40}};

    ranges::sort(employees, {}, &Employee::salary);

    for (Employee &employee : employees) {
        cout << employee.name << " " << employee.salary << endl;
    }

    // views are lazy: nothing runs until the loop
    auto expensive = numbers | views::transform([](int n) {
                         cout << "computing " << n << endl;
                         return n * 2;
                     }) |
                     views::take(2);

    cout << "before loop" << endl;

    for (int value : expensive) {
        cout << value << endl;
    }

    return 0;
}

// Special cases: a view must not outlive its underlying range; views can be
// iterated multiple times only if the underlying range is not input-only;
// dangling views are caught by std::ranges::borrowed_range in some cases;
// filter does not resize the source; use ranges::to<vector> (C++23) to
// materialize a view

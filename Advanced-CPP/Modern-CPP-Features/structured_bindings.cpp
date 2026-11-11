// Structured bindings (C++17) -> unpack a pair, tuple, array or structure
// into separate variables in one declaration

#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;
};

int main() {
    pair<int, string> p = {1, "one"};
    auto [id, name] = p;
    cout << id << " " << name << endl;

    tuple<int, double, string> t = {1, 2.5, "three"};
    auto [a, b, c] = t;
    cout << a << " " << b << " " << c << endl;

    int arr[3] = {10, 20, 30};
    auto [x, y, z] = arr;
    cout << x << " " << y << " " << z << endl;

    Point pt = {3, 4};
    auto [px, py] = pt;
    cout << px << " " << py << endl;

    map<string, int> m = {{"a", 1}, {"b", 2}};
    for (const auto &[key, value] : m) {
        cout << key << " " << value << endl;
    }

    vector<pair<int, int>> v = {{1, 2}, {3, 4}};
    for (auto &[first, second] : v) {
        first++;
        second++;
    }
    cout << v[0].first << " " << v[0].second << endl;

    return 0;
}

// Use auto& in a structured binding to modify the original elements

// pair -> stores exactly two values of possibly different types
// tuple -> stores a fixed number of values of possibly different types
// Both are defined in <utility> and <tuple>

#include <algorithm>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;

int main() {
    pair<int, string> p1 = {1, "one"};
    pair<int, string> p2 = make_pair(2, "two");

    cout << p1.first << " " << p1.second << endl;
    cout << p2.first << " " << p2.second << endl;

    vector<pair<int, int>> v = {{3, 4}, {1, 2}, {3, 1}};
    sort(v.begin(), v.end());  // compares first, then second
    for (auto p : v) {
        cout << p.first << " " << p.second << endl;
    }

    tuple<int, string, double> t = make_tuple(1, "Ayush", 9.5);
    cout << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << endl;

    int id;
    string name;
    double marks;
    tie(id, name, marks) = t;
    cout << id << " " << name << " " << marks << endl;

    cout << tuple_size<decltype(t)>::value << endl;

    return 0;
}

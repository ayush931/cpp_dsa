/**
 * std::initializer_list and Custom List Constructors
 *
 * `std::initializer_list<T>` is a lightweight, read-only view over a
 * compiler-generated array of T values. It is the mechanism behind brace
 * initialization such as `vector<int> v{1, 2, 3}`.
 *
 * Key properties:
 *   - It has `begin()`, `end()`, and `size()`.
 *   - A directly initialized initializer_list variable extends the backing
 *     array's lifetime to its own lifetime. A function argument's backing
 *     array lasts through the call's full expression. Copying the view does
 *     not extend that lifetime again; never return a view into a local array.
 *   - A constructor taking `initializer_list<T>` is preferred for brace
 *     initialization over other constructors.
 *   - Narrowing conversions inside braces are compile-time errors.
 *
 * This file shows a custom list class, a nested initializer_list matrix, a
 * free function taking an initializer_list, and the constructor overload
 * subtlety between `IntList(3)` and `IntList{3}`.
 */

#include <initializer_list>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
using namespace std;

class IntList {
   private:
    vector<int> values;

   public:
    IntList() = default;

    // Count constructor: IntList(3) creates three zeros.
    explicit IntList(int count) : values(count, 0) {
        cout << "  used IntList(int count)" << endl;
    }

    // List constructor: IntList{3} creates one element equal to 3.
    IntList(initializer_list<int> list) : values(list) {
        cout << "  used IntList(initializer_list<int>)" << endl;
    }

    IntList &operator=(initializer_list<int> list) {
        values = list;
        return *this;
    }

    size_t size() const {
        return values.size();
    }

    const vector<int> &data() const {
        return values;
    }
};

class Matrix {
   private:
    vector<vector<int>> rows;

   public:
    Matrix(initializer_list<initializer_list<int>> input) {
        for (const initializer_list<int> &row : input) {
            rows.emplace_back(row);
        }
    }

    void print() const {
        for (const vector<int> &row : rows) {
            cout << "  [";

            for (int i = 0; i < static_cast<int>(row.size()); i++) {
                if (i > 0) {
                    cout << ", ";
                }
                cout << row[i];
            }

            cout << "]" << endl;
        }
    }
};

int sumAll(initializer_list<int> values) {
    return accumulate(values.begin(), values.end(), 0);
}

void printWords(initializer_list<string> words) {
    cout << "  ";

    for (const string &word : words) {
        cout << word << " ";
    }

    cout << endl;
}

void printIntList(const IntList &list) {
    cout << "  [";

    const vector<int> &values = list.data();

    for (int i = 0; i < static_cast<int>(values.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << values[i];
    }

    cout << "] size=" << list.size() << endl;
}

int main() {
    cout << "vector brace initialization:" << endl;
    vector<int> braceList{1, 2, 3, 4};
    vector<int> countList(4, 9);

    cout << "  braceList: ";
    for (int value : braceList) {
        cout << value << " ";
    }
    cout << endl;

    cout << "  countList: ";
    for (int value : countList) {
        cout << value << " ";
    }
    cout << endl;

    cout << "IntList(3) vs IntList{3}:" << endl;
    IntList counted(3);
    printIntList(counted);

    IntList listed{3};
    printIntList(listed);

    cout << "assignment from a braced list:" << endl;
    IntList assigned;
    assigned = {5, 6, 7};
    printIntList(assigned);

    cout << "nested initializer_list matrix:" << endl;
    Matrix matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    matrix.print();

    cout << "free function taking initializer_list:" << endl;
    cout << "  sumAll({1, 2, 3, 4, 5}) = " << sumAll({1, 2, 3, 4, 5}) << endl;

    cout << "initializer_list<string>:" << endl;
    printWords({"initializer", "list", "of", "strings"});

    cout << "iterating an initializer_list directly:" << endl;
    cout << "  ";

    for (int value : {10, 20, 30}) {
        cout << value << " ";
    }

    cout << endl;

    initializer_list<int> view = {1, 2, 3};
    cout << "size: " << view.size() << ", front: " << *view.begin()
         << ", back: " << *(view.end() - 1) << endl;

    return 0;
}

// Expected output:
// vector brace initialization:
//   braceList: 1 2 3 4
//   countList: 9 9 9 9
// IntList(3) vs IntList{3}:
//   used IntList(int count)
//   [0, 0, 0] size=3
//   used IntList(initializer_list<int>)
//   [3] size=1
// assignment from a braced list:
//   [5, 6, 7] size=3
// nested initializer_list matrix:
//   [1, 2, 3]
//   [4, 5, 6]
//   [7, 8, 9]
// free function taking initializer_list:
//   sumAll({1, 2, 3, 4, 5}) = 15
// initializer_list<string>:
//   initializer list of strings
// iterating an initializer_list directly:
//   10 20 30
// size: 3, front: 1, back: 3
//
// Special cases: default construction, empty lists, assignment from a braced
// list, nested lists, and the `IntList(3)` versus `IntList{3}` overload
// resolution difference.

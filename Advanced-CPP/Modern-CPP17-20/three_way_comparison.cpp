/**
 * Three-way comparison (C++20) -> operator<=>
 * Returns one of:
 *   std::strong_ordering  -> less, equal, greater (substitutable equality)
 *   std::weak_ordering    -> less, equivalent, greater (case-insensitive names)
 *   std::partial_ordering -> less, equivalent, greater, unordered (NaN)
 *
 * The compiler can rewrite a < b as (a <=> b) < 0 and a == b as
 * (a <=> b) == 0
 *
 * = default generates all six comparisons member-wise, in declaration order
 */

#include <algorithm>
#include <compare>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Version {
    int major;
    int minor;
    int patch;

    auto operator<=>(const Version &other) const = default;
};

struct CaseInsensitiveName {
    string value;

    weak_ordering operator<=>(const CaseInsensitiveName &other) const {
        string left = value;
        string right = other.value;

        for (char &ch : left) {
            ch = tolower(ch);
        }
        for (char &ch : right) {
            ch = tolower(ch);
        }

        return left <=> right;
    }

    bool operator==(const CaseInsensitiveName &other) const {
        return (*this <=> other) == 0;
    }
};

int main() {
    Version first{1, 2, 3};
    Version second{1, 3, 0};

    cout << (first < second) << endl;   // 1
    cout << (first == second) << endl;  // 0
    cout << (first > second) << endl;   // 0
    cout << (first <= second) << endl;  // 1

    vector<Version> versions = {{2, 0, 0}, {1, 10, 0}, {1, 2, 5}, {0, 9, 9}};

    sort(versions.begin(), versions.end());

    for (Version &version : versions) {
        cout << version.major << "." << version.minor << "." << version.patch
             << endl;
    }

    CaseInsensitiveName alpha{"Alpha"};
    CaseInsensitiveName beta{"beta"};

    cout << (alpha < beta) << endl;                           // 1
    cout << (alpha == CaseInsensitiveName{"ALPHA"}) << endl;  // 1

    // comparison categories are comparable to zero
    strong_ordering result = first <=> second;
    cout << (result < 0) << endl;  // 1

    // partial ordering with NaN
    double nan = 0.0 / 0.0;
    partial_ordering order = 1.0 <=> nan;
    cout << (order == partial_ordering::unordered) << endl;  // 1

    return 0;
}

// Special cases: a defaulted <=> compares members in declaration order;
// if any member has no <=>, the defaulted operator is deleted;
// == is not generated unless you default it or write it;
// weak_ordering is right when different values may be equivalent;
// floating point gives partial_ordering because of NaN;
// do not return bool from <=>, return a comparison category

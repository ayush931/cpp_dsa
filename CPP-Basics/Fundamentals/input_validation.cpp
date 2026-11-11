// Input validation
//
// `cin >> number` succeeds only when the input text starts with a valid value
// of the target type. On failure the stream sets failbit and leaves the bad
// characters in the buffer, so simply retrying reads the same bad input again.
//
// The recovery pattern is:
//
//   if (!(cin >> value)) {
//     cin.clear();                                  // clear failbit
//     cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard the line
//   }
//
// For a whole line, read with getline and parse with std::stoi/std::stoll.
// Always check how much of the string was consumed: stoi("12x") returns 12,
// which may hide a typo.
//
// Example:
//   parseInt("42", value)    -> true,  value = 42
//   parseInt("12x", value)   -> false
//   parseInt("", value)      -> false
//
// Special cases:
//   - leading and trailing spaces are accepted by stoi
//   - "+7" and "-7" are valid
//   - "007" is valid and equals 7
//   - overflow throws std::out_of_range
//   - a failed extraction leaves the bad token in the stream until it is
//     cleared and ignored
//   - after clear(), the stream's formatting state is unchanged
//
// Complexity: O(number of characters read) per value.

#include <climits>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool parseInt(const string &token, int &value) {
    try {
        size_t used = 0;
        long long parsed = stoll(token, &used);

        if (used != token.size()) {
            return false;  // trailing characters such as "12x" or "3.5"
        }

        if (parsed < INT_MIN || parsed > INT_MAX) {
            return false;
        }

        value = static_cast<int>(parsed);
        return true;
    } catch (const invalid_argument &) {
        return false;
    } catch (const out_of_range &) {
        return false;
    }
}

bool parseIntLine(const string &line, int &value) {
    istringstream input(line);
    string extra;

    if (!(input >> value)) {
        return false;
    }

    if (input >> extra) {
        return false;  // more than one token on the line
    }

    return true;
}

int main() {
    vector<string> tokens = {"42", "abc", "-5",          "150",
                             "7",  "3.5", "999999999999"};

    int accepted = 0;
    int rejected = 0;

    for (const string &token : tokens) {
        int value = 0;

        if (!parseInt(token, value)) {
            cout << token << " -> rejected (not an integer)" << endl;
            rejected++;
        } else if (value < 1 || value > 100) {
            cout << token << " -> rejected (outside 1..100)" << endl;
            rejected++;
        } else {
            cout << token << " -> accepted (" << value << ")" << endl;
            accepted++;
        }
    }

    cout << "accepted: " << accepted << ", rejected: " << rejected << endl;

    // Recover from a bad token in a stream.
    istringstream input("10 20 x 30");
    int value;

    while (input >> value) {
        cout << "stream value: " << value << endl;
    }

    if (input.fail()) {
        input.clear();
        string badToken;
        input >> badToken;
        cout << "bad token: " << badToken << endl;
    }

    input >> value;
    cout << "next value: " << value << endl;

    // Line-based parsing rejects trailing text.
    int lineValue = 0;

    if (parseIntLine("  123  ", lineValue)) {
        cout << "line parse: " << lineValue << endl;
    }

    if (!parseIntLine("12x", lineValue)) {
        cout << "line parse: rejected" << endl;
    }

    return 0;
}

// Output:
// 42 -> accepted (42)
// abc -> rejected (not an integer)
// -5 -> rejected (outside 1..100)
// 150 -> rejected (outside 1..100)
// 7 -> accepted (7)
// 3.5 -> rejected (not an integer)
// 999999999999 -> rejected (not an integer)
// accepted: 2, rejected: 5
// stream value: 10
// stream value: 20
// bad token: x
// next value: 30
// line parse: 123
// line parse: rejected
//
// Validate at the boundary where input enters the program. Once a value is
// inside, keep the invariant (for example 1 <= value <= 100) instead of
// re-checking it everywhere.

/**
 * Restore IP Addresses
 *
 * Given a string of digits, return every valid IP address that can be formed
 * by inserting three dots. Each of the four parts must:
 *   - contain between 1 and 3 digits,
 *   - be between 0 and 255,
 *   - have no leading zero unless the part is exactly "0".
 *
 * Example:
 * Input:  "25525511135"
 * Output: 255.255.11.135, 255.255.111.35
 *
 * Input:  "0000"
 * Output: 0.0.0.0
 *
 * Input:  "101023"
 * Output: 1.0.10.23, 1.0.102.3, 10.1.0.23, 10.10.2.3, 101.0.2.3
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool validPart(const string &part) {
    if (part.empty() || part.size() > 3) {
        return false;
    }
    if (part.size() > 1 && part[0] == '0') {
        return false;
    }

    int value = 0;
    for (char digit : part) {
        value = value * 10 + (digit - '0');
    }

    return value <= 255;
}

// Brute force: try every placement of the three dots.
// There are only O(n^3) placements, and each check is O(1) for these lengths.
vector<string> restoreIpAddressesBrute(const string &s) {
    vector<string> result;

    if (s.size() < 4 || s.size() > 12) {
        return result;
    }

    for (int first = 1; first <= 3; first++) {
        for (int second = first + 1; second <= first + 3; second++) {
            for (int third = second + 1; third <= second + 3; third++) {
                int fourthLength = s.size() - third;
                if (fourthLength < 1 || fourthLength > 3) {
                    continue;
                }

                string a = s.substr(0, first);
                string b = s.substr(first, second - first);
                string c = s.substr(second, third - second);
                string d = s.substr(third);

                if (validPart(a) && validPart(b) && validPart(c) &&
                    validPart(d)) {
                    result.push_back(a + "." + b + "." + c + "." + d);
                }
            }
        }
    }

    return result;
}

// Optimized: build one part at a time and prune impossible branches early.
void build(const string &s, int index, int part, vector<string> &current,
           vector<string> &result) {
    if (part == 4) {
        if (index == s.size()) {
            result.push_back(current[0] + "." + current[1] + "." + current[2] +
                             "." + current[3]);
        }
        return;
    }

    int remainingChars = s.size() - index;
    int remainingParts = 4 - part;

    // Every remaining part needs 1..3 digits.
    if (remainingChars < remainingParts ||
        remainingChars > 3 * remainingParts) {
        return;
    }

    int value = 0;

    for (int length = 1; length <= 3 && index + length <= s.size(); length++) {
        if (length > 1 && s[index] == '0') {
            break;  // no leading zero
        }

        value = value * 10 + (s[index + length - 1] - '0');
        if (value > 255) {
            break;
        }

        current.push_back(s.substr(index, length));
        build(s, index + length, part + 1, current, result);
        current.pop_back();
    }
}

vector<string> restoreIpAddresses(const string &s) {
    vector<string> result;
    vector<string> current;

    if (s.size() < 4 || s.size() > 12) {
        return result;
    }

    build(s, 0, 0, current, result);

    return result;
}

void print(const vector<string> &addresses) {
    if (addresses.empty()) {
        cout << "(none)\n";
        return;
    }

    for (const string &address : addresses) {
        cout << address << "\n";
    }
}

int main() {
    print(restoreIpAddressesBrute("25525511135"));
    // 255.255.11.135
    // 255.255.111.35

    cout << "---\n";
    print(restoreIpAddresses("25525511135"));
    // 255.255.11.135
    // 255.255.111.35

    cout << "---\n";
    print(restoreIpAddresses("0000"));
    // 0.0.0.0

    cout << "---\n";
    print(restoreIpAddresses("101023"));
    // 1.0.10.23
    // 1.0.102.3
    // 10.1.0.23
    // 10.10.2.3
    // 101.0.2.3

    cout << "---\n";
    print(restoreIpAddresses("1111"));
    // 1.1.1.1

    cout << "---\n";
    print(restoreIpAddresses("9999999999999"));
    // (none) because it has more than 12 digits

    return 0;
}

/**
 * Time complexity -> O(3^4) valid cut placements in the worst case, which is
 * constant; the brute-force loop is O(n^3) for general n.
 * Space complexity -> O(1) recursion depth (at most four parts).
 *
 * Special cases: fewer than 4 or more than 12 digits, leading zeros, parts
 * greater than 255, "0.0.0.0", and strings that produce no valid address.
 */

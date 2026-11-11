/**
 * Generate binary numbers from 1 to n using a queue
 *
 * Produce the binary representation of every integer from 1 through n in
 * increasing order without using arithmetic conversion for each number.
 *
 * Example:
 * Input:  n = 5
 * Output: 1 10 11 100 101
 *
 * Queue idea: start with "1". Each time a string is removed, append "0" and
 * "1" to the queue to create the next two binary numbers in order.
 *
 * Time -> O(n) queue operations, total output O(n log n) digits
 * Space -> O(n) for the queue and result
 */

#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

vector<string> generateBinary(int n) {
    vector<string> result;
    if (n <= 0) {
        return result;
    }

    queue<string> pending;
    pending.push("1");

    for (int i = 0; i < n; i++) {
        string current = pending.front();
        pending.pop();

        result.push_back(current);

        pending.push(current + "0");
        pending.push(current + "1");
    }

    return result;
}

// Reference conversion by repeated division, used to verify the queue output.
string toBinaryBrute(int value) {
    if (value == 0) {
        return "0";
    }

    string bits;

    while (value > 0) {
        bits.push_back(char('0' + value % 2));
        value /= 2;
    }

    for (int i = 0, j = bits.size() - 1; i < j; i++, j--) {
        swap(bits[i], bits[j]);
    }

    return bits;
}

void print(const vector<string> &values) {
    for (int i = 0; i < (int)values.size(); i++) {
        cout << values[i];
        if (i + 1 < (int)values.size()) {
            cout << " ";
        }
    }
    cout << endl;
}

int main() {
    print(generateBinary(5));   // 1 10 11 100 101
    print(generateBinary(1));   // 1
    print(generateBinary(0));   // (empty line)
    print(generateBinary(-3));  // (empty line)

    // Verify the first 8 values against direct conversion.
    vector<string> generated = generateBinary(8);
    bool matches = true;

    for (int i = 0; i < (int)generated.size(); i++) {
        if (generated[i] != toBinaryBrute(i + 1)) {
            matches = false;
        }
    }

    cout << matches << endl;  // 1

    return 0;
}

/**
 * Special cases: n <= 0 (no output), n = 1, and a value with a new highest
 * power of two such as 8 -> 1000.
 */

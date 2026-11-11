/**
 * String hashing -> maps a string to an integer using a polynomial hash
 * hash(s) = (s[0] * p^(n-1) + s[1] * p^(n-2) + ... + s[n-1]) % mod
 * Prefix hashes allow substring hash comparison in O(1)
 * Collisions are possible; use double hashing or verify matches
 *
 * Build -> O(n), substring hash -> O(1)
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

const long long MOD = 1e9 + 7;
const long long BASE = 31;

vector<long long> buildPrefixHash(string &s) {
    int n = s.size();
    vector<long long> prefix(n + 1, 0);

    for (int i = 0; i < n; i++) {
        prefix[i + 1] = (prefix[i] * BASE + (s[i] - 'a' + 1)) % MOD;
    }

    return prefix;
};

vector<long long> buildPowers(int n) {
    vector<long long> power(n + 1, 1);

    for (int i = 1; i <= n; i++) {
        power[i] = (power[i - 1] * BASE) % MOD;
    }

    return power;
};

long long substringHash(vector<long long> &prefix, vector<long long> &power,
                        int left, int right) {
    long long result =
        (prefix[right + 1] - prefix[left] * power[right - left + 1]) % MOD;
    return (result + MOD) % MOD;
};

bool rabinKarpCompare(string &text, string &pattern) {
    int n = text.size();
    int m = pattern.size();

    if (m > n) {
        return false;
    }

    vector<long long> textPrefix = buildPrefixHash(text);
    vector<long long> patternPrefix = buildPrefixHash(pattern);
    vector<long long> power = buildPowers(n);

    long long patternHash = patternPrefix[m];

    for (int i = 0; i + m <= n; i++) {
        if (substringHash(textPrefix, power, i, i + m - 1) == patternHash) {
            if (text.substr(i, m) == pattern) {
                return true;  // verify to avoid collisions
            }
        }
    }

    return false;
};

int main() {
    string text = "ababcababcabc";
    string pattern = "abc";

    vector<long long> prefix = buildPrefixHash(text);
    vector<long long> power = buildPowers(text.size());

    cout << substringHash(prefix, power, 0, 2) << endl;
    cout << substringHash(prefix, power, 5, 7) << endl;
    cout << (substringHash(prefix, power, 0, 2) ==
             substringHash(prefix, power, 5, 7))
         << endl;

    cout << rabinKarpCompare(text, pattern) << endl;  // 1

    return 0;
}

// Special cases: empty string, pattern longer than text, hash collision,
// uppercase letters, negative hash values (add MOD)

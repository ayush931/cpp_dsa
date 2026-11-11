/**
 * DP on an Aho-Corasick automaton
 *
 * Build an Aho-Corasick automaton for a set of forbidden patterns. Every state
 * represents a prefix of some pattern. A state is terminal if its string ends
 * with a forbidden pattern, including through a failure link.
 *
 * DP:
 *   dp[length][state] = number of strings of the given length that end in this
 *                       state without containing a forbidden pattern
 *
 * Transition: append a character, follow the automaton transition, and skip
 * terminal states.
 *
 * Time -> O(length * states * alphabetSize)
 * Space -> O(states)
 *
 * Edge cases: empty pattern set, a pattern that is a prefix of another, a
 * pattern equal to the empty string, duplicate patterns, and length 0.
 */

#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
using namespace std;

const int MOD = 1000000007;

class AhoCorasick {
    struct Node {
        map<char, int> next;
        int failure = 0;
        bool terminal = false;
    };

    vector<Node> nodes;

   public:
    AhoCorasick() {
        nodes.push_back(Node());
    }

    void insert(string &pattern) {
        int node = 0;

        for (char ch : pattern) {
            if (!nodes[node].next.count(ch)) {
                nodes[node].next[ch] = nodes.size();
                nodes.push_back(Node());
            }
            node = nodes[node].next[ch];
        }

        nodes[node].terminal = true;
    }

    void build() {
        queue<int> q;

        for (auto &[ch, child] : nodes[0].next) {
            nodes[child].failure = 0;
            q.push(child);
        }

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (auto &[ch, child] : nodes[node].next) {
                int failure = nodes[node].failure;

                while (failure != 0 && !nodes[failure].next.count(ch)) {
                    failure = nodes[failure].failure;
                }

                if (nodes[failure].next.count(ch) &&
                    nodes[failure].next[ch] != child) {
                    failure = nodes[failure].next[ch];
                }

                nodes[child].failure = failure;
                nodes[child].terminal =
                    nodes[child].terminal || nodes[failure].terminal;
                q.push(child);
            }
        }
    }

    int transition(int node, char ch) {
        while (node != 0 && !nodes[node].next.count(ch)) {
            node = nodes[node].failure;
        }

        if (nodes[node].next.count(ch)) {
            return nodes[node].next[ch];
        }
        return 0;
    }

    int size() const {
        return nodes.size();
    }

    bool isTerminal(int node) const {
        return nodes[node].terminal;
    }
};

int countStrings(int length, vector<string> &patterns, string alphabet) {
    AhoCorasick automaton;

    for (string &pattern : patterns) {
        automaton.insert(pattern);
    }
    automaton.build();

    int states = automaton.size();
    vector<long long> dp(states, 0);
    dp[0] = 1;

    for (int position = 0; position < length; position++) {
        vector<long long> next(states, 0);

        for (int state = 0; state < states; state++) {
            if (dp[state] == 0) {
                continue;
            }

            for (char ch : alphabet) {
                int nextState = automaton.transition(state, ch);
                if (automaton.isTerminal(nextState)) {
                    continue;
                }

                next[nextState] = (next[nextState] + dp[state]) % MOD;
            }
        }

        dp = next;
    }

    long long total = 0;
    for (long long value : dp) {
        total = (total + value) % MOD;
    }
    return total;
}

int bruteForce(int length, vector<string> &patterns, string alphabet) {
    int total = 0;
    int combinations = 1;

    for (int i = 0; i < length; i++) {
        combinations *= alphabet.size();
    }

    for (int mask = 0; mask < combinations; mask++) {
        int value = mask;
        string candidate;

        for (int i = 0; i < length; i++) {
            candidate.push_back(alphabet[value % alphabet.size()]);
            value /= alphabet.size();
        }

        bool valid = true;
        for (string &pattern : patterns) {
            if (candidate.find(pattern) != string::npos) {
                valid = false;
                break;
            }
        }

        if (valid) {
            total++;
        }
    }

    return total;
}

int main() {
    vector<string> patterns = {"ab", "bc"};
    string alphabet = "abc";

    cout << countStrings(3, patterns, alphabet) << endl;  // 16
    cout << bruteForce(3, patterns, alphabet) << endl;    // 16

    vector<string> none;
    cout << countStrings(2, none, "ab") << endl;  // 4

    vector<string> duplicate = {"aa", "aa"};
    cout << countStrings(2, duplicate, "ab") << endl;  // 3

    return 0;
}

// Special cases:
// - no patterns -> alphabetSize^length
// - duplicate patterns do not change the automaton
// - a terminal state reached through a failure link is also forbidden
// - length 0 -> 1 (the empty string)

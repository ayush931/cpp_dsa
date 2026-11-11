/**
 * Suffix Automaton (SAM)
 *
 * A Suffix Automaton is a directed acyclic word graph (DAWG) that represents
 * all substrings of a given string in compact form.
 *
 * Key Theoretical Properties:
 *   - For a string of length N, the automaton has at most 2*N - 1 states
 *     and at most 3*N - 4 transitions.
 *   - Each state corresponds to an equivalence class of substrings having
 *     the exact same end-positions (endpos equivalence classes).
 *   - Can be built online in strictly linear O(N) time and space.
 *   - Suffix links connect each state to the longest proper suffix of its
 *     class that belongs to a different equivalence class, forming a tree
 *     (the link tree, which is isomorphic to the suffix tree of the reversed
 * string).
 *
 * Applications:
 *   1. Substring query: check if pattern P is a substring in O(|P|) time.
 *   2. Number of distinct substrings in O(N).
 *   3. Total length of all distinct substrings in O(N).
 *   4. Longest Common Substring (LCS) of two strings in O(|S1| + |S2|).
 *   5. Lexicographically k-th substring query.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

class SuffixAutomaton {
   public:
    struct State {
        int len;              // maximum length of a substring in this state
        int link;             // suffix link to parent equivalence class
        map<char, int> next;  // transitions
        bool is_clone;        // true if this state was cloned during split
        long long cnt_paths;  // memoization for number of paths

        State() : len(0), link(-1), is_clone(false), cnt_paths(-1) {}
    };

   private:
    vector<State> st;
    int sz;
    int last;

   public:
    SuffixAutomaton(int max_len = 0) {
        init(max_len);
    }

    void init(int max_len = 0) {
        st.clear();
        st.reserve(max(2 * max_len, 2));
        st.push_back(State());  // state 0 is initial empty state
        sz = 1;
        last = 0;
    }

    // Extend the automaton by character c (online linear-time insertion)
    void extend(char c) {
        int cur = sz++;
        st.push_back(State());
        st[cur].len = st[last].len + 1;

        int p = last;
        // Walk back along suffix links and add transition 'c' pointing to 'cur'
        while (p != -1 && !st[p].next.count(c)) {
            st[p].next[c] = cur;
            p = st[p].link;
        }

        if (p == -1) {
            // Reached before root: suffix link goes to initial state
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                // Continuous transition: link directly to q
                st[cur].link = q;
            } else {
                // Split state q into clone state
                int clone = sz++;
                st.push_back(State());
                st[clone].len = st[p].len + 1;
                st[clone].next = st[q].next;
                st[clone].link = st[q].link;
                st[clone].is_clone = true;

                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }

                st[q].link = clone;
                st[cur].link = clone;
            }
        }

        last = cur;
    }

    void build(const string &s) {
        init(s.length());
        for (char c : s) {
            extend(c);
        }
    }

    // 1. Check if pattern is a substring in O(|pattern|)
    bool contains(const string &pattern) const {
        int curr = 0;
        for (char c : pattern) {
            auto it = st[curr].next.find(c);
            if (it == st[curr].next.end()) {
                return false;
            }
            curr = it->second;
        }
        return true;
    }

    // 2. Count distinct substrings in O(states) = O(N)
    long long countDistinctSubstrings() {
        long long total = 0;
        // In SAM, each state 'u' (except 0) contributes (len[u] - len[link[u]])
        // distinct substrings
        for (size_t i = 1; i < st.size(); ++i) {
            total += st[i].len - st[st[i].link].len;
        }
        return total;
    }

    // 3. Longest Common Substring of string S and T
    static pair<int, string> longestCommonSubstring(const string &s,
                                                    const string &t) {
        SuffixAutomaton sam;
        sam.build(s);

        int curr = 0;
        int curr_len = 0;
        int max_len = 0;
        int best_end_in_t = -1;

        for (size_t i = 0; i < t.length(); ++i) {
            char c = t[i];
            while (curr != 0 && !sam.st[curr].next.count(c)) {
                curr = sam.st[curr].link;
                curr_len = sam.st[curr].len;
            }
            if (sam.st[curr].next.count(c)) {
                curr = sam.st[curr].next[c];
                curr_len++;
            }
            if (curr_len > max_len) {
                max_len = curr_len;
                best_end_in_t = i;
            }
        }

        string lcs = "";
        if (best_end_in_t != -1) {
            lcs = t.substr(best_end_in_t - max_len + 1, max_len);
        }
        return {max_len, lcs};
    }

    int numStates() const {
        return sz;
    }
};

int main() {
    cout << "=== Suffix Automaton (SAM) Demo ===" << endl;

    string s = "abacaba";
    SuffixAutomaton sam;
    sam.build(s);

    cout << "String: \"" << s << "\"" << endl;
    cout << "Number of states: " << sam.numStates()
         << " (at most 2*N - 1 = " << 2 * s.length() - 1 << ")" << endl;

    // 1. Substring queries
    vector<string> queries = {"aba", "caba", "bac", "xyz", "abacaba", "caa"};
    cout << "\n--- Substring Queries ---" << endl;
    for (const auto &q : queries) {
        cout << "  Contains \"" << q << "\"? "
             << (sam.contains(q) ? "YES" : "NO") << endl;
    }

    // 2. Count distinct substrings
    long long distinct = sam.countDistinctSubstrings();
    cout << "\n--- Distinct Substrings ---" << endl;
    cout << "Total distinct substrings in \"" << s << "\": " << distinct
         << endl;

    // 3. Longest Common Substring (LCS)
    string s1 = "algorithmic_trading";
    string s2 = "logarithm_and_graphs";
    auto [lcs_len, lcs_str] = SuffixAutomaton::longestCommonSubstring(s1, s2);
    cout << "\n--- Longest Common Substring ---" << endl;
    cout << "String 1: \"" << s1 << "\"" << endl;
    cout << "String 2: \"" << s2 << "\"" << endl;
    cout << "LCS length: " << lcs_len << ", LCS string: \"" << lcs_str << "\""
         << endl;

    return 0;
}

/**
 * Complexity:
 *   - Build: O(N * alphabet_size) or O(N log(alphabet_size)) using map
 *   - States: at most 2 * N - 1
 *   - Transitions: at most 3 * N - 4
 *   - Substring Query: O(|Pattern|)
 *   - Distinct Substrings: O(N)
 *   - Longest Common Substring: O(|S1| + |S2|)
 *   - Space Complexity: O(N)
 */

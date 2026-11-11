/**
 * Huffman Coding
 *
 * Huffman coding builds an optimal prefix-free binary code for a set of
 * symbols with known frequencies.
 *
 * Greedy idea:
 *   1. Put every symbol in a min-heap keyed by frequency.
 *   2. Repeatedly remove the two least frequent nodes.
 *   3. Create a parent whose frequency is their sum and put it back.
 *   4. The final node is the root of the Huffman tree.
 *
 * Left edges are labelled 0 and right edges are labelled 1. Because no code is
 * a prefix of another code, the bit stream is uniquely decodable.
 *
 * Time        -> O(n log n) for n distinct symbols
 * Space       -> O(n)
 * Optimality  -> the weighted external path length is minimal among all
 *                prefix-free binary codes for the given frequencies
 *
 * The implementation uses node indices instead of raw pointers, so the tree is
 * stored contiguously and needs no manual delete calls.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct Node {
    long long frequency;
    char symbol;
    int left;
    int right;

    Node(long long f, char c, int l, int r)
        : frequency(f), symbol(c), left(l), right(r) {}
};

class HuffmanCoder {
   private:
    vector<Node> nodes;
    int root;

    void buildCodes(int index, const string &prefix,
                    map<char, string> &codes) const {
        if (index == -1) {
            return;
        }

        const Node &node = nodes[index];

        if (node.left == -1 && node.right == -1) {
            // A one-symbol alphabet still needs a non-empty code.
            codes[node.symbol] = prefix.empty() ? "0" : prefix;
            return;
        }

        buildCodes(node.left, prefix + "0", codes);
        buildCodes(node.right, prefix + "1", codes);
    }

   public:
    HuffmanCoder() : root(-1) {}

    void build(const string &text) {
        nodes.clear();
        root = -1;

        map<char, long long> frequency;
        for (char c : text) {
            frequency[c]++;
        }

        if (frequency.empty()) {
            return;
        }

        // The second value is the node index, which breaks frequency ties
        // deterministically.
        priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                       greater<pair<long long, int>>>
            minHeap;

        for (const auto &entry : frequency) {
            int index = static_cast<int>(nodes.size());
            nodes.push_back(Node(entry.second, entry.first, -1, -1));
            minHeap.push({entry.second, index});
        }

        while (minHeap.size() > 1) {
            pair<long long, int> first = minHeap.top();
            minHeap.pop();
            pair<long long, int> second = minHeap.top();
            minHeap.pop();

            int index = static_cast<int>(nodes.size());
            nodes.push_back(Node(first.first + second.first, '\0', first.second,
                                 second.second));
            minHeap.push({nodes[index].frequency, index});
        }

        root = minHeap.top().second;
    }

    map<char, string> codes() const {
        map<char, string> result;
        buildCodes(root, "", result);
        return result;
    }

    string encode(const string &text) const {
        map<char, string> table = codes();
        string bits;

        for (char c : text) {
            auto it = table.find(c);
            if (it == table.end()) {
                return "";
            }
            bits += it->second;
        }

        return bits;
    }

    string decode(const string &bits) const {
        if (root == -1) {
            return "";
        }
        if (bits.empty()) {
            return "";
        }

        const Node &rootNode = nodes[root];

        // Single-symbol alphabet uses code 0; any other bit is invalid.
        if (rootNode.left == -1 && rootNode.right == -1) {
            if (bits.find_first_not_of('0') != string::npos) {
                return "";
            }
            return string(bits.size(), rootNode.symbol);
        }

        string text;
        int current = root;

        for (char bit : bits) {
            if (bit == '0') {
                current = nodes[current].left;
            } else if (bit == '1') {
                current = nodes[current].right;
            } else {
                return "";
            }

            if (current == -1) {
                return "";
            }

            if (nodes[current].left == -1 && nodes[current].right == -1) {
                text += nodes[current].symbol;
                current = root;
            }
        }

        // An incomplete code is invalid.
        if (current != root) {
            return "";
        }
        return text;
    }

    long long weightedPathLength() const {
        long long total = 0;

        // Our one-symbol convention uses a one-bit code, not an empty code.
        if (root != -1 && nodes[root].left == -1 && nodes[root].right == -1) {
            return nodes[root].frequency;
        }

        // Every internal node contributes its frequency once for every level it
        // lifts its leaves. Summing all internal frequencies is equivalent to
        // the weighted external path length.
        for (const Node &node : nodes) {
            if (node.left != -1 || node.right != -1) {
                total += node.frequency;
            }
        }

        return total;
    }
};

void printCodes(const map<char, string> &table) {
    for (const auto &entry : table) {
        if (entry.first == ' ') {
            cout << "' '" << " -> " << entry.second << endl;
        } else {
            cout << entry.first << " -> " << entry.second << endl;
        }
    }
}

int main() {
    vector<string> tests = {"abracadabra", "mississippi", "aaaaa", "",
                            "a",           "aaabbc"};

    for (const string &text : tests) {
        HuffmanCoder coder;
        coder.build(text);

        map<char, string> table = coder.codes();
        string bits = coder.encode(text);
        string restored = coder.decode(bits);

        cout << "text: \"" << text << "\"" << endl;
        printCodes(table);
        cout << "encoded: " << bits << endl;
        cout << "decoded: \"" << restored << "\"" << endl;
        cout << "round trip: " << (restored == text ? "yes" : "no") << endl;
        cout << "weighted path length: " << coder.weightedPathLength() << endl;
        cout << "bit count: " << bits.size() << endl;
        cout << "---" << endl;
    }

    return 0;
}

// Expected output (abridged):
// text: "abracadabra"
// a -> 0
// b -> 110
// c -> 100
// d -> 101
// r -> 111
// encoded: 01101110100010101101110
// decoded: "abracadabra"
// round trip: yes
// weighted path length: 23
// bit count: 23
// ---
//
// Special cases: empty text (empty code table and empty bit stream), a single
// distinct symbol (code "0" repeated), all-equal frequencies, and strings with
// characters that occur only once.

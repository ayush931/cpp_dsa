/**
 * Amortized analysis examples
 *
 * 1. Binary counter
 *    Incrementing a k-bit counter flips a variable number of bits.
 *    Bit i flips every 2^i increments, so the total flips for n increments is
 *    less than 2n. Aggregate amortized cost -> O(1) per increment.
 *
 * 2. Queue using two stacks
 *    Each element is pushed once and popped once from each stack.
 *    A single dequeue can cost O(n), but a sequence of m operations costs O(m).
 *    Accounting amortized cost -> O(1) per operation.
 *
 * 3. DSU with path compression and union by rank
 *    A sequence of m operations costs O(m * alpha(n)) amortized, where alpha is
 *    the inverse Ackermann function and is at most 4 for practical n.
 *
 * 4. Monotonic stack (next greater element)
 *    Every index is pushed once and popped at most once -> O(n) total.
 *
 * Edge cases: empty structures, repeated operations, worst-case single
 * operation, and sequences that trigger many flips or moves at once.
 */

#include <iostream>
#include <stack>
#include <vector>
using namespace std;

class BinaryCounter {
    vector<int> bits;
    long long flips;

   public:
    BinaryCounter(int k) : bits(k, 0), flips(0) {}

    void increment() {
        int i = 0;
        while (i < (int)bits.size() && bits[i] == 1) {
            bits[i] = 0;
            flips++;
            i++;
        }
        if (i < (int)bits.size()) {
            bits[i] = 1;
            flips++;
        }
    }

    long long totalFlips() const {
        return flips;
    }
};

class QueueWithTwoStacks {
    stack<int> input;
    stack<int> output;

    void move() {
        while (!input.empty()) {
            output.push(input.top());
            input.pop();
        }
    }

   public:
    void push(int value) {
        input.push(value);
    }

    int pop() {
        if (output.empty()) {
            move();
        }
        if (output.empty()) {
            return -1;
        }

        int value = output.top();
        output.pop();
        return value;
    }

    int front() {
        if (output.empty()) {
            move();
        }
        if (output.empty()) {
            return -1;
        }
        return output.top();
    }

    bool empty() const {
        return input.empty() && output.empty();
    }
};

class DisjointSet {
    vector<int> parent;
    vector<int> rank;

   public:
    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int node) {
        if (parent[node] != node) {
            parent[node] = find(parent[node]);
        }
        return parent[node];
    }

    bool unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);
        if (rootA == rootB) {
            return false;
        }

        if (rank[rootA] < rank[rootB]) {
            swap(rootA, rootB);
        }
        parent[rootB] = rootA;
        if (rank[rootA] == rank[rootB]) {
            rank[rootA]++;
        }
        return true;
    }
};

vector<int> nextGreater(vector<int> &arr) {
    int n = arr.size();
    vector<int> answer(n, -1);
    stack<int> st;

    for (int i = 0; i < n; i++) {
        while (!st.empty() && arr[st.top()] < arr[i]) {
            answer[st.top()] = arr[i];
            st.pop();
        }
        st.push(i);
    }

    return answer;
}

int main() {
    BinaryCounter counter(10);
    for (int i = 0; i < 1024; i++) {
        counter.increment();
    }
    cout << counter.totalFlips()
         << endl;  // 2046 (10-bit counter overflows on the last increment)

    QueueWithTwoStacks queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    cout << queue.pop() << endl;  // 1
    queue.push(4);
    cout << queue.front() << endl;  // 2
    cout << queue.pop() << endl;    // 2
    cout << queue.pop() << endl;    // 3
    cout << queue.pop() << endl;    // 4
    cout << queue.pop() << endl;    // -1

    DisjointSet dsu(5);
    cout << dsu.unite(0, 1) << endl;  // 1
    cout << dsu.unite(1, 2) << endl;  // 1
    cout << dsu.unite(0, 2) << endl;  // 0
    cout << dsu.find(2) << endl;      // 0

    vector<int> arr = {2, 1, 3, 5, 4};
    vector<int> greater = nextGreater(arr);
    for (int value : greater) {
        cout << value << " ";
    }
    cout << endl;  // 3 3 5 -1 -1

    return 0;
}

// Special cases:
// - a counter with all bits set flips all k bits on the next increment
// - a queue pop on an empty queue returns -1
// - DSU union of already connected nodes returns false
// - the monotonic stack leaves -1 for elements with no greater element

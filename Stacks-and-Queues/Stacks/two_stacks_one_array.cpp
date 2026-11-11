/**
 * Two stacks in one array
 *
 * Store both stacks in a single array and let them grow toward each other:
 * stack 1 starts at index 0 and grows right, stack 2 starts at the last index
 * and grows left. They share the free space in the middle.
 *
 * push/pop/peek -> O(1)
 * Space -> O(capacity)
 *
 * Example with capacity 5:
 * push1(1), push1(2), push2(3), push2(4)
 * peek1() -> 2, peek2() -> 4
 * A fifth push fills the array; the next push on either stack fails.
 */

#include <iostream>
#include <vector>
using namespace std;

class TwoStacks {
    vector<int> data;
    int top1;  // index of stack 1's top, -1 when empty
    int top2;  // index of stack 2's top, data.size() when empty

   public:
    TwoStacks(int capacity) : data(capacity), top1(-1), top2(capacity) {}

    bool push1(int value) {
        if (top1 + 1 == top2) {
            return false;  // no free slot left
        }

        data[++top1] = value;
        return true;
    }

    bool push2(int value) {
        if (top1 + 1 == top2) {
            return false;
        }

        data[--top2] = value;
        return true;
    }

    int pop1() {
        if (top1 == -1) {
            return -1;
        }

        return data[top1--];
    }

    int pop2() {
        if (top2 == (int)data.size()) {
            return -1;
        }

        return data[top2++];
    }

    int peek1() const {
        if (top1 == -1) {
            return -1;
        }
        return data[top1];
    }

    int peek2() const {
        if (top2 == (int)data.size()) {
            return -1;
        }
        return data[top2];
    }

    bool empty1() const {
        return top1 == -1;
    }

    bool empty2() const {
        return top2 == (int)data.size();
    }

    bool full() const {
        return top1 + 1 == top2;
    }
};

int main() {
    TwoStacks stacks(5);

    cout << stacks.push1(1) << " " << stacks.push1(2) << endl;  // 1 1
    cout << stacks.push2(3) << " " << stacks.push2(4) << endl;  // 1 1

    cout << stacks.peek1() << " " << stacks.peek2() << endl;  // 2 4
    cout << stacks.full() << endl;                            // 0

    cout << stacks.push1(5) << endl;  // 1
    cout << stacks.full() << endl;    // 1
    cout << stacks.push2(6) << endl;  // 0

    cout << stacks.pop1() << endl;    // 5
    cout << stacks.push2(6) << endl;  // 1 (slot reused)

    cout << stacks.pop2() << " " << stacks.pop2() << endl;  // 6 4
    cout << stacks.pop1() << " " << stacks.pop1() << endl;  // 2 1

    cout << stacks.empty1() << " " << stacks.empty2() << endl;  // 1 1
    cout << stacks.pop1() << endl;                              // -1

    return 0;
}

/**
 * Special cases: capacity 0 (every push fails), one stack empty while the
 * other is full, and alternating pushes that reuse freed slots.
 * The -1 return value is a sentinel; the empty1/empty2 checks distinguish it
 * from a stored -1.
 */

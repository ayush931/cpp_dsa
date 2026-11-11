/**
 * Interleave the first half of a queue with the second half
 *
 * Given a queue of integers, split it into a first half and a second half,
 * then interleave them: first element of the first half, first element of the
 * second half, second element of the first half, and so on.
 *
 * For an odd number of elements, the first half gets the extra element.
 *
 * Example:
 * Input:  [1, 2, 3, 4, 5, 6]
 * Output: [1, 4, 2, 5, 3, 6]
 *
 * Input:  [1, 2, 3, 4, 5]
 * Output: [1, 4, 2, 5, 3]
 *
 * Time -> O(n), Space -> O(n) for the two helper queues
 */

#include <deque>
#include <iostream>
#include <queue>
using namespace std;

queue<int> interleave(queue<int> input) {
    int n = input.size();
    if (n <= 1) {
        return input;
    }

    int firstHalfSize = (n + 1) / 2;
    queue<int> first;
    queue<int> second;

    for (int i = 0; i < firstHalfSize; i++) {
        first.push(input.front());
        input.pop();
    }

    while (!input.empty()) {
        second.push(input.front());
        input.pop();
    }

    queue<int> result;

    while (!first.empty() || !second.empty()) {
        if (!first.empty()) {
            result.push(first.front());
            first.pop();
        }

        if (!second.empty()) {
            result.push(second.front());
            second.pop();
        }
    }

    return result;
}

void print(queue<int> values) {
    cout << "[";
    bool firstValue = true;

    while (!values.empty()) {
        if (!firstValue) {
            cout << ", ";
        }
        cout << values.front();
        values.pop();
        firstValue = false;
    }

    cout << "]" << endl;
}

int main() {
    // queue is an adaptor: initialize its underlying deque, not a list of ints.
    print(interleave(
        queue<int>{deque<int>{1, 2, 3, 4, 5, 6}}));  // [1, 4, 2, 5, 3, 6]
    print(
        interleave(queue<int>{deque<int>{1, 2, 3, 4, 5}}));  // [1, 4, 2, 5, 3]
    print(interleave(queue<int>{deque<int>{1, 2}}));         // [1, 2]
    print(interleave(queue<int>{deque<int>{1}}));            // [1]
    print(interleave(queue<int>{}));                         // []

    return 0;
}

/**
 * Special cases: empty queue, one element, two elements, an odd number of
 * elements (the middle element ends up at the back), and an even number of
 * elements (the halves have equal size).
 *
 * A one-queue variant is possible by rotating the queue and pushing elements
 * back, but the two-helper-queue version is easier to read.
 */

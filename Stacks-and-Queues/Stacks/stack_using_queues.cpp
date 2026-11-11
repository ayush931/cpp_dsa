/**
 * Stack using queues
 *
 * A stack is LIFO while a queue is FIFO. To remove the newest stack element,
 * move every element except the last one into a second queue, remove the last
 * element, then swap the two queues.
 *
 * push -> O(1)
 * pop/top -> O(n)
 * empty/size -> O(1)
 * Space -> O(n)
 *
 * Example:
 * push(1), push(2), push(3)
 * top() -> 3
 * pop() -> 3
 * top() -> 2
 */

#include <iostream>
#include <queue>
#include <stdexcept>
using namespace std;

class StackUsingQueues {
    queue<int> primary;
    queue<int> secondary;

   public:
    void push(int value) {
        primary.push(value);
    }

    int pop() {
        if (primary.empty()) {
            throw underflow_error("pop from an empty stack");
        }

        // Leave exactly one element in primary: the newest stack element.
        while (primary.size() > 1) {
            secondary.push(primary.front());
            primary.pop();
        }

        int value = primary.front();
        primary.pop();

        swap(primary, secondary);
        return value;
    }

    int top() {
        int value = pop();
        primary.push(value);  // restore the element on top
        return value;
    }

    bool empty() const {
        return primary.empty();
    }

    int size() const {
        return primary.size();
    }
};

int main() {
    StackUsingQueues st;

    st.push(1);
    st.push(2);
    st.push(3);

    cout << st.top() << endl;   // 3
    cout << st.size() << endl;  // 3

    cout << st.pop() << endl;  // 3
    cout << st.top() << endl;  // 2

    st.push(4);
    cout << st.pop() << endl;  // 4
    cout << st.pop() << endl;  // 2
    cout << st.pop() << endl;  // 1

    cout << st.empty() << endl;  // 1
    cout << st.size() << endl;   // 0

    return 0;
}

/**
 * A one-queue variant is also possible: push the new value, then rotate the
 * queue by moving the previous size elements from front to back. That makes
 * push O(n) and pop/top O(1). This two-queue version keeps push O(1).
 */

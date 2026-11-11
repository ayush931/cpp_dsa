/**
 * Design a stack that supports push, pop, top and retrieving the minimum
 * element in constant time. Implement the MinStack class.
 *
 * You must implement a solution with O(1) time complexity for each function.
 *
 * Assumption: pop, top and getMin are always called on a non-empty stack.
 */

#include <climits>
#include <iostream>
#include <stack>
using namespace std;

#define ll long long int

class MinStack {
   public:
    stack<ll> st;
    ll mn;

    MinStack() {
        this->mn = INT_MAX;
    }

    void push(int val) {
        if (this->st.empty()) {
            this->mn = val;
            this->st.push(val);
        } else {
            // store the difference from the current minimum
            this->st.push((ll)val - this->mn);

            if (this->mn > val) {
                this->mn = val;
            }
        }
    }

    void pop() {
        if (not this->st.empty()) {
            if (this->st.top() >= 0) {
                this->st.pop();
            } else {
                // the minimum was stored here; restore the previous minimum
                this->mn = this->mn - this->st.top();
                this->st.pop();
            }
        }
    }

    int top() {
        if (this->st.size() == 1) {
            return this->st.top();
        } else if (this->st.top() < 0) {
            return this->mn;
        } else {
            return this->mn + this->st.top();
        }
    }

    int getMin() {
        return this->mn;
    }
};

int main() {
    MinStack *obj = new MinStack();

    obj->push(5);
    obj->push(3);
    obj->push(7);

    cout << obj->top() << endl;     // 7
    cout << obj->getMin() << endl;  // 3

    obj->pop();
    cout << obj->top() << endl;  // 3

    obj->pop();
    cout << obj->getMin() << endl;  // 5
    cout << obj->top() << endl;     // 5

    delete obj;

    return 0;
}

/**
 * Time complexity -> O(1) for push, pop, top and getMin
 * Space complexity -> O(n)
 * Special cases: pushing a new minimum, popping the current minimum,
 * duplicate minimum values, negative values, INT_MIN / INT_MAX
 * (the stored difference uses long long to avoid overflow)
 */

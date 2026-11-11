/**
 * We are given a stack data structure with push and pop operations, the task
 * is to implement a queue using instances of the stack data structure and
 * operations on them.
 *
 * This version keeps one stack and moves elements aside for pop/front, so
 * push is O(1) while pop and front are O(n).
 */

#include <bits/stdc++.h>
using namespace std;

class Queue {
    stack<int> st;

   public:
    Queue() {}

    void push(int x) {
        // Time: O(1)
        this->st.push(x);
    }

    void pop() {
        // Time: O(n)
        if (this->st.empty()) {
            return;
        }
        stack<int> tmp;
        while (this->st.size() > 1) {
            tmp.push(st.top());
            st.pop();
        }

        // now stack size is 1, and we are at the bottom element
        this->st.pop();
        while (not tmp.empty()) {
            this->st.push(tmp.top());
            tmp.pop();
        }
    }

    bool empty() {
        return this->st.empty();
    }

    int front() {
        // Time: O(n)
        if (this->st.empty()) {
            return -1;
        }
        stack<int> tmp;
        while (this->st.size() > 1) {
            tmp.push(st.top());
            st.pop();
        }

        int result = this->st.top();
        while (not tmp.empty()) {
            this->st.push(tmp.top());
            tmp.pop();
        }

        return result;
    }
};

int main() {
    Queue qu;
    qu.push(10);
    qu.push(20);
    qu.push(30);
    qu.push(40);

    qu.pop();

    while (not qu.empty()) {
        cout << qu.front() << " ";
        qu.pop();
    }
    cout << endl;

    return 0;
}
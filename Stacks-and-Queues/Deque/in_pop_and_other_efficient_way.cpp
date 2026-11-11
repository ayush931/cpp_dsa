// in pop and other efficient way

#include <bits/stdc++.h>
using namespace std;

class Queue {
    stack<int> st;

   public:
    Queue() {};

    void push(int x) {
        // Time:- O(n)
        stack<int> temp;
        while (not this->st.empty()) {
            temp.push(this->st.top());
            this->st.pop();
        }
        this->st.push(x);
        while (not temp.empty()) {
            this->st.push(temp.top());
            temp.pop();
        }
    }

    void pop() {
        // Time:- O(1)
        if (this->st.empty()) {
            return;
        }
        this->st.pop();
    }

    bool empty() {
        return this->st.empty();
    }

    int front() {
        // Time:- O(1)
        if (this->st.empty()) {
            return INT_MIN;
        }
        return this->st.top();
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

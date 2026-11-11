// stack using an array

#include <climits>
#include <iostream>
using namespace std;

class Stack {
    int capacity;
    int *arr;
    int top;

   public:
    Stack(int c) {
        this->capacity = c;
        arr = new int[c];
        this->top = -1;
    }

    ~Stack() {
        delete[] arr;
    }

    void push(int data) {
        if (this->top == this->capacity - 1) {
            cout << "Overflow" << endl;
            return;
        }

        this->top++;
        this->arr[this->top] = data;
    }

    int pop() {
        if (this->top == -1) {
            cout << "Underflow" << endl;
            return INT_MIN;
        }

        int value = this->arr[this->top];
        this->top--;
        return value;
    }

    int getTop() {
        if (this->top == -1) {
            cout << "Underflow" << endl;
            return INT_MIN;
        }
        return this->arr[this->top];
    }

    bool isEmpty() {
        return this->top == -1;
    }

    int size() {
        return this->top + 1;
    }

    bool isFull() {
        return this->top == this->capacity - 1;
    }
};

int main() {
    Stack st(5);
    st.push(1);
    st.push(2);
    st.push(3);

    cout << st.getTop() << endl;  // 3

    st.push(4);
    st.push(5);

    cout << st.getTop() << endl;  // 5
    cout << st.isFull() << endl;  // 1

    st.push(6);  // Overflow

    cout << st.pop() << endl;  // 5
    cout << st.pop() << endl;  // 4

    cout << st.getTop() << endl;  // 3
    cout << st.size() << endl;    // 3

    return 0;
}

// Special cases: pop on an empty stack (underflow), push on a full stack
// (overflow), capacity 0, destroying the stack frees the array

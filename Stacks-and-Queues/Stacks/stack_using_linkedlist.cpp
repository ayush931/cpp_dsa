// stack using linkedlist

#include <climits>
#include <iostream>
using namespace std;

class Node {
   public:
    int data;
    Node *next;

    Node(int data) {
        this->data = data;
        this->next = NULL;
    }
};

class Stack {
    Node *head;
    int capacity;
    int currSize;

   public:
    Stack(int c) {
        this->capacity = c;
        this->currSize = 0;
        head = NULL;
    }

    ~Stack() {
        while (head != NULL) {
            Node *next = head->next;
            delete head;
            head = next;
        }
    }

    bool isEmpty() {
        return this->head == NULL;
    }

    bool isFull() {
        return this->currSize == this->capacity;
    }

    void push(int data) {
        if (this->currSize == this->capacity) {
            cout << "Overflow" << endl;
            return;
        }
        Node *new_node = new Node(data);
        new_node->next = this->head;
        this->head = new_node;
        this->currSize++;
    }

    int pop() {
        if (this->head == NULL) {
            cout << "Underflow" << endl;
            return INT_MIN;
        }

        Node *toBeRemoved = this->head;
        int result = toBeRemoved->data;
        this->head = this->head->next;
        delete toBeRemoved;
        this->currSize--;
        return result;
    }

    int getTop() {
        if (this->head == NULL) {
            cout << "Underflow" << endl;
            return INT_MIN;
        }
        return this->head->data;
    }

    int size() {
        return this->currSize;
    }
};

int main() {
    Stack st(5);
    st.push(1);
    st.push(2);
    st.push(3);

    cout << st.getTop() << endl;   // 3
    cout << st.isEmpty() << endl;  // 0

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

// Special cases: pop on an empty stack, push on a full stack, capacity 0,
// the destructor frees every remaining node (no memory leak)

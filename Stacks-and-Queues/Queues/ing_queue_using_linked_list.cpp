// Implementing queue using linked list

// Space efficient

#include <bits/stdc++.h>
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

class Queue {
    Node *head;
    Node *tail;
    int size;

   public:
    Queue() {
        this->head = NULL;
        this->tail = NULL;
        this->size = 0;
    }

    ~Queue() {
        while (this->head != NULL) {
            Node *next = this->head->next;
            delete this->head;
            this->head = next;
        }
    }

    void enqueue(int data) {
        Node *new_node = new Node(data);
        if (this->head == NULL) {
            // LL is empty
            this->head = this->tail = new_node;
        } else {
            this->tail->next = new_node;
            this->tail = new_node;
        }
        this->size++;
    }

    void dequeue() {
        if (this->head == NULL) {
            // LL is empty
            return;
        } else {
            Node *old_head = this->head;
            Node *new_head = this->head->next;
            this->head = new_head;
            if (this->head == NULL) {
                this->tail = NULL;
            }
            old_head->next = NULL;
            delete old_head;
            this->size--;
        }
    }

    int getSize() {
        return this->size;
    }

    bool isEmpty() {
        return this->head == NULL;
    }

    int front() {
        if (this->head == NULL) {
            return -1;
        }
        return this->head->data;
    }
};

int main() {
    Queue qu;
    qu.enqueue(10);
    qu.enqueue(20);
    qu.enqueue(30);

    qu.dequeue();

    qu.enqueue(40);

    while (not qu.isEmpty()) {
        cout << qu.front() << " ";
        qu.dequeue();
    }
    cout << endl;

    return 0;
}
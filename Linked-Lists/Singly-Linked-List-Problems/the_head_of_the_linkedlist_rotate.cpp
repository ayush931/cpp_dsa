// Given the head of the linkedlist, rotate the list to the right by k places.

#include <bits/stdc++.h>
using namespace std;

class Node {
   public:
    int val;
    Node *next;

    Node(int data) {
        val = data;
        next = NULL;
    };
};

class LinkedList {
   public:
    Node *head;

    LinkedList() {
        head = NULL;
    }

    void insertAtTail(int value) {
        Node *new_node = new Node(value);
        if (head == NULL) {  // LinkedList is empty
            head = new_node;
            return;
        }

        Node *temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }

    void display() {
        Node *temp = head;
        while (temp != NULL) {
            cout << temp->val << "->";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }
};

Node *rotateByK(Node *&head, int k) {
    // 1. find the length of linkedlist
    int n = 0;

    // 2. find tail node
    Node *tail = head;

    while (tail->next) {
        n++;                // finding length
        tail = tail->next;  // finding last node
    }

    n++;  // for including last node

    k = k % n;

    if (k == 0) {
        return head;
    }

    tail->next = head;

    // 3. traverse n-k node
    Node *temp = head;

    for (int i = 1; i < n - k; i++) {
        temp = temp->next;
    }

    // temp is now pointing to (n - k) node

    Node *newHead = temp->next;
    temp->next = NULL;
    return newHead;
};

int main() {
    LinkedList ll1;
    ll1.insertAtTail(1);
    ll1.insertAtTail(2);
    ll1.insertAtTail(3);
    ll1.insertAtTail(4);
    ll1.insertAtTail(5);
    ll1.display();

    ll1.head = rotateByK(ll1.head, 4);
    ll1.display();

    return 0;
}

// Time complexity -> O(2n - k)
// Given a linkedlist, swap every two adjacent nodes and return its head. You
// may not modify the values in the list's nodes. Only nodes themselves may be
// changed.

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

Node *swapAdjacent(Node *&head) {
    // base case
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // recursvie case
    Node *secondHead = head->next;
    head->next = swapAdjacent(secondHead->next);
    secondHead->next =
        head;  // reversing the link between first and second node
    return secondHead;
};

int main() {
    LinkedList ll1;
    ll1.insertAtTail(1);
    ll1.insertAtTail(2);
    ll1.insertAtTail(3);
    ll1.insertAtTail(4);
    ll1.insertAtTail(5);
    ll1.insertAtTail(6);
    ll1.display();

    ll1.head = swapAdjacent(ll1.head);
    ll1.display();

    return 0;
}
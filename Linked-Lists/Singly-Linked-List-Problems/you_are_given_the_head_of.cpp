// You are given the head of a singly linkedlist. The list can be represented
// as: L0 -> L1 -> ... -> Ln-1 -> Ln Reorder the list to be on the following
// form: L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2.....

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

Node *reorderLinkedList(Node *&head) {
    // can check if linkedlist has atleast 3 nodes

    // 1. finding the middle element
    Node *slow = head;
    Node *fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // now slow is pointing to the middle element
    // 2. separate the linkedlist and reverse second half
    Node *curr = slow->next;
    slow->next = NULL;
    Node *prev = slow;

    while (curr) {
        Node *nextPtr = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextPtr;
    }

    // 3. merging the two havles of the linked list
    Node *ptr1 = head;  // linkedlist first half
    Node *ptr2 = prev;  // linkedlist second half

    while (ptr1 != ptr2) {
        Node *temp = ptr1->next;
        ptr1->next = ptr2;
        ptr1 = ptr2;
        ptr2 = temp;
    }

    return head;
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

    ll1.head = reorderLinkedList(ll1.head);
    ll1.display();

    return 0;
}
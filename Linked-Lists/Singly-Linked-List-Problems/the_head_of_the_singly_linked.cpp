// Given the head of the singly linked list, group all the nodes with odd
// indices together followed by the nodes with the even indices, and return the
// reordered list.

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

Node *oddEvenLinkedList(Node *&head) {
    if (!head) {
        return head;
    }

    Node *evenHead = head->next;
    Node *oddPtr = head;
    Node *evenPtr = evenHead;

    while (evenPtr && evenPtr->next) {
        oddPtr->next = oddPtr->next->next;
        evenPtr->next = evenPtr->next->next;
        oddPtr = oddPtr->next;
        evenPtr = evenPtr->next;
    }

    oddPtr->next = evenHead;
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

    ll1.head = oddEvenLinkedList(ll1.head);
    ll1.display();

    return 0;
}

// Time complexity -> O(n)
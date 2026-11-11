// Given 2 sorted linked lists, merge them into 1 singly linked list such that
// resulting list is also sorted.

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

Node *mergeLinkedLists(Node *&head1, Node *&head2) {
    Node *dummyHeadNode = new Node(-1);

    Node *ptr1 = head1;
    Node *ptr2 = head2;
    Node *ptr3 = dummyHeadNode;

    while (ptr1 && ptr2) {
        if (ptr1->val < ptr2->val) {
            ptr3->next = ptr1;
            ptr1 = ptr1->next;
        } else {
            ptr3->next = ptr2;
            ptr2 = ptr2->next;
        }
        ptr3 = ptr3->next;
    }

    if (ptr1) {
        ptr3->next = ptr1;
    } else {
        ptr3->next = ptr2;
    }

    Node *mergedHead = dummyHeadNode->next;
    delete dummyHeadNode;  // the dummy node itself is not part of the result
    return mergedHead;
}

int main() {
    LinkedList ll1;
    ll1.insertAtTail(1);
    ll1.insertAtTail(4);
    ll1.insertAtTail(5);
    ll1.display();

    LinkedList ll2;
    ll2.insertAtTail(2);
    ll2.insertAtTail(3);
    ll2.display();

    LinkedList ll3;
    ll3.head = mergeLinkedLists(ll1.head, ll2.head);
    ll3.display();

    return 0;
}

// Time complexity -> O(n + m);
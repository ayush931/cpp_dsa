// Given the head of the LinkedList, remove the kth node from the end of the
// LinkedList and return its head.

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

// assuming k is less than / equal to length of LinkedList
void removeKthNodeFromTheEnd(Node *&head, int k) {
    Node *ptr1 = head;
    Node *ptr2 = head;

    // move ptr2 by k steps ahead
    int count = k;
    while (count--) {
        ptr2 = ptr2->next;
    }

    if (ptr2 == NULL) {  // k is equal to length of the LinkedList
        // we have to delete the head node
        Node *temp = head;
        head = head->next;
        delete temp;
        return;
    }

    // now ptr2 is k steps ahead of ptr1
    // when ptr2 is a null(end of the list), ptr1 will be at the node to be
    // deleted
    while (ptr2->next != NULL) {
        ptr1 = ptr1->next;
        ptr2 = ptr2->next;
    }

    // now ptr1 is pointing to the node before kth node from the end
    // node to be deleted is ptr1->next
    Node *temp = ptr1->next;
    ptr1->next = ptr1->next->next;
    delete temp;
};

int main() {
    LinkedList ll1;
    ll1.insertAtTail(1);
    ll1.insertAtTail(2);
    ll1.insertAtTail(3);
    ll1.insertAtTail(4);
    ll1.insertAtTail(5);
    ll1.display();

    removeKthNodeFromTheEnd(ll1.head, 3);
    ll1.display();

    return 0;
}
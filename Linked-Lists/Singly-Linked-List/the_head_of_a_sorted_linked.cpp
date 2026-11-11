// Given the head of a sorted linked list, delete all the duplicates such that
// each element appears only once. Return the linked list sorted as well.

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

void deleteDuplicateNodes(Node *&head) {
    Node *current_node = head;
    while (current_node) {
        while (current_node->next &&
               current_node->val == current_node->next->val) {
            // delete current_node->next
            Node *temp = current_node->next;  // node to be deleted
            current_node->next = current_node->next->next;
            delete temp;
        }

        // this loop ends when current node and the next node values are
        // different or linked list ends
        current_node = current_node->next;
    };
};

int main() {
    LinkedList ll;
    ll.insertAtTail(1);
    ll.insertAtTail(2);
    ll.insertAtTail(2);
    ll.insertAtTail(3);
    ll.insertAtTail(3);
    ll.insertAtTail(4);
    ll.display();

    deleteDuplicateNodes(ll.head);
    ll.display();

    return 0;
}
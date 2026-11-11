// Given the head of a linked list, delete every alternate element from the list
// starting from the second element.

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

void deleteAlternateNode(Node *&head) {
    Node *current_node = head;
    while (current_node != NULL && current_node->next != NULL) {
        Node *temp = current_node->next;  // this is the node to be deleted
        current_node->next = current_node->next->next;
        delete temp;
        current_node = current_node->next;
    };
};

int main() {
    LinkedList ll;
    ll.insertAtTail(1);
    ll.insertAtTail(2);
    ll.insertAtTail(3);
    ll.insertAtTail(4);
    ll.insertAtTail(5);
    ll.display();

    deleteAlternateNode(ll.head);
    ll.display();

    return 0;
}
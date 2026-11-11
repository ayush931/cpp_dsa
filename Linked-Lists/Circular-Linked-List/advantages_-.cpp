/**
 * Advantages:-
 * 1. We can start traversing from any node to print all nodes until we reach
 * visited nodes.
 * 2. We can have traversed linked list in circular fashion multiple times.
 */

/**
 * Template class:- they allow you to pass the datatype as a parameter
 */

#include <iostream>
using namespace std;

class Node {
   public:
    int val;
    Node *next;

    Node(int data) {
        val = data;
        next = NULL;
    }
};

class CircularLinkedList {
   public:
    Node *head;

    CircularLinkedList() {
        head = NULL;
    }

    ~CircularLinkedList() {
        if (head == NULL) {
            return;
        }

        // Break the circle first so the normal traversal can delete every node.
        Node *tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }
        tail->next = NULL;

        Node *current = head;
        while (current != NULL) {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }

    void display() {
        if (head == NULL) {
            cout << "empty" << endl;
            return;
        }

        Node *temp = head;
        do {
            cout << temp->val << "->";
            temp = temp->next;
        } while (temp != head);
        cout << endl;
    };

    void printCircular() {
        if (head == NULL) {
            return;
        }

        Node *temp = head;
        for (int i = 0; i < 15; i++) {
            cout << temp->val << "->";
            temp = temp->next;
        }
        cout << endl;
    }

    void insertAtStart(int val) {
        Node *new_node = new Node(val);
        if (head == NULL) {
            head = new_node;
            new_node->next = head;  // circular linked list
            return;
        }

        Node *tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }

        // now tail is pointed to the last node

        tail->next = new_node;
        new_node->next = head;
        head = new_node;
    }

    void insertAtEnd(int val) {
        Node *new_node = new Node(val);
        if (head == NULL) {
            head = new_node;
            new_node->next = head;  // circular linked list
            return;
        }

        Node *tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }

        // here tail will be pointed to next node.
        tail->next = new_node;
        new_node->next = head;
    }

    void deleteAtStart() {
        if (head == NULL) {
            return;
        }

        if (head->next == head) {  // only one node
            delete head;
            head = NULL;
            return;
        }

        Node *temp = head;
        Node *tail = head;
        while (tail->next != head) {
            tail = tail->next;
        }

        head = head->next;
        tail->next = head;
        delete temp;
    }

    void deleteAtEnd() {
        if (head == NULL) {
            return;
        }

        if (head->next == head) {  // only one node
            delete head;
            head = NULL;
            return;
        }

        Node *tail = head;
        while (tail->next->next != head) {
            tail = tail->next;
        }

        // here the tail is pointed to the second last node

        Node *temp = tail->next;  // to be deleted
        tail->next = head;
        delete temp;
    }
};

int main() {
    CircularLinkedList cll;

    cll.insertAtStart(3);
    cll.insertAtStart(2);
    cll.insertAtStart(1);
    cll.display();
    cll.printCircular();

    cll.insertAtEnd(4);
    cll.insertAtEnd(5);
    cll.insertAtEnd(6);

    cll.display();

    cll.deleteAtStart();
    cll.display();

    cll.deleteAtEnd();
    cll.display();

    return 0;
}

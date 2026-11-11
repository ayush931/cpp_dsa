// Operations on a doubly linked list:
//   1. insertion at the start
//   2. insertion at the end
//   3. insertion at the kth position (1-based)
//   4. deletion from the start
//   5. deletion from the end
//   6. deletion from the kth position (1-based)
//
// Example:
//   insertAtEnd(1), insertAtEnd(2), insertAtStart(0) -> 0 <-> 1 <-> 2
//   deleteAtPosition(2)                              -> 0 <-> 2

#include <iostream>
using namespace std;

class Node {
   public:
    int val;
    Node *prev;
    Node *next;

    Node(int data) {
        val = data;
        prev = NULL;
        next = NULL;
    }
};

class DoublyLinkedList {
   public:
    Node *head;
    Node *tail;

    DoublyLinkedList() {
        head = NULL;
        tail = NULL;
    }

    ~DoublyLinkedList() {
        Node *current = head;
        while (current != NULL) {
            Node *next = current->next;
            delete current;
            current = next;
        }
    }

    void display() {
        Node *temp = head;
        while (temp != NULL) {
            cout << temp->val << "<->";
            temp = temp->next;
        }
        cout << endl;
    }

    void insertAtStart(int val) {
        Node *new_node = new Node(val);

        if (head == NULL) {
            head = new_node;
            tail = new_node;
            return;
        }

        new_node->next = head;
        head->prev = new_node;
        head = new_node;
    }

    void insertAtEnd(int val) {
        Node *new_node = new Node(val);

        if (tail == NULL) {
            head = new_node;
            tail = new_node;
            return;
        }

        tail->next = new_node;
        new_node->prev = tail;
        tail = new_node;
    }

    void insertAtPosition(int val, int k) {
        if (k <= 1 || head == NULL) {
            insertAtStart(val);
            return;
        }

        Node *temp = head;
        int count = 1;

        // Stop at the node just before position k, or at the tail.
        while (temp->next != NULL && count < k - 1) {
            temp = temp->next;
            count++;
        }

        if (temp->next == NULL) {
            insertAtEnd(val);
            return;
        }

        Node *new_node = new Node(val);
        new_node->next = temp->next;
        new_node->prev = temp;
        temp->next->prev = new_node;
        temp->next = new_node;
    }

    void deleteAtStart() {
        if (head == NULL) {
            return;
        }

        Node *temp = head;

        if (head == tail) {  // only one node
            head = NULL;
            tail = NULL;
            delete temp;
            return;
        }

        head = head->next;
        head->prev = NULL;
        delete temp;
    }

    void deleteAtEnd() {
        if (head == NULL) {
            return;
        }

        Node *temp = tail;

        if (head == tail) {  // only one node
            head = NULL;
            tail = NULL;
            delete temp;
            return;
        }

        tail = tail->prev;
        tail->next = NULL;
        delete temp;
    }

    void deleteAtPosition(int k) {
        if (head == NULL || k < 1) {
            return;
        }

        if (k == 1) {
            deleteAtStart();
            return;
        }

        Node *temp = head;
        int counter = 1;

        while (temp != NULL && counter < k) {
            temp = temp->next;
            counter++;
        }

        if (temp == NULL) {
            return;  // k is beyond the length
        }

        if (temp == tail) {
            deleteAtEnd();
            return;
        }

        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
        delete temp;
    }
};

int main() {
    DoublyLinkedList dll;

    dll.insertAtStart(1);
    dll.display();  // 1<->

    dll.insertAtStart(2);
    dll.display();  // 2<->1<->

    dll.insertAtStart(3);
    dll.display();  // 3<->2<->1<->

    dll.insertAtEnd(4);
    dll.display();  // 3<->2<->1<->4<->

    dll.insertAtEnd(5);
    dll.display();  // 3<->2<->1<->4<->5<->

    dll.insertAtEnd(6);
    dll.display();  // 3<->2<->1<->4<->5<->6<->

    dll.insertAtPosition(9, 4);
    dll.display();  // 3<->2<->1<->9<->4<->5<->6<->

    dll.deleteAtStart();
    dll.display();  // 2<->1<->9<->4<->5<->6<->

    dll.deleteAtEnd();
    dll.display();  // 2<->1<->9<->4<->5<->

    dll.deleteAtPosition(3);
    dll.display();  // 2<->1<->4<->5<->

    return 0;
}

// Time complexity -> O(1) at the ends, O(k) at the kth position
// Space complexity -> O(1) extra

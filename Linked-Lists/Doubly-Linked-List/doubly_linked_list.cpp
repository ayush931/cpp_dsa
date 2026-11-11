// Doubly linked list

/**
 * Advantages:-
 * 1. Traversal both ways
 * 2. Insertion and deletion becomes more efficient
 */

// Disadvantages:- Extra space for previous node pointer

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
};

int main() {
    Node *new_node = new Node(3);
    DoublyLinkedList dll;

    dll.head = new_node;
    dll.tail = new_node;

    cout << dll.head->val << endl;
    cout << dll.tail->val << endl;

    delete new_node;  // release the node allocated above

    return 0;
}

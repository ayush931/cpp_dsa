// Given the head of a linked list, reverse the nodes of the list k at a time
// and return the modified list. If the number of nodes left at the end is less
// than k, leave those nodes in their original order.
//
// Example:
//   1 -> 2 -> 3 -> 4 -> 5, k = 2  ->  2 -> 1 -> 4 -> 3 -> 5
//   1 -> 2 -> 3 -> 4 -> 5, k = 3  ->  3 -> 2 -> 1 -> 4 -> 5

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

Node *reverseKLinked(Node *head, int k) {
    if (k <= 1 || head == NULL) {
        return head;
    }

    // Check that a complete group of k nodes exists before reversing it.
    Node *check = head;
    int count = 0;
    while (check != NULL && count < k) {
        check = check->next;
        count++;
    }

    if (count < k) {
        return head;  // leftover group stays unchanged
    }

    Node *prevPtr = NULL;
    Node *currPtr = head;
    count = 0;

    while (currPtr != NULL && count < k) {
        Node *nextPtr = currPtr->next;
        currPtr->next = prevPtr;
        prevPtr = currPtr;
        currPtr = nextPtr;
        count++;
    }

    // currPtr is the first node of the next group; head is now the tail of this
    // reversed group.
    head->next = reverseKLinked(currPtr, k);

    return prevPtr;
}

int main() {
    LinkedList ll;
    ll.insertAtTail(1);
    ll.insertAtTail(2);
    ll.insertAtTail(3);
    ll.insertAtTail(4);
    ll.insertAtTail(5);
    ll.display();  // 1->2->3->4->5->NULL

    ll.head = reverseKLinked(ll.head, 2);
    ll.display();  // 2->1->4->3->5->NULL

    LinkedList second;
    for (int i = 1; i <= 5; i++) {
        second.insertAtTail(i);
    }

    second.head = reverseKLinked(second.head, 3);
    second.display();  // 3->2->1->4->5->NULL

    return 0;
}

// Time complexity -> O(n)
// Space complexity -> O(n / k) recursion depth, O(1) extra per call

// Insertion, update, and deletion in a singly linked list.
//
// Positions are 0-based. Inserting at a position past the end appends to the
// tail, which keeps the demo safe for out-of-range input.
//
// Example:
//   insertAtTail(1), insertAtTail(2), insertAtHead(0) -> 0 -> 1 -> 2
//   insertAtPosition(..., 1, 9)                       -> 0 -> 9 -> 1 -> 2

#include <bits/stdc++.h>
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

int lengthOfList(Node *head) {
    int length = 0;
    while (head != NULL) {
        length++;
        head = head->next;
    }
    return length;
}

void insertAtHead(Node *&head, int val) {
    Node *new_node = new Node(val);
    new_node->next = head;
    head = new_node;
}

void insertAtTail(Node *&head, int val) {
    Node *new_node = new Node(val);

    if (head == NULL) {
        head = new_node;
        return;
    }

    Node *temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = new_node;
}

void insertAtPosition(Node *&head, int pos, int val) {
    if (pos <= 0) {
        insertAtHead(head, val);
        return;
    }

    if (pos >= lengthOfList(head)) {
        insertAtTail(head, val);
        return;
    }

    Node *new_node = new Node(val);
    Node *temp = head;

    for (int current_pos = 0; current_pos < pos - 1; current_pos++) {
        temp = temp->next;
    }

    // temp is the node at pos - 1.
    new_node->next = temp->next;
    temp->next = new_node;
}

void updateAtPosition(Node *head, int pos, int val) {
    Node *temp = head;
    int current_pos = 0;

    while (temp != NULL && current_pos != pos) {
        temp = temp->next;
        current_pos++;
    }

    if (temp == NULL) {
        return;  // position is outside the list
    }

    temp->val = val;
}

void deleteAtHead(Node *&head) {
    if (head == NULL) {
        return;
    }

    Node *temp = head;
    head = head->next;
    delete temp;
}

void deleteAtTail(Node *&head) {
    if (head == NULL) {
        return;
    }

    if (head->next == NULL) {
        delete head;
        head = NULL;
        return;
    }

    Node *second_last = head;
    while (second_last->next->next != NULL) {
        second_last = second_last->next;
    }

    Node *temp = second_last->next;
    second_last->next = NULL;
    delete temp;
}

void deleteAtPosition(Node *&head, int pos) {
    if (head == NULL || pos < 0) {
        return;
    }

    if (pos == 0) {
        deleteAtHead(head);
        return;
    }

    int n = lengthOfList(head);
    if (pos >= n) {
        return;  // position is outside the list
    }

    if (pos == n - 1) {
        deleteAtTail(head);
        return;
    }

    Node *prev = head;
    for (int current_pos = 0; current_pos < pos - 1; current_pos++) {
        prev = prev->next;
    }

    Node *temp = prev->next;
    prev->next = temp->next;
    delete temp;
}

void display(Node *head) {
    Node *temp = head;
    while (temp != NULL) {
        cout << temp->val << "->";
        temp = temp->next;
    }

    cout << "NULL" << endl;
}

void freeList(Node *&head) {
    while (head != NULL) {
        Node *next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    Node *head = NULL;
    insertAtHead(head, 2);
    display(head);  // 2->NULL

    insertAtHead(head, 1);
    display(head);  // 1->2->NULL

    insertAtTail(head, 3);
    display(head);  // 1->2->3->NULL

    insertAtPosition(head, 1, 9);
    display(head);  // 1->9->2->3->NULL

    updateAtPosition(head, 3, 5);
    display(head);  // 1->9->2->5->NULL

    deleteAtHead(head);
    display(head);  // 9->2->5->NULL

    deleteAtTail(head);
    display(head);  // 9->2->NULL

    deleteAtPosition(head, 1);
    display(head);  // 9->NULL

    freeList(head);
    return 0;
}

// Time complexity (insert/delete at head) -> O(1)
// Time complexity (insert/delete at tail or kth position) -> O(n)
// Space complexity -> O(1) extra

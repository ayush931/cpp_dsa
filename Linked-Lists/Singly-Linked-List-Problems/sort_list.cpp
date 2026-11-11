/**
 * Sort an acyclic singly linked list by stable merge sort.
 * Split with fast/slow pointers, recursively sort, merge existing nodes.
 * O(n log n) time; O(log n) recursion stack; no new list nodes allocated.
 * Prerequisites: merge two sorted lists and finding the middle node.
 */

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

struct Node {
    int value;
    Node *next = nullptr;
};

Node *merge(Node *left, Node *right) {
    Node dummy{0};
    Node *tail = &dummy;
    while (left && right) {
        if (left->value <= right->value) {
            tail->next = left;
            left = left->next;
        } else {
            tail->next = right;
            right = right->next;
        }
        tail = tail->next;
    }
    tail->next = left ? left : right;
    return dummy.next;
}

Node *sortList(Node *head) {
    if (!head || !head->next) {
        return head;
    }
    Node *slow = head, *fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    Node *right = slow->next;
    slow->next = nullptr;  // disconnect halves so recursive calls get smaller
    Node *leftSorted = sortList(head);
    Node *rightSorted = sortList(right);
    return merge(leftSorted, rightSorted);
}

int main() {
    Node fourth{2}, third{1, &fourth}, second{2, &third}, first{4, &second};
    Node *head = sortList(&first);
    vector<int> result;
    for (Node *node = head; node; node = node->next) {
        result.push_back(node->value);
    }
    assert((result == vector<int>{1, 2, 2, 4}));
    assert(head->next ==
           &second);  // original first 2 precedes original second 2
    assert(sortList(nullptr) == nullptr);
    Node single{-1};
    assert(sortList(&single) == &single);
    for (int value : result) {
        cout << value << ' ';
    }
    cout << '\n';  // 1 2 2 4
}

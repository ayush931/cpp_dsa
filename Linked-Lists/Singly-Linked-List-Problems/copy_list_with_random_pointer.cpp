/**
 * Copy List with Random Pointer
 *
 * A linked list of length n is given such that each node contains an extra
 * random pointer, which could point to any node in the list or to null.
 * Return a deep copy of the list.
 *
 * Example:
 * Input:  head = [[7, null], [13, 0], [11, 4], [10, 2], [1, 0]]
 * Output: a deep copy with the same values and random links
 *
 * Constraints: 0 <= n <= 1000, -10^4 <= Node.val <= 10^4.
 */

#include <iostream>
#include <unordered_map>
using namespace std;

class Node {
   public:
    int val;
    Node *next;
    Node *random;

    Node(int value) {
        val = value;
        next = NULL;
        random = NULL;
    }
};

// Brute force: map every original node to its copy -> O(n) time, O(n) space
Node *copyRandomListBrute(Node *head) {
    if (head == NULL) {
        return NULL;
    }

    unordered_map<Node *, Node *> copies;

    for (Node *current = head; current != NULL; current = current->next) {
        copies[current] = new Node(current->val);
    }

    for (Node *current = head; current != NULL; current = current->next) {
        copies[current]->next =
            (current->next == NULL) ? NULL : copies[current->next];
        copies[current]->random =
            (current->random == NULL) ? NULL : copies[current->random];
    }

    return copies[head];
}

// Optimized: interleave copies with originals, then split -> O(n), O(1) extra
Node *copyRandomList(Node *head) {
    if (head == NULL) {
        return NULL;
    }

    // Step 1: insert a copy after every original node.
    for (Node *current = head; current != NULL; current = current->next->next) {
        Node *copy = new Node(current->val);
        copy->next = current->next;
        current->next = copy;
    }

    // Step 2: set the random pointer of each copy.
    for (Node *current = head; current != NULL; current = current->next->next) {
        if (current->random != NULL) {
            current->next->random = current->random->next;
        }
    }

    // Step 3: separate the two lists.
    Node *newHead = head->next;

    for (Node *current = head; current != NULL;) {
        Node *copy = current->next;
        current->next = copy->next;
        copy->next = (copy->next == NULL) ? NULL : copy->next->next;
        current = current->next;
    }

    return newHead;
}

void printRandomList(Node *head) {
    for (Node *current = head; current != NULL; current = current->next) {
        cout << current->val << "(";

        if (current->random == NULL) {
            cout << "null";
        } else {
            cout << current->random->val;
        }

        cout << ") ";
    }
    cout << endl;
}

void freeList(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    Node *first = new Node(7);
    Node *second = new Node(13);
    Node *third = new Node(11);

    first->next = second;
    second->next = third;

    first->random = NULL;
    second->random = first;
    third->random = second;

    Node *bruteCopy = copyRandomListBrute(first);
    printRandomList(bruteCopy);  // 7(null) 13(7) 11(13)

    Node *optimizedCopy = copyRandomList(first);
    printRandomList(optimizedCopy);  // 7(null) 13(7) 11(13)

    Node *empty = NULL;
    cout << (copyRandomList(empty) == NULL) << endl;  // 1

    freeList(bruteCopy);
    freeList(optimizedCopy);
    freeList(first);

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n) time, O(n) extra space
 *   interleaving -> O(n) time, O(1) extra space (the copy itself is O(n))
 *
 * Special cases: empty list, one node, random pointer is null, random pointer
 * points to itself, random pointer points backward or forward, duplicate
 * values (always copy by node identity, never by value).
 */

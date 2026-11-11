/**
 * Given the head of the doubly linked list. The values of the linked list are
 * sorted in non-decreasing order. First if there exists a pair of distinct
 * nodes such that the sum of their values is x. Return the pair in the form of
 * a vector[l, r], where l and r are the values stored in the 2 nodes pointed by
 * the pointers. If there are multiple such pairs, return any of them. If there
 * is no such pair return [-1, -1].
 */

#include <climits>
#include <iostream>
#include <vector>
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

    void display() {
        Node *temp = head;
        while (temp != NULL) {
            cout << temp->val << "<->";
            temp = temp->next;
        }
        cout << endl;
    };

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
        return;
    };
};

vector<int> pairSumDLL(Node *head, Node *tail, int x) {
    vector<int> ans(2, -1);

    while (tail != head) {
        int sum = head->val + tail->val;
        if (sum == x) {
            ans[0] = head->val;
            ans[1] = tail->val;
            return ans;
        }

        if (sum > x) {  // i need smaller values, i will move tail back
            tail = tail->prev;
        } else {  // i need bigger values, i will move head forward
            head = head->next;
        }
    }

    return ans;
}

int main() {
    DoublyLinkedList dll;

    dll.insertAtEnd(2);
    dll.insertAtEnd(5);
    dll.insertAtEnd(6);
    dll.insertAtEnd(8);
    dll.insertAtEnd(10);

    vector<int> ans = pairSumDLL(dll.head, dll.tail, 11);
    cout << ans[0] << " " << ans[1] << endl;

    return 0;
}

// Time complexity -> O(n)
// Space complexity -> O(1)
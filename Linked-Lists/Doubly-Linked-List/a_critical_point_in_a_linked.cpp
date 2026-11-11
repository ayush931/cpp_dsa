/**
 * A critical point in a linked list is defined as either a local maxima or a
 local minima. Given a linked list tail, return an array of length 2 containing
 [minDistance, maxDistance] where minDistance is the minimum distance between
 any two distinct critical points and maxDistance is the maximum distance
 between any two distinct critical points. If there are fewer than two critical
 points, return [-1, -1].

 * Note that a node can only be a local maxima/minima if there exists both a
 previous node and a next node.
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

bool isCriticalPoint(Node *&currNode) {
    if (currNode->prev->val < currNode->val &&
        currNode->next->val < currNode->val) {  // local maxima
        return true;
    }
    if (currNode->prev->val > currNode->val &&
        currNode->next->val > currNode->val) {  // local minima
        return true;
    }

    return false;
}

vector<int> distanceBetweenCriticalPoint(Node *head, Node *tail) {
    vector<int> ans(2, -1);
    if (head == NULL || head->next == NULL) {
        return ans;
    }

    ans[0] = ans[1] = INT_MAX;
    int firstCp = -1, lastCp = -1;

    Node *currNode = tail->prev;
    int currPos = 0;
    while (currNode->prev != NULL) {
        if (isCriticalPoint(currNode)) {
            if (firstCp == -1) {
                firstCp = lastCp = currPos;
            } else {
                ans[0] = min(ans[0], currPos - lastCp);  // min distance
                ans[1] = currPos - firstCp;              // max distance
                lastCp = currPos;
            }
        }

        currPos++;
        currNode = currNode->prev;
    }

    if (ans[0] == INT_MAX) {
        ans[0] = ans[1] = -1;
    }

    return ans;
};

int main() {
    DoublyLinkedList dll;

    dll.insertAtEnd(1);
    dll.insertAtEnd(5);  // cp
    dll.insertAtEnd(4);
    dll.insertAtEnd(2);  // cp
    dll.insertAtEnd(6);  // cp
    dll.insertAtEnd(3);
    dll.display();

    vector<int> ans = distanceBetweenCriticalPoint(dll.head, dll.tail);
    cout << ans[0] << " " << ans[1] << endl;

    return 0;
}
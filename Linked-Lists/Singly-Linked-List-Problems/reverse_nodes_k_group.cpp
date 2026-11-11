/**
 * Reverse nodes in groups of k; leave an incomplete final group unchanged.
 * Rewire links, not values. A dummy head makes the first group ordinary.
 * O(n) time, O(1) auxiliary space. Contract: acyclic input, k > 0.
 */

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

struct Node {
    int value;
    Node *next = nullptr;
};

Node *reverseKGroup(Node *head, int k) {
    if (k <= 0) {
        throw invalid_argument("k must be positive");
    }
    Node dummy{0, head};
    Node *beforeGroup = &dummy;
    while (true) {
        Node *last = beforeGroup;
        for (int count = 0; count < k && last; ++count) {
            last = last->next;
        }
        if (!last) {
            break;  // fewer than k nodes left
        }
        Node *afterGroup = last->next;
        Node *previous = afterGroup;
        Node *current = beforeGroup->next;
        while (current != afterGroup) {
            Node *next = current->next;
            current->next = previous;
            previous = current;
            current = next;
        }
        Node *newTail = beforeGroup->next;  // old head becomes tail
        beforeGroup->next = last;
        beforeGroup = newTail;
    }
    return dummy.next;
}

vector<int> values(Node *head) {
    vector<int> result;
    for (; head; head = head->next) {
        result.push_back(head->value);
    }
    return result;
}

int main() {
    // Stack-owned nodes keep the demonstration free of allocation/leak
    // concerns.
    Node fifth{5}, fourth{4, &fifth}, third{3, &fourth};
    Node second{2, &third}, first{1, &second};
    Node *head = reverseKGroup(&first, 2);
    assert((values(head) == vector<int>{2, 1, 4, 3, 5}));
    assert(reverseKGroup(head, 1) == head);
    assert(reverseKGroup(head, 10) == head);
    assert(reverseKGroup(nullptr, 3) == nullptr);
    for (int value : values(head)) {
        cout << value << ' ';
    }
    cout << '\n';  // 2 1 4 3 5
}

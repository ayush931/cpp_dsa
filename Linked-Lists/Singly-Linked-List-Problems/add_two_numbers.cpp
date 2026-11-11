/**
 * Add Two Numbers
 *
 * You are given two non-empty linked lists representing two non-negative
 * integers. The digits are stored in reverse order, and each node contains a
 * single digit. Add the two numbers and return the sum as a linked list.
 *
 * Example:
 * Input:  l1 = [2, 4, 3], l2 = [5, 6, 4]
 * Output: [7, 0, 8]   (342 + 465 = 807)
 *
 * Constraints: 1 <= nodes per list <= 100, 0 <= digit <= 9.
 */

#include <iostream>
#include <vector>
using namespace std;

class ListNode {
   public:
    int val;
    ListNode *next;

    ListNode(int value) {
        val = value;
        next = NULL;
    }
};

ListNode *buildList(vector<int> values) {
    ListNode *dummy = new ListNode(0);
    ListNode *tail = dummy;

    for (int value : values) {
        tail->next = new ListNode(value);
        tail = tail->next;
    }

    ListNode *result = dummy->next;
    delete dummy;
    return result;
}

void freeList(ListNode *head) {
    while (head != NULL) {
        ListNode *next = head->next;
        delete head;
        head = next;
    }
}

void printList(ListNode *head) {
    while (head != NULL) {
        cout << head->val;

        if (head->next != NULL) {
            cout << " -> ";
        }

        head = head->next;
    }
    cout << endl;
}

// Brute force: convert both lists to integers, add, then rebuild the list.
// This overflows for lists longer than about 19 digits.
ListNode *addTwoNumbersBrute(ListNode *l1, ListNode *l2) {
    long long first = 0;
    long long place = 1;

    for (ListNode *current = l1; current != NULL; current = current->next) {
        first += current->val * place;
        place *= 10;
    }

    long long second = 0;
    place = 1;

    for (ListNode *current = l2; current != NULL; current = current->next) {
        second += current->val * place;
        place *= 10;
    }

    long long sum = first + second;

    if (sum == 0) {
        return new ListNode(0);
    }

    ListNode *dummy = new ListNode(0);
    ListNode *tail = dummy;

    while (sum > 0) {
        tail->next = new ListNode(sum % 10);
        tail = tail->next;
        sum /= 10;
    }

    ListNode *result = dummy->next;
    delete dummy;
    return result;
}

// Optimized: add digit by digit with a carry -> O(max(m, n))
ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    ListNode *dummy = new ListNode(0);
    ListNode *tail = dummy;
    int carry = 0;

    while (l1 != NULL || l2 != NULL || carry != 0) {
        int sum = carry;

        if (l1 != NULL) {
            sum += l1->val;
            l1 = l1->next;
        }

        if (l2 != NULL) {
            sum += l2->val;
            l2 = l2->next;
        }

        carry = sum / 10;
        tail->next = new ListNode(sum % 10);
        tail = tail->next;
    }

    ListNode *result = dummy->next;
    delete dummy;
    return result;
}

int main() {
    ListNode *first = buildList({2, 4, 3});
    ListNode *second = buildList({5, 6, 4});

    ListNode *bruteResult = addTwoNumbersBrute(first, second);
    printList(bruteResult);  // 7 -> 0 -> 8

    ListNode *optimizedResult = addTwoNumbers(first, second);
    printList(optimizedResult);  // 7 -> 0 -> 8

    freeList(bruteResult);
    freeList(optimizedResult);

    ListNode *third = buildList({9, 9, 9, 9, 9, 9, 9});
    ListNode *fourth = buildList({9, 9, 9, 9});
    ListNode *longResult = addTwoNumbers(third, fourth);
    printList(longResult);  // 8 -> 9 -> 9 -> 9 -> 0 -> 0 -> 0 -> 1

    ListNode *zero1 = buildList({0});
    ListNode *zero2 = buildList({0});
    ListNode *zeroResult = addTwoNumbers(zero1, zero2);
    printList(zeroResult);  // 0

    ListNode *small = buildList({1});
    ListNode *large = buildList({9, 9});
    ListNode *smallResult = addTwoNumbers(small, large);
    printList(smallResult);  // 0 -> 0 -> 1

    freeList(first);
    freeList(second);
    freeList(third);
    freeList(fourth);
    freeList(zero1);
    freeList(zero2);
    freeList(small);
    freeList(large);
    freeList(longResult);
    freeList(zeroResult);
    freeList(smallResult);

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(m + n) but overflows for long lists
 *   digit by digit -> O(max(m, n)) time, O(max(m, n)) space for the result
 *
 * Special cases: different lengths, a final carry (999 + 1 = 1000), zeros,
 * one list longer than the other, lists with 100 digits (the brute force
 * cannot handle these).
 */

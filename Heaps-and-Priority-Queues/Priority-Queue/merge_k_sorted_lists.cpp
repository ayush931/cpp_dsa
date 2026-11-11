/**
 * Merge k Sorted Lists
 *
 * You are given an array of k linked lists, each sorted in ascending order.
 * Merge all the lists into one sorted linked list and return its head.
 *
 * Example:
 * Input:  [[1,4,5], [1,3,4], [2,6]]
 * Output: [1,1,2,3,4,4,5,6]
 *
 * Constraints: 0 <= k <= 10^4, 0 <= nodes per list <= 500,
 * -10^4 <= Node.val <= 10^4.
 */

#include <algorithm>
#include <iostream>
#include <queue>
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

    return dummy->next;
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

// Brute force: collect every value, sort, then rebuild the list.
// O(N log N) time and O(N) space, where N is the total number of nodes.
ListNode *mergeKListsBrute(vector<ListNode *> &lists) {
    vector<int> values;

    for (ListNode *head : lists) {
        for (ListNode *current = head; current != NULL;
             current = current->next) {
            values.push_back(current->val);
        }
    }

    if (values.empty()) {
        return NULL;
    }

    sort(values.begin(), values.end());

    return buildList(values);
}

// Min-heap comparator: the node with the smallest value has the highest
// priority.
class CompareNode {
   public:
    bool operator()(ListNode *a, ListNode *b) {
        return a->val > b->val;
    }
};

// Optimized: keep one node per list in a min-heap -> O(N log k), O(k) space
ListNode *mergeKLists(vector<ListNode *> &lists) {
    priority_queue<ListNode *, vector<ListNode *>, CompareNode> minHeap;

    for (ListNode *head : lists) {
        if (head != NULL) {
            minHeap.push(head);
        }
    }

    ListNode *dummy = new ListNode(0);
    ListNode *tail = dummy;

    while (!minHeap.empty()) {
        ListNode *smallest = minHeap.top();
        minHeap.pop();

        tail->next = smallest;
        tail = tail->next;

        if (smallest->next != NULL) {
            minHeap.push(smallest->next);
        }
    }

    tail->next = NULL;  // the last node may still point into its old list

    return dummy->next;
}

int main() {
    vector<ListNode *> lists = {buildList({1, 4, 5}), buildList({1, 3, 4}),
                                buildList({2, 6})};

    ListNode *brute = mergeKListsBrute(lists);
    printList(brute);  // 1 -> 1 -> 2 -> 3 -> 4 -> 4 -> 5 -> 6

    vector<ListNode *> listsCopy = {buildList({1, 4, 5}), buildList({1, 3, 4}),
                                    buildList({2, 6})};

    ListNode *optimized = mergeKLists(listsCopy);
    printList(optimized);  // 1 -> 1 -> 2 -> 3 -> 4 -> 4 -> 5 -> 6

    vector<ListNode *> empty;
    cout << (mergeKLists(empty) == NULL) << endl;  // 1

    vector<ListNode *> withEmpty = {NULL, buildList({1}), NULL};
    printList(mergeKLists(withEmpty));  // 1

    vector<ListNode *> one = {buildList({5, 7, 9})};
    printList(mergeKLists(one));  // 5 -> 7 -> 9

    vector<ListNode *> duplicates = {buildList({1, 1}), buildList({1, 1})};
    printList(mergeKLists(duplicates));  // 1 -> 1 -> 1 -> 1

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(N log N) for sorting all N nodes
 *   min-heap -> O(N log k), because every pop/push costs O(log k)
 * Space complexity:
 *   brute -> O(N) for the values and the new list
 *   min-heap -> O(k) for the heap, plus the output list
 *
 * Special cases: k = 0 (empty result), every list empty, some lists empty,
 * one list only, duplicate values, negative values, and a very large k with
 * short lists (the heap still only holds k nodes).
 *
 * Alternatives: divide and conquer (merge pairs of lists, O(N log k)), or
 * merging one list at a time (O(N * k), too slow for large k).
 */

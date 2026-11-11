/**
 * Linear Search on a Linked List and a 2-D Array
 *
 * A linked list has no random access, so search walks the nodes one by one.
 * A 2-D array can be scanned row by row. When every row and every column is
 * sorted, a staircase search from the top-right corner eliminates one row or
 * one column per step.
 *
 * Example:
 *   list  3 -> 8 -> 12 -> 5 -> 7
 *   search 12 -> index 2, search 6 -> -1
 *
 *   matrix = {{1, 2, 3},
 *             {4, 5, 6},
 *             {7, 8, 9}}
 *   search 6 -> (1, 2), search 10 -> (-1, -1)
 *
 * Time complexity:
 *   linked list       -> O(n)
 *   2-D plain scan    -> O(rows * cols)
 *   staircase (sorted)-> O(rows + cols)
 * Space complexity -> O(1)
 */

#include <iostream>
#include <utility>
#include <vector>
using namespace std;

struct ListNode {
    int value;
    ListNode *next;

    ListNode(int v) {
        value = v;
        next = NULL;
    }
};

void append(ListNode *&head, ListNode *&tail, int value) {
    ListNode *node = new ListNode(value);

    if (head == NULL) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
}

int searchList(ListNode *head, int target) {
    int index = 0;

    for (ListNode *current = head; current != NULL; current = current->next) {
        if (current->value == target) {
            return index;
        }
        index++;
    }

    return -1;
}

pair<int, int> search2D(vector<vector<int>> &matrix, int target) {
    for (int row = 0; row < (int)matrix.size(); row++) {
        for (int col = 0; col < (int)matrix[row].size(); col++) {
            if (matrix[row][col] == target) {
                return {row, col};
            }
        }
    }

    return {-1, -1};
}

// Rows and columns are sorted in increasing order.
pair<int, int> staircaseSearch(vector<vector<int>> &matrix, int target) {
    if (matrix.empty() || matrix[0].empty()) {
        return {-1, -1};
    }

    int row = 0;
    int col = (int)matrix[0].size() - 1;

    while (row < (int)matrix.size() && col >= 0) {
        if (matrix[row][col] == target) {
            return {row, col};
        }

        if (matrix[row][col] > target) {
            col--;  // discard this column
        } else {
            row++;  // discard this row
        }
    }

    return {-1, -1};
}

void freeList(ListNode *head) {
    while (head != NULL) {
        ListNode *next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    ListNode *head = NULL;
    ListNode *tail = NULL;

    int values[] = {3, 8, 12, 5, 7};
    for (int value : values) {
        append(head, tail, value);
    }

    cout << searchList(head, 12) << endl;  // 2
    cout << searchList(head, 6) << endl;   // -1

    vector<vector<int>> matrix = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

    pair<int, int> plain = search2D(matrix, 6);
    cout << plain.first << " " << plain.second << endl;  // 1 2

    pair<int, int> missing = search2D(matrix, 10);
    cout << missing.first << " " << missing.second << endl;  // -1 -1

    pair<int, int> fast = staircaseSearch(matrix, 6);
    cout << fast.first << " " << fast.second << endl;  // 1 2

    pair<int, int> fastMissing = staircaseSearch(matrix, 10);
    cout << fastMissing.first << " " << fastMissing.second << endl;  // -1 -1

    freeList(head);

    return 0;
}

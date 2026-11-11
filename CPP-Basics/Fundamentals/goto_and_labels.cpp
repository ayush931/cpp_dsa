// goto and labels
//
// A label names a statement inside a function:
//
//   label_name:
//     statement;
//
// `goto label_name;` transfers control to that label. The label must be in the
// same function; goto cannot jump into another function.
//
// Two useful cases:
//   1. leaving several nested loops at once
//   2. jumping forward to a common cleanup block in C-style code
//
// Everything else is usually clearer with loops, break, continue, and return.
// In particular, a backward goto can imitate a loop, but a real loop states
// the intent and keeps the loop variable scoped.
//
// Example:
//   int i = 0;
// loop:
//   if (i < 3) { cout << i; i++; goto loop; }
//
// prints 0 1 2.
//
// Special cases:
//   - a label has function scope, so two labels in one function cannot share
//     a name
//   - goto cannot jump over the initialization of a variable into the part of
//     the block where that variable is in scope
//   - goto out of a block destroys the automatic objects of that block
//   - goto is not an exception mechanism; destructors still run for objects
//     whose scope is left
//
// Complexity: goto itself is O(1) and adds no memory.

#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 1. Forward jump: skip a statement.
    cout << "start" << endl;
    goto afterSkip;
    cout << "this line is skipped" << endl;
afterSkip:
    cout << "after skip" << endl;

    // 2. Backward jump used as a loop. A while loop is the better choice.
    int i = 0;
repeat:
    if (i < 3) {
        cout << i << " ";
        i++;
        goto repeat;
    }
    cout << endl;

    // 3. Search a grid and leave both loops as soon as the target is found.
    int grid[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};

    int target = 7;
    int foundRow = -1;
    int foundColumn = -1;

    for (int row = 0; row < 3; row++) {
        for (int column = 0; column < 4; column++) {
            if (grid[row][column] == target) {
                foundRow = row;
                foundColumn = column;
                goto searchDone;
            }
        }
    }

searchDone:
    if (foundRow != -1) {
        cout << "found " << target << " at " << foundRow << "," << foundColumn
             << endl;
    } else {
        cout << "not found" << endl;
    }

    // 4. The same exit without goto uses a flag in the loop condition.
    bool found = false;

    for (int row = 0; row < 3 && !found; row++) {
        for (int column = 0; column < 4 && !found; column++) {
            if (grid[row][column] == 11) {
                foundRow = row;
                foundColumn = column;
                found = true;
            }
        }
    }

    cout << "flag search: " << foundRow << "," << foundColumn << endl;

    // 5. A goto that leaves a block runs the destructors of that block.
    {
        vector<int> temporary = {1, 2, 3};
        cout << "temporary size: " << temporary.size() << endl;
        goto afterBlock;
    }
afterBlock:
    cout << "temporary destroyed" << endl;

    return 0;
}

// Output:
// start
// after skip
// 0 1 2
// found 7 at 1,2
// flag search: 2,2
// temporary size: 3
// temporary destroyed
//
// Prefer break/continue for one loop, a flag or a helper function for nested
// loops, and goto only when it makes cleanup or a multi-level exit clearly
// simpler. Modern C++ code rarely needs goto.

// Tower of Hanoi
//
// Move n disks from the source rod to the destination rod. Only one disk can
// move at a time, a larger disk may never sit on a smaller one, and the helper
// rod can be used as temporary storage.
//
// Example for n = 3 (source A, helper B, destination C):
//   A->C, A->B, C->B, A->C, B->A, B->C, A->C
// The minimum number of moves is 2^n - 1.
//
// Recurrence:
//   T(n) = 2 * T(n - 1) + 1, T(0) = 0
//   T(n) = 2^n - 1

#include <iostream>
#include <string>
#include <vector>

void hanoi(int n, char source, char helper, char destination,
           std::vector<std::string> &moves) {
    // base case: no disk left to move
    if (n == 0) {
        return;
    }

    // move the top n - 1 disks out of the way
    hanoi(n - 1, source, destination, helper, moves);

    // move the largest remaining disk
    moves.push_back(std::string(1, source) + "->" + destination);

    // move the n - 1 disks onto the destination
    hanoi(n - 1, helper, source, destination, moves);
}

long long minimumMoves(int n) {
    return (1LL << n) - 1;
}

int main() {
    std::vector<std::string> moves;
    hanoi(3, 'A', 'B', 'C', moves);

    std::cout << "moves: " << moves.size() << "\n";  // 7
    for (int i = 0; i < (int)moves.size(); i++) {
        std::cout << moves[i] << " ";
    }
    std::cout << "\n";  // A->C A->B C->B A->C B->A B->C A->C

    std::cout << minimumMoves(3) << "\n";   // 7
    std::cout << minimumMoves(0) << "\n";   // 0
    std::cout << minimumMoves(10) << "\n";  // 1023

    // n = 0 is a valid input: no moves are needed.
    std::vector<std::string> empty;
    hanoi(0, 'A', 'B', 'C', empty);
    std::cout << empty.size() << "\n";  // 0

    return 0;
}

// Output:
// moves: 7
// A->C A->B C->B A->C B->A B->C A->C
// 7
// 0
// 1023
// 0
//
// Time complexity: O(2^n), because the recursion tree has 2^n - 1 moves.
// Space complexity: O(n) recursion stack, plus O(2^n) if all moves are stored.
//
// Edge cases: n = 0 (nothing to do), n = 1 (one move), and large n (the number
// of moves grows exponentially even though the stack depth stays O(n)).

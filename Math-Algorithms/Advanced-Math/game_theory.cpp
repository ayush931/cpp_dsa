/**
 * Combinatorial Game Theory: Nim and Sprague-Grundy
 *
 * Impartial games have two players, perfect information, no chance, and the
 * same moves available to both players. The player who cannot move loses
 * (normal play).
 *
 * Nim
 *   - several piles of stones
 *   - a move removes any positive number of stones from exactly one pile
 *   - the position is losing exactly when the XOR of all pile sizes is 0
 *   - if the XOR is x != 0, a winning move reduces a pile p to p ^ x, which
 *     must be smaller than p for at least one pile
 *
 * Sprague-Grundy
 *   - every impartial game position has a Grundy number
 *   - grundy(position) = mex of the Grundy numbers of all reachable positions
 *   - mex is the smallest non-negative integer not in the set
 *   - a sum of independent games is losing exactly when the XOR of the Grundy
 *     numbers is 0
 *   - Nim is the special case where a pile of size n has Grundy number n
 *
 * This file implements:
 *   1. Nim winner and a concrete winning move
 *   2. Grundy numbers for a subtraction game (remove 1, 3, or 4 stones)
 *   3. A brute-force solver for a single pile, used to verify the Grundy table
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int nimSum(const vector<int> &piles) {
    int result = 0;

    for (int pile : piles) {
        result ^= pile;
    }

    return result;
}

bool firstPlayerWinsNim(const vector<int> &piles) {
    return nimSum(piles) != 0;
}

// Returns {pileIndex, newSize}, or {-1, -1} if the position is losing.
pair<int, int> winningNimMove(const vector<int> &piles) {
    int target = nimSum(piles);

    if (target == 0) {
        return {-1, -1};
    }

    for (int i = 0; i < piles.size(); i++) {
        int newSize = piles[i] ^ target;

        if (newSize < piles[i]) {
            return {i, newSize};
        }
    }

    return {-1, -1};
}

// Grundy numbers for a subtraction game: a move removes one of `moves`.
vector<int> grundyTable(int maximum, const vector<int> &moves) {
    vector<int> grundy(maximum + 1, 0);

    for (int stones = 1; stones <= maximum; stones++) {
        vector<bool> reachable(maximum + 2, false);

        for (int move : moves) {
            if (move > 0 && move <= stones) {
                reachable[grundy[stones - move]] = true;
            }
        }

        int mex = 0;

        while (reachable[mex]) {
            mex++;
        }

        grundy[stones] = mex;
    }

    return grundy;
}

// XOR of the Grundy numbers of several independent piles.
int grundyGame(const vector<int> &piles, const vector<int> &moves) {
    int maximum = 0;

    for (int pile : piles) {
        maximum = max(maximum, pile);
    }

    vector<int> grundy = grundyTable(maximum, moves);
    int result = 0;

    for (int pile : piles) {
        result ^= grundy[pile];
    }

    return result;
}

// Brute force for one pile: true if the player to move can force a win.
bool singlePileWins(int stones, const vector<int> &moves, vector<int> &memo) {
    if (stones == 0) {
        return false;
    }
    if (memo[stones] != -1) {
        return memo[stones] == 1;
    }

    for (int move : moves) {
        if (move > 0 && move <= stones &&
            !singlePileWins(stones - move, moves, memo)) {
            memo[stones] = 1;
            return true;
        }
    }

    memo[stones] = 0;
    return false;
}

void print(const vector<int> &values) {
    cout << "[";

    for (int i = 0; i < values.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << values[i];
    }

    cout << "]" << endl;
}

int main() {
    vector<int> first = {1, 2, 3};
    vector<int> second = {3, 4, 5};
    vector<int> zeros = {0, 0, 0};
    vector<int> single = {7};

    cout << nimSum(first) << " " << firstPlayerWinsNim(first) << endl;
    // 0 0 (the second player wins)

    cout << nimSum(second) << " " << firstPlayerWinsNim(second) << endl;
    // 2 1

    pair<int, int> move = winningNimMove(second);
    cout << move.first << " " << move.second << endl;
    // 0 1 (reduce pile 3 to 1: 1 xor 4 xor 5 = 0)

    cout << nimSum(zeros) << " " << firstPlayerWinsNim(zeros) << endl;
    // 0 0

    cout << nimSum(single) << " " << firstPlayerWinsNim(single) << endl;
    // 7 1

    pair<int, int> singleMove = winningNimMove(single);
    cout << singleMove.first << " " << singleMove.second << endl;
    // 0 0 (take the whole pile)

    // Subtraction game: remove 1, 3, or 4 stones.
    vector<int> moves = {1, 3, 4};
    vector<int> grundy = grundyTable(10, moves);

    print(grundy);
    // [0, 1, 0, 1, 2, 3, 2, 0, 1, 0, 1]

    // Verify the Grundy table against a brute-force win/lose solver.
    vector<int> memo(11, -1);
    bool matches = true;

    for (int stones = 0; stones <= 10; stones++) {
        bool bruteWins = singlePileWins(stones, moves, memo);
        bool grundyWins = grundy[stones] != 0;

        if (bruteWins != grundyWins) {
            matches = false;
        }
    }

    cout << matches << endl;  // 1

    vector<int> firstGame = {5, 6};
    vector<int> secondGame = {2, 7};
    vector<int> thirdGame = {4};

    cout << grundyGame(firstGame, moves) << endl;   // 1 (3 xor 2)
    cout << grundyGame(secondGame, moves) << endl;  // 0 (0 xor 0)
    cout << grundyGame(thirdGame, moves) << endl;   // 2

    // A single pile of 4 has Grundy 2. Its legal moves lead to:
    //   remove 1 -> 3, grundy(3) = 1
    //   remove 3 -> 1, grundy(1) = 1
    //   remove 4 -> 0, grundy(0) = 0
    // The mex of {1, 1, 0} is 2, and the move to Grundy 0 is taking all 4.
    cout << grundy[4] << endl;  // 2

    return 0;
}

/**
 * Time complexity:
 *   Nim -> O(n) to compute the XOR, O(n) to find a winning move
 *   Grundy table -> O(maximum * number of moves) for the reachable marks,
 *   plus the mex scan, which is O(maximum) per position in the worst case
 *   (the Grundy number is at most the number of moves)
 *   Grundy game -> O(maximum * moves + number of piles)
 * Space complexity -> O(maximum) for the Grundy table and O(maximum) for the
 * brute-force memo
 *
 * Special cases:
 *   - all piles empty: XOR 0, the player to move loses
 *   - one non-empty pile: the first player wins by taking the whole pile
 *   - a losing Nim position: no winning move exists
 *   - duplicate piles: they cancel in the XOR, which is why two equal piles
 *     are a losing position
 *   - a move larger than the current pile is ignored
 *   - a non-positive move is ignored; a legal move removes at least one stone
 *   - a game with no legal moves: Grundy 0, losing
 *   - the Grundy sequence of a finite subtraction game is eventually periodic;
 *     the period and preperiod depend on the move set
 *
 * The mex definition is the key to Sprague-Grundy. If a position has a move to
 * a position of Grundy 0, its Grundy number is not 0. If it has moves to every
 * Grundy number below k but not k, its Grundy number is k. The XOR rule then
 * combines independent games without enumerating the full game tree.
 *
 * Misere play (the player who takes the last object loses) is different. Nim
 * has a separate rule: if every pile has size 1, the first player wins when the
 * number of piles is even; otherwise the normal-play XOR rule applies.
 */

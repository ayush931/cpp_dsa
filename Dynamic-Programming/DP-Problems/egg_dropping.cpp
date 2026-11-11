/**
 * Egg Dropping
 *
 * You have "eggs" identical eggs and a building with "floors" floors. There is
 * a highest safe floor: an egg dropped from a safe floor does not break, and
 * an egg dropped from a higher floor breaks. Return the minimum number of
 * drops needed in the worst case to find that floor.
 *
 * Examples:
 *   eggs = 2, floors = 10 -> 4
 *   eggs = 1, floors = 10 -> 10
 *   eggs = 3, floors = 14 -> 4
 *   eggs = 2, floors = 1  -> 1
 *   eggs = 2, floors = 0  -> 0
 *
 * Recurrence:
 *   dp[e][f] = 1 + min over x in [1..f] of
 *              max(dp[e - 1][x - 1], dp[e][f - x])
 * The first term is the broken case, the second is the safe case, and the
 * max is the worst case.
 *
 * The O(eggs * floors) "moves" DP is also shown: with m moves and e eggs, the
 * maximum number of floors that can be handled is
 *   covered[e] = covered[e] + covered[e - 1] + 1.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

const long long INF = 1e18;

// Memoized recursion. Extra eggs beyond the number of floors do not help, so
// they are capped.
// O(eggs * floors^2) time, O(eggs * floors) space.
long long eggDropMemo(int eggs, int floors, vector<vector<long long>> &dp) {
    if (floors == 0) {
        return 0;
    }
    if (eggs == 0) {
        return INF;
    }
    if (eggs == 1) {
        return floors;
    }
    if (dp[eggs][floors] != -1) {
        return dp[eggs][floors];
    }

    long long best = INF;

    for (int x = 1; x <= floors; x++) {
        long long broken = eggDropMemo(eggs - 1, x - 1, dp);
        long long safe = eggDropMemo(eggs, floors - x, dp);
        best = min(best, 1 + max(broken, safe));
    }

    return dp[eggs][floors] = best;
}

// Bottom-up version of the same recurrence.
// O(eggs * floors^2) time, O(eggs * floors) space.
long long eggDropDP(int eggs, int floors) {
    if (floors == 0) {
        return 0;
    }
    if (eggs == 0) {
        return INF;
    }

    eggs = min(eggs, floors);  // more eggs than floors cannot help

    vector<vector<long long>> dp(eggs + 1, vector<long long>(floors + 1, 0));

    for (int e = 1; e <= eggs; e++) {
        for (int f = 1; f <= floors; f++) {
            if (e == 1) {
                dp[e][f] = f;
                continue;
            }

            dp[e][f] = INF;

            for (int x = 1; x <= f; x++) {
                long long worst = 1 + max(dp[e - 1][x - 1], dp[e][f - x]);
                dp[e][f] = min(dp[e][f], worst);
            }
        }
    }

    return dp[eggs][floors];
}

// O(eggs * answer) time, O(eggs) space. The answer is at most floors, so this
// is at most O(eggs * floors) and usually much faster.
int eggDropMoves(int eggs, int floors) {
    if (floors == 0) {
        return 0;
    }
    if (eggs == 0) {
        return -1;
    }

    vector<int> covered(eggs + 1, 0);
    int moves = 0;

    while (covered[eggs] < floors) {
        moves++;

        // Descending order keeps covered[e - 1] from the previous move.
        for (int e = eggs; e >= 1; e--) {
            covered[e] = covered[e] + covered[e - 1] + 1;
        }
    }

    return moves;
}

int main() {
    int eggs = 2;
    int floors = 10;

    vector<vector<long long>> memo(eggs + 1, vector<long long>(floors + 1, -1));

    cout << eggDropMemo(eggs, floors, memo) << " " << eggDropDP(eggs, floors)
         << " " << eggDropMoves(eggs, floors) << endl;
    // 4 4 4

    cout << eggDropDP(1, 10) << " " << eggDropMoves(1, 10) << endl;
    // 10 10

    cout << eggDropDP(3, 14) << " " << eggDropMoves(3, 14) << endl;
    // 4 4

    cout << eggDropMoves(2, 1) << endl;  // 1
    cout << eggDropMoves(2, 0) << endl;  // 0
    cout << eggDropMoves(0, 5) << endl;  // -1 (no egg, impossible)

    return 0;
}

/**
 * Complexity summary:
 *   memo / bottom-up -> O(eggs * floors^2) time, O(eggs * floors) space
 *   moves DP         -> O(eggs * answer) time, O(eggs) space
 *
 * Edge cases: zero floors (0 drops), one egg (linear search, floors drops),
 * zero eggs with floors > 0 (impossible), one floor, and more eggs than
 * floors.
 *
 * Why the moves DP works: with m moves, an egg can either break (then e - 1
 * eggs remain for m - 1 moves) or survive (e eggs remain for m - 1 moves).
 * The largest number of floors distinguishable with m moves is therefore
 * covered[e][m] = covered[e][m - 1] + covered[e - 1][m - 1] + 1.
 */

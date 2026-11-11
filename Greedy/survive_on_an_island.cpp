/**
 * Survive on an Island
 *
 * You are on an island with one shop. The shop is open every day except
 * Sunday. You need to survive for S days.
 *   - N: maximum units of food you can buy per day
 *   - M: units of food you need each day
 *
 * Find the minimum number of days on which you must buy food, or -1 if it is
 * impossible to survive.
 *
 * Example:
 * Input:  S = 10, N = 16, M = 2
 * Output: 2
 *
 * Input:  S = 10, N = 16, M = 3
 * Output: 2  (buy 16 units on day 1 and day 6 -> 32 >= 30)
 *
 * Constraints: 1 <= S, N, M <= 10^4.
 */

#include <iostream>
#include <vector>
using namespace std;

const int INF = 1e9;

// Brute force: try buying and not buying on every open day.
// Exponential without memoization; fine for small S.
int surviveBrute(int day, int food, int S, int N, int M) {
    if (day > S) {
        return 0;
    }

    // Sunday: the shop is closed.
    if (day % 7 == 0) {
        if (food < M) {
            return INF;
        }
        return surviveBrute(day + 1, food - M, S, N, M);
    }

    int best = INF;

    // Do not buy today.
    if (food >= M) {
        best = min(best, surviveBrute(day + 1, food - M, S, N, M));
    }

    // Buy N units today (if that is enough to eat).
    if (food + N >= M) {
        int result = surviveBrute(day + 1, food + N - M, S, N, M);

        if (result != INF) {
            best = min(best, result + 1);
        }
    }

    return best;
}

// Greedy: buy only when the current stock cannot cover today.
int survive(int S, int N, int M) {
    if (M > N) {
        return -1;  // cannot buy enough for even one day
    }

    // A full week needs 7 * M units, but the shop is open only 6 days.
    if (S > 6 && 6 * N < 7 * M) {
        return -1;
    }

    int buyingDays = 0;
    int food = 0;

    for (int day = 1; day <= S; day++) {
        if (day % 7 == 0) {
            // Sunday: no purchase, just eat.
            if (food < M) {
                return -1;
            }
            food -= M;
        } else {
            if (food < M) {
                food += N;
                buyingDays++;
            }

            food -= M;
        }
    }

    return buyingDays;
}

int main() {
    cout << surviveBrute(1, 0, 10, 16, 2) << " " << survive(10, 16, 2) << endl;
    // 2 2

    cout << survive(10, 16, 3) << endl;  // 2
    cout << survive(10, 16, 4) << endl;  // 3
    cout << survive(5, 10, 2) << endl;   // 1
    cout << survive(1, 1, 1) << endl;    // 1
    cout << survive(7, 10, 2) << endl;   // 2

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(2^S) without memoization, O(S * food) with memoization
 *   greedy -> O(S) time, O(1) space
 *
 * Special cases: M > N (impossible), a full week where 6 * N < 7 * M
 * (impossible), S = 1, S = 7 (one Sunday), buying exactly enough for Sunday,
 * and a large S where the greedy is required.
 *
 * The greedy is optimal because buying earlier only helps: food does not spoil,
 * and every open day can add at most N units.
 */

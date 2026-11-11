/**
 * Algorithm design paradigms
 *
 * Brute force -> try every possibility, always correct, usually exponential
 * Divide and conquer -> divide, solve recursively, combine
 * Greedy -> make the locally best choice, correct only with a proof
 * Dynamic programming -> optimal substructure + overlapping subproblems
 * Backtracking -> explore choices and undo them, prune invalid states
 * Branch and bound -> backtracking with a bound to discard bad branches
 * Randomized -> use randomness, Las Vegas or Monte Carlo
 *
 * How to choose:-
 * Small n and no structure -> brute force
 * Independent subproblems -> divide and conquer
 * Greedy choice property -> greedy
 * Overlapping subproblems -> DP
 * Need all solutions or constraints -> backtracking
 * Optimization with a good bound -> branch and bound
 * Huge search space with acceptable error -> randomized
 */

#include <iostream>
#include <string>
using namespace std;

string chooseParadigm(bool optimalSubstructure, bool overlappingSubproblems,
                      bool greedyChoice, bool needAllSolutions) {
    if (needAllSolutions) {
        return "Backtracking";
    }
    if (overlappingSubproblems && optimalSubstructure) {
        return "Dynamic Programming";
    }
    if (greedyChoice && optimalSubstructure) {
        return "Greedy";
    }
    if (optimalSubstructure) {
        return "Divide and Conquer";
    }
    return "Brute Force";
};

int main() {
    cout << chooseParadigm(true, true, false, false) << endl;  // DP
    cout << chooseParadigm(true, false, true, false) << endl;  // Greedy
    cout << chooseParadigm(true, false, false, false)
         << endl;  // Divide and Conquer
    cout << chooseParadigm(true, true, false, true) << endl;  // Backtracking

    return 0;
}

// Greedy needs an exchange argument or matroid proof
// DP needs a correct state, transition and base case
// Backtracking needs a way to mark and unmark choices

/**
 * 2-SAT (2-Satisfiability)
 *
 * A 2-CNF formula is a conjunction of clauses, where each clause is a
 * disjunction of at most two literals:
 *
 *   (x0 OR x1) AND (NOT x0 OR x2) AND (NOT x2 OR NOT x1) ...
 *
 * Each clause (a OR b) is equivalent to two implications:
 *   NOT a -> b
 *   NOT b -> a
 *
 * Build an implication graph with two nodes per variable:
 *   node 2*i     -> variable i is false
 *   node 2*i + 1 -> variable i is true
 *
 * The formula is satisfiable exactly when no variable and its negation are in
 * the same strongly connected component. If x and NOT x are mutually
 * reachable, assuming either value forces the opposite one, which is a
 * contradiction.
 *
 * Algorithm (Kosaraju):
 *   1. DFS on the implication graph, record finish order
 *   2. DFS on the reversed graph in reverse finish order -> SCCs
 *   3. component[true] > component[false] -> set the variable true
 *
 * Complexity -> O(V + E) = O(n + m) for n variables and m clauses.
 */

#include <algorithm>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

class TwoSat {
    int variables;
    vector<vector<int>> graph;
    vector<vector<int>> reverseGraph;
    vector<bool> visited;
    vector<int> finishOrder;
    vector<int> component;

    int literal(int variable, bool isTrue) {
        return 2 * variable + (isTrue ? 1 : 0);
    }

    int negate(int node) {
        return node ^ 1;  // flips the lowest bit: true <-> false
    }

    void dfsFirst(int node) {
        visited[node] = true;

        for (int next : graph[node]) {
            if (!visited[next]) {
                dfsFirst(next);
            }
        }

        finishOrder.push_back(node);
    }

    void dfsSecond(int node, int componentId) {
        component[node] = componentId;

        for (int next : reverseGraph[node]) {
            if (component[next] == -1) {
                dfsSecond(next, componentId);
            }
        }
    }

   public:
    explicit TwoSat(int n) {
        variables = n;
        graph.resize(2 * n);
        reverseGraph.resize(2 * n);
        visited.assign(2 * n, false);
        component.assign(2 * n, -1);
    }

    // Adds the clause (a == aTrue) OR (b == bTrue).
    void addClause(int a, bool aTrue, int b, bool bTrue) {
        int first = literal(a, aTrue);
        int second = literal(b, bTrue);

        // NOT first -> second
        graph[negate(first)].push_back(second);
        reverseGraph[second].push_back(negate(first));

        // NOT second -> first
        graph[negate(second)].push_back(first);
        reverseGraph[first].push_back(negate(second));
    }

    // Adds the unit clause (a == aTrue).
    void addUnit(int a, bool aTrue) {
        addClause(a, aTrue, a, aTrue);
    }

    bool satisfiable() {
        fill(visited.begin(), visited.end(), false);
        fill(component.begin(), component.end(), -1);
        finishOrder.clear();

        for (int node = 0; node < 2 * variables; node++) {
            if (!visited[node]) {
                dfsFirst(node);
            }
        }

        int componentId = 0;

        for (int index = (int)finishOrder.size() - 1; index >= 0; index--) {
            int node = finishOrder[index];

            if (component[node] == -1) {
                dfsSecond(node, componentId++);
            }
        }

        for (int variable = 0; variable < variables; variable++) {
            if (component[literal(variable, false)] ==
                component[literal(variable, true)]) {
                return false;
            }
        }

        return true;
    }

    // Only valid when satisfiable() returned true.
    vector<bool> assignment() {
        vector<bool> value(variables, false);

        for (int variable = 0; variable < variables; variable++) {
            value[variable] = component[literal(variable, true)] >
                              component[literal(variable, false)];
        }

        return value;
    }
};

struct Clause {
    int a;
    bool aTrue;
    int b;
    bool bTrue;
};

// Brute force: try all 2^n assignments. Only for small n.
bool satisfiableBrute(int variables, vector<Clause> &clauses) {
    for (int mask = 0; mask < (1 << variables); mask++) {
        bool valid = true;

        for (Clause &clause : clauses) {
            bool first = ((mask >> clause.a) & 1) == (clause.aTrue ? 1 : 0);
            bool second = ((mask >> clause.b) & 1) == (clause.bTrue ? 1 : 0);

            if (!first && !second) {
                valid = false;
                break;
            }
        }

        if (valid) {
            return true;
        }
    }

    return false;
}

bool verifyAssignment(int variables, vector<Clause> &clauses,
                      vector<bool> &value) {
    if (value.size() != variables) {
        return false;
    }

    for (Clause &clause : clauses) {
        bool first = value[clause.a] == clause.aTrue;
        bool second = value[clause.b] == clause.bTrue;

        if (!first && !second) {
            return false;
        }
    }

    return true;
}

void print(const vector<bool> &value) {
    cout << "[";

    for (int i = 0; i < value.size(); i++) {
        if (i > 0) {
            cout << ",";
        }
        cout << value[i];
    }

    cout << "]" << endl;
}

int main() {
    // x0 and x1 must both be true:
    //   (x0 OR x1) AND (NOT x0 OR x1) AND (x0 OR NOT x1)
    vector<Clause> forcing = {
        {0, true, 1, true}, {0, false, 1, true}, {0, true, 1, false}};

    TwoSat first(2);
    for (Clause &clause : forcing) {
        first.addClause(clause.a, clause.aTrue, clause.b, clause.bTrue);
    }

    cout << first.satisfiable() << endl;  // 1
    vector<bool> firstValue = first.assignment();
    print(firstValue);                                         // [1,1]
    cout << verifyAssignment(2, forcing, firstValue) << endl;  // 1

    // Contradiction: (x0) AND (NOT x0).
    TwoSat second(1);
    second.addUnit(0, true);
    second.addUnit(0, false);
    cout << second.satisfiable() << endl;  // 0

    // A cycle of implications: (NOT x0 OR x1), (NOT x1 OR x2),
    // (NOT x2 OR x0). All false and all true both work.
    vector<Clause> cycle = {
        {0, false, 1, true}, {1, false, 2, true}, {2, false, 0, true}};

    TwoSat third(3);
    for (Clause &clause : cycle) {
        third.addClause(clause.a, clause.aTrue, clause.b, clause.bTrue);
    }

    cout << third.satisfiable() << endl;  // 1
    vector<bool> thirdValue = third.assignment();
    print(thirdValue);
    cout << verifyAssignment(3, cycle, thirdValue) << endl;  // 1

    // A larger formula, checked against brute force.
    vector<Clause> mixed = {{0, true, 1, true},
                            {0, false, 2, true},
                            {1, false, 2, false},
                            {0, true, 2, false},
                            {1, true, 2, true}};

    TwoSat fourth(3);
    for (Clause &clause : mixed) {
        fourth.addClause(clause.a, clause.aTrue, clause.b, clause.bTrue);
    }

    cout << satisfiableBrute(3, mixed) << " " << fourth.satisfiable() << endl;
    // 1 1

    vector<bool> fourthValue = fourth.assignment();
    print(fourthValue);
    cout << verifyAssignment(3, mixed, fourthValue) << endl;  // 1

    // Unsatisfiable: (x0 OR x0), (NOT x0 OR NOT x0), (x0 OR NOT x1),
    // (NOT x0 OR x1).
    vector<Clause> impossible = {{0, true, 0, true},
                                 {0, false, 0, false},
                                 {0, true, 1, false},
                                 {0, false, 1, true}};

    TwoSat fifth(2);
    for (Clause &clause : impossible) {
        fifth.addClause(clause.a, clause.aTrue, clause.b, clause.bTrue);
    }

    cout << satisfiableBrute(2, impossible) << " " << fifth.satisfiable()
         << endl;
    // 0 0

    // No variables and no clauses: trivially satisfiable.
    TwoSat empty(0);
    cout << empty.satisfiable() << endl;  // 1

    return 0;
}

/**
 * Time complexity -> O(V + E), two DFS passes over 2n nodes and the
 * implications of m clauses (at most 2m edges)
 * Space complexity -> O(V + E) for the graph, the reversed graph, and the
 * component array
 *
 * Special cases:
 *   - a unit clause (x) is (x OR x)
 *   - a clause with both literals equal, such as (x OR x), still works
 *   - a variable that never appears is free; either value works
 *   - zero variables and zero clauses is satisfiable
 *   - a formula with a variable and its negation forced true is unsatisfiable
 *   - multiple edges and duplicate clauses are harmless
 *
 * Applications: dependency constraints, scheduling with mutually exclusive
 * choices, graph coloring with two colors per node, and any problem whose
 * constraints can be written as 2-CNF. 3-SAT is NP-complete, but 2-SAT is
 * linear because every clause gives only two implications.
 *
 * Why the assignment works: Kosaraju numbers components in topological order
 * of the implication graph (sources first). If component[true] is later than
 * component[false], choosing true cannot force false, because a path from true
 * to false would put false in a later component. Choosing the later component
 * for every variable therefore satisfies every implication.
 */

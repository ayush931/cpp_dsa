/**
 * A* Search
 *
 * A* is Dijkstra's algorithm with a heuristic. It expands the node with the
 * smallest f(n) = g(n) + h(n):
 *   - g(n): the exact cost from the start to n
 *   - h(n): an estimate of the cost from n to the goal
 *
 * If h is admissible (never overestimates), A* finds the optimal path. If h is
 * also consistent (h(u) <= cost(u, v) + h(v)), then a node is finalized once,
 * just like Dijkstra.
 *
 *   h = 0                  -> Dijkstra, expands in every direction
 *   h = Manhattan distance -> focuses the search toward the goal on a grid
 *
 * The heuristic must match the movement rules. For 4-directional movement with
 * unit costs, Manhattan distance is admissible. For 8-directional movement,
 * use Chebyshev distance; for weighted grids, use the minimum edge weight.
 *
 * This file compares Dijkstra and A* on the same grid and counts how many
 * nodes each one expands. In the grid, 0 is an open cell and 1 is a wall.
 */

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

struct State {
    int estimate;  // f = g + h
    int cost;      // g
    int row;
    int column;
};

struct CompareState {
    bool operator()(const State &a, const State &b) {
        if (a.estimate != b.estimate) {
            return a.estimate > b.estimate;  // min-heap on f
        }

        // Tie-break on larger g: among equal f, prefer the node closer to the
        // goal. Without this, A* may expand every node whose f equals the
        // optimal cost, which on an open grid is most of the grid.
        return a.cost < b.cost;
    }
};

int manhattan(int row, int column, int targetRow, int targetColumn) {
    return abs(row - targetRow) + abs(column - targetColumn);
}

// Returns the shortest path cost, or -1 if the goal is unreachable.
// `expanded` counts how many nodes were finalized.
int shortestPath(vector<vector<int>> &grid, int startRow, int startColumn,
                 int targetRow, int targetColumn, bool useHeuristic,
                 int &expanded) {
    if (grid.empty() || grid[0].empty()) {
        expanded = 0;
        return -1;
    }

    int rows = grid.size();
    int columns = grid[0].size();

    if (startRow < 0 || startRow >= rows || startColumn < 0 ||
        startColumn >= columns || targetRow < 0 || targetRow >= rows ||
        targetColumn < 0 || targetColumn >= columns) {
        expanded = 0;
        return -1;
    }

    vector<vector<int>> distance(rows, vector<int>(columns, INT_MAX));
    priority_queue<State, vector<State>, CompareState> open;

    int heuristic =
        useHeuristic ? manhattan(startRow, startColumn, targetRow, targetColumn)
                     : 0;

    distance[startRow][startColumn] = 0;
    open.push({heuristic, 0, startRow, startColumn});
    expanded = 0;

    int rowChange[4] = {1, -1, 0, 0};
    int columnChange[4] = {0, 0, 1, -1};

    while (!open.empty()) {
        State current = open.top();
        open.pop();

        // A stale entry: a better path to this cell was already found.
        if (current.cost != distance[current.row][current.column]) {
            continue;
        }

        expanded++;

        if (current.row == targetRow && current.column == targetColumn) {
            return current.cost;
        }

        for (int direction = 0; direction < 4; direction++) {
            int nextRow = current.row + rowChange[direction];
            int nextColumn = current.column + columnChange[direction];

            if (nextRow < 0 || nextColumn < 0 || nextRow >= rows ||
                nextColumn >= columns) {
                continue;
            }

            if (grid[nextRow][nextColumn] == 1) {
                continue;  // wall
            }

            int nextCost = current.cost + 1;

            if (nextCost >= distance[nextRow][nextColumn]) {
                continue;
            }

            distance[nextRow][nextColumn] = nextCost;

            int estimate = nextCost;

            if (useHeuristic) {
                estimate +=
                    manhattan(nextRow, nextColumn, targetRow, targetColumn);
            }

            open.push({estimate, nextCost, nextRow, nextColumn});
        }
    }

    return -1;
}

int main() {
    vector<vector<int>> grid = {{0, 0, 0, 0, 0},
                                {0, 1, 1, 0, 0},
                                {0, 0, 0, 0, 0},
                                {0, 1, 0, 1, 0},
                                {0, 0, 0, 0, 0}};

    int dijkstraExpanded = 0;
    int astarExpanded = 0;

    int dijkstraCost = shortestPath(grid, 0, 0, 4, 4, false, dijkstraExpanded);
    int astarCost = shortestPath(grid, 0, 0, 4, 4, true, astarExpanded);

    cout << dijkstraCost << " " << astarCost << endl;  // 8 8
    cout << dijkstraExpanded << " " << astarExpanded << endl;
    // 21 9: A* expands fewer nodes on this grid because the Manhattan
    // heuristic points it toward the goal.

    // Open 5x5 grid: the Manhattan heuristic is exact.
    vector<vector<int>> openGrid(5, vector<int>(5, 0));

    int openDijkstra = 0;
    int openAstar = 0;

    cout << shortestPath(openGrid, 0, 0, 4, 4, false, openDijkstra) << " "
         << shortestPath(openGrid, 0, 0, 4, 4, true, openAstar) << endl;
    // 8 8
    cout << openDijkstra << " " << openAstar << endl;
    // 25 9

    // Unreachable goal.
    vector<vector<int>> blocked = {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}};

    int blockedExpanded = 0;
    cout << shortestPath(blocked, 0, 0, 0, 2, true, blockedExpanded) << endl;
    // -1

    // Start == goal.
    int sameExpanded = 0;
    cout << shortestPath(openGrid, 2, 2, 2, 2, true, sameExpanded) << endl;
    // 0

    return 0;
}

/**
 * Time complexity:
 *   Dijkstra -> O((V + E) log V)
 *   A* -> O((V + E) log V) worst case, often much less with a good heuristic
 * Space complexity -> O(V + E) worst case for the distance table and the open
 * set (O(V) on a bounded-degree grid)
 *
 * Special cases:
 *   - start == goal (0)
 *   - unreachable goal (-1)
 *   - a heuristic that overestimates breaks optimality
 *   - an inconsistent heuristic can require reopening nodes; the simple
 *     implementation above still works if stale entries are skipped, but it
 *     may expand more nodes
 *   - zero-cost edges are fine; negative edges are not, because A* assumes
 *     non-negative costs like Dijkstra
 *   - a wall on the start or goal should be checked by the caller
 *   - an empty grid or an out-of-range endpoint returns -1
 *
 * A* is used in pathfinding, puzzle solving, route planning, and any search
 * where a domain-specific lower bound is available. The quality of the
 * heuristic decides how much of the graph is explored.
 */

/**
 * Course Schedule II
 *
 * There are numCourses courses labelled from 0 to numCourses - 1. You are given
 * prerequisites where prerequisites[i] = [ai, bi] means you must take course bi
 * before course ai.
 *
 * Return any valid order to finish all courses. If it is impossible, return an
 * empty array.
 *
 * Example 1:
 * Input: numCourses = 2, prerequisites = [[1,0]]
 * Output: [0,1]
 *
 * Example 2:
 * Input: numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]
 * Output: [0,1,2,3] (or [0,2,1,3])
 *
 * Example 3:
 * Input: numCourses = 1, prerequisites = []
 * Output: [0]
 *
 * Constraints: 1 <= numCourses <= 2000,
 * 0 <= prerequisites.length <= numCourses * (numCourses - 1),
 * all pairs are distinct and ai != bi.
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// Approach 1: repeatedly scan for a course with no remaining prerequisites.
// O(V^2 + E) time, O(V + E) space. Simple, but slow for large inputs.
vector<int> findOrderScan(int numCourses, vector<vector<int>> &prerequisites) {
    vector<vector<int>> nextCourses(numCourses);
    vector<int> indegree(numCourses, 0);

    for (vector<int> &pair : prerequisites) {
        int course = pair[0];
        int prerequisite = pair[1];

        nextCourses[prerequisite].push_back(course);
        indegree[course]++;
    }

    vector<bool> taken(numCourses, false);
    vector<int> order;

    for (int step = 0; step < numCourses; step++) {
        int course = -1;

        for (int i = 0; i < numCourses; i++) {
            if (!taken[i] && indegree[i] == 0) {
                course = i;
                break;
            }
        }

        if (course == -1) {
            return {};  // every remaining course has a prerequisite
        }

        taken[course] = true;
        order.push_back(course);

        for (int next : nextCourses[course]) {
            indegree[next]--;
        }
    }

    return order;
}

// Approach 2: Kahn's algorithm with a queue.
// O(V + E) time, O(V + E) space.
vector<int> findOrder(int numCourses, vector<vector<int>> &prerequisites) {
    vector<vector<int>> nextCourses(numCourses);
    vector<int> indegree(numCourses, 0);

    for (vector<int> &pair : prerequisites) {
        int course = pair[0];
        int prerequisite = pair[1];

        nextCourses[prerequisite].push_back(course);
        indegree[course]++;
    }

    queue<int> ready;

    for (int i = 0; i < numCourses; i++) {
        if (indegree[i] == 0) {
            ready.push(i);
        }
    }

    vector<int> order;

    while (!ready.empty()) {
        int course = ready.front();
        ready.pop();

        order.push_back(course);

        for (int next : nextCourses[course]) {
            indegree[next]--;

            if (indegree[next] == 0) {
                ready.push(next);
            }
        }
    }

    if (order.size() != numCourses) {
        return {};  // a cycle blocks some courses
    }

    return order;
}

bool isValidOrder(int numCourses, vector<vector<int>> &prerequisites,
                  vector<int> &order) {
    if (order.size() != numCourses) {
        return false;
    }

    vector<int> position(numCourses, -1);

    for (int i = 0; i < order.size(); i++) {
        position[order[i]] = i;
    }

    for (vector<int> &pair : prerequisites) {
        if (position[pair[1]] > position[pair[0]]) {
            return false;
        }
    }

    return true;
}

void print(const vector<int> &order) {
    cout << "[";

    for (int i = 0; i < order.size(); i++) {
        if (i > 0) {
            cout << ",";
        }
        cout << order[i];
    }

    cout << "]" << endl;
}

int main() {
    vector<vector<int>> first = {{1, 0}};
    vector<vector<int>> second = {{1, 0}, {2, 0}, {3, 1}, {3, 2}};
    vector<vector<int>> cycle = {{1, 0}, {0, 1}};
    vector<vector<int>> noPrerequisites;

    vector<int> scan = findOrderScan(2, first);
    vector<int> kahn = findOrder(2, first);
    print(scan);                                   // [0,1]
    print(kahn);                                   // [0,1]
    cout << isValidOrder(2, first, kahn) << endl;  // 1

    vector<int> secondOrder = findOrder(4, second);
    print(secondOrder);                                    // [0,1,2,3]
    cout << isValidOrder(4, second, secondOrder) << endl;  // 1

    print(findOrder(2, cycle));            // [] (cycle)
    print(findOrder(1, noPrerequisites));  // [0]

    vector<vector<int>> disconnected = {{1, 0}, {3, 2}};
    vector<int> disconnectedOrder = findOrder(4, disconnected);
    print(disconnectedOrder);  // [0,1,2,3]
    cout << isValidOrder(4, disconnected, disconnectedOrder) << endl;  // 1

    return 0;
}

/**
 * Time complexity:
 *   scan -> O(V^2 + E), because finding the next zero-indegree course scans
 *   every vertex on every step
 *   queue -> O(V + E), every vertex and edge is processed once
 * Space complexity -> O(V + E) for the adjacency list and indegree array
 *
 * Special cases: no prerequisites (any order works), one course, a cycle
 * (empty result), several disconnected components, duplicate prerequisites
 * (excluded by the constraints), and a prerequisite chain that is a single
 * long path.
 *
 * The queue version is Kahn's algorithm. If it emits fewer than V courses, the
 * remaining courses are part of a cycle (or depend on one), so no order exists.
 */

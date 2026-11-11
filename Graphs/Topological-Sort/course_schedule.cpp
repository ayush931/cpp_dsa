/**
 * Course Schedule
 *
 * There are numCourses courses labelled from 0 to numCourses - 1. You are given
 * prerequisites where prerequisites[i] = [ai, bi] means you must take course bi
 * before course ai.
 *
 * Return true if you can finish all courses, otherwise false.
 *
 * Example 1:
 * Input: numCourses = 2, prerequisites = [[1,0]]
 * Output: true
 *
 * Example 2:
 * Input: numCourses = 2, prerequisites = [[1,0],[0,1]]
 * Output: false
 *
 * Constraints: 1 <= numCourses <= 2000,
 * 0 <= prerequisites.length <= 5000, all pairs are unique.
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// Approach 1: repeatedly remove courses that have no prerequisites left.
// O(V^2 + E) time, O(V + E) space.
bool canFinishScan(int numCourses, vector<vector<int>> &prerequisites) {
    vector<vector<int>> nextCourses(numCourses);
    vector<int> indegree(numCourses, 0);

    for (vector<int> &pair : prerequisites) {
        nextCourses[pair[1]].push_back(pair[0]);
        indegree[pair[0]]++;
    }

    vector<bool> removed(numCourses, false);
    int removedCount = 0;

    for (int step = 0; step < numCourses; step++) {
        int course = -1;

        for (int i = 0; i < numCourses; i++) {
            if (!removed[i] && indegree[i] == 0) {
                course = i;
                break;
            }
        }

        if (course == -1) {
            break;
        }

        removed[course] = true;
        removedCount++;

        for (int next : nextCourses[course]) {
            indegree[next]--;
        }
    }

    return removedCount == numCourses;
}

// Approach 2: Kahn's algorithm. If every course can be processed, there is no
// cycle. O(V + E) time, O(V + E) space.
bool canFinish(int numCourses, vector<vector<int>> &prerequisites) {
    vector<vector<int>> nextCourses(numCourses);
    vector<int> indegree(numCourses, 0);

    for (vector<int> &pair : prerequisites) {
        nextCourses[pair[1]].push_back(pair[0]);
        indegree[pair[0]]++;
    }

    queue<int> ready;

    for (int i = 0; i < numCourses; i++) {
        if (indegree[i] == 0) {
            ready.push(i);
        }
    }

    int processed = 0;

    while (!ready.empty()) {
        int course = ready.front();
        ready.pop();
        processed++;

        for (int next : nextCourses[course]) {
            indegree[next]--;

            if (indegree[next] == 0) {
                ready.push(next);
            }
        }
    }

    return processed == numCourses;
}

int main() {
    vector<vector<int>> first = {{1, 0}};
    vector<vector<int>> cycle = {{1, 0}, {0, 1}};
    vector<vector<int>> chain = {{1, 0}, {2, 1}, {3, 2}};
    vector<vector<int>> noPrerequisites;
    vector<vector<int>> selfLoop = {{0, 0}};
    vector<vector<int>> disconnected = {{1, 0}, {3, 2}};
    vector<vector<int>> none;

    cout << canFinishScan(2, first) << " " << canFinish(2, first) << endl;
    // 1 1

    cout << canFinishScan(2, cycle) << " " << canFinish(2, cycle) << endl;
    // 0 0

    cout << canFinish(4, chain) << endl;            // 1
    cout << canFinish(1, noPrerequisites) << endl;  // 1
    cout << canFinish(1, selfLoop) << endl;         // 0 (a course needs itself)
    cout << canFinish(4, disconnected) << endl;     // 1
    cout << canFinish(3, none) << endl;             // 1

    return 0;
}

/**
 * Time complexity:
 *   scan -> O(V^2 + E)
 *   queue -> O(V + E)
 * Space complexity -> O(V + E) for the adjacency list and indegree array
 *
 * Special cases: no prerequisites, one course, a two-course cycle, a long
 * chain, a self-loop, several disconnected components, and a cycle mixed with
 * courses that can still be finished (the answer is false because all courses
 * must be finished).
 *
 * A DFS with three colors (white, gray, black) also detects the cycle in
 * O(V + E). Kahn's algorithm is used here because it reuses the topological
 * sort machinery from the previous files.
 */

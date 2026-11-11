/**
 * Given a characters array tasks, representing the tasks a CPU needs to do,
 * where each letter represents a different task. Tasks could be done in any
 * order. Each task is done in one unit of time. For each unit of time, the CPU
 * could complete either one task or just be idle. However, there is an
 * non-negative integer n that represents the cooldown period between two same
 * tasks (the same letter in the array), that is that there must be at least n
 * unit of time between any two same tasks. Return the least number of units of
 * times that the CPU will take to finish all the given tasks.
 *
 * Input: tasks = ["A", "A", "A", "B", "B", "B"], n = 2
 * Output: 8
 *
 * Explanation: A -> B -> idle -> A -> B -> idle -> A -> B
 * There is at least 2 units of time between any two same tasks.
 */

#include <bits/stdc++.h>
using namespace std;

int leastInterval(vector<char> tasks, int n) {
    // 1. count freq of tasks
    unordered_map<char, int> taskFreq;

    for (auto t : tasks) {
        taskFreq[t]++;
    }

    // 2. Insert freq into max heap
    priority_queue<int> pq;

    for (auto e : taskFreq) {
        pq.push(e.second);
    }

    // 3. Finding time until pq is empty
    int totalTime = 0;

    while (!pq.empty()) {
        vector<int> temp;

        // logging one time frame = n + 1 units of time
        for (int i = 0; i <= n; i++) {
            if (!pq.empty()) {
                int freq = pq.top();
                pq.pop();

                if (freq > 1) {
                    temp.push_back(
                        freq - 1);  // adding remaining tasks in the temp vector
                }
            }
            totalTime++;

            if (pq.empty() && temp.empty()) {
                return totalTime;  // all tasks have been executed
            }
        }

        for (auto t : temp) {
            pq.push(t);  // adding back remaining tasks from temp vector to pq
        }
    }

    return totalTime;
};

int main() {
    int n, cooldown;
    cin >> n >> cooldown;

    vector<char> tasks(n);

    for (int i = 0; i < n; i++) {
        cin >> tasks[i];
    }

    int leastTime = leastInterval(tasks, cooldown);
    cout << "Least number of units of time: " << leastTime << endl;

    return 0;
}
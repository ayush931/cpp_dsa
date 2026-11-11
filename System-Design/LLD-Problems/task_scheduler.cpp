/**
 * Task Scheduler
 *
 * A single-threaded scheduler that runs tasks at a specified time, with an
 * optional repeat interval and a priority for tasks due at the same moment.
 *
 * Ordering:
 *   1. earliest executeAt first
 *   2. higher priority first
 *   3. lower task id first
 *
 * The heap stores task snapshots. Cancellation is lazy: the task is marked
 * cancelled in the registry and skipped when it reaches the top of the heap.
 * This avoids removing an arbitrary element from a priority queue, which is
 * O(n). A production scheduler may use an indexed heap or a balanced tree when
 * cancellation is frequent.
 *
 * Recurring tasks:
 *   next executeAt = previous executeAt + interval
 *
 * The next run is based on the scheduled time, not on the current clock, so a
 * late run does not drift the cadence. If the scheduler is far behind, it will
 * run every missed occurrence in order. A production system usually chooses a
 * policy: catch up, skip missed runs, or coalesce them.
 *
 * This model is single-threaded and uses a virtual clock. A real scheduler adds
 * a worker pool, durable storage, leases to avoid duplicate execution across
 * nodes, retries with backoff, and a dead-letter queue.
 */

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct Task {
    int id;
    string name;
    long long executeAt;
    long long interval;  // 0 for one-shot
    int priority;        // higher runs first at the same time
    bool scheduled = true;
    bool cancelled = false;
};

struct Execution {
    string name;
    long long time;
};

struct CompareTask {
    bool operator()(const Task &a, const Task &b) const {
        if (a.executeAt != b.executeAt) {
            return a.executeAt > b.executeAt;
        }

        if (a.priority != b.priority) {
            return a.priority < b.priority;
        }

        return a.id > b.id;
    }
};

class Scheduler {
    priority_queue<Task, vector<Task>, CompareTask> queue;
    map<int, Task> tasks;
    vector<Execution> history;
    long long currentTime = 0;
    int nextId = 1;
    int pendingCount = 0;

    void push(const Task &task) {
        queue.push(task);
    }

   public:
    // Returns the task id, or -1 for an invalid time.
    int schedule(const string &name, long long executeAt, int priority = 0) {
        if (executeAt < 0 || name.empty()) {
            return -1;
        }

        int id = nextId++;
        Task task{id, name, executeAt, 0, priority, true, false};

        tasks[id] = task;
        push(task);
        pendingCount++;

        return id;
    }

    // Returns the task id, or -1 for an invalid time or interval.
    int scheduleRecurring(const string &name, long long firstAt,
                          long long interval, int priority = 0) {
        if (firstAt < 0 || interval <= 0 || name.empty()) {
            return -1;
        }

        int id = nextId++;
        Task task{id, name, firstAt, interval, priority, true, false};

        tasks[id] = task;
        push(task);
        pendingCount++;

        return id;
    }

    bool cancel(int taskId) {
        auto entry = tasks.find(taskId);

        if (entry == tasks.end()) {
            return false;
        }
        if (!entry->second.scheduled) {
            return false;
        }

        entry->second.scheduled = false;
        entry->second.cancelled = true;
        pendingCount--;

        return true;
    }

    // Runs every task due at or before `time`, then advances the clock.
    void runUntil(long long time) {
        if (time < currentTime) {
            return;
        }

        while (!queue.empty() && queue.top().executeAt <= time) {
            Task task = queue.top();
            queue.pop();

            auto entry = tasks.find(task.id);

            if (entry == tasks.end()) {
                continue;
            }
            if (!entry->second.scheduled || entry->second.cancelled) {
                continue;
            }

            history.push_back({task.name, task.executeAt});

            if (task.interval > 0 &&
                task.executeAt <=
                    numeric_limits<long long>::max() - task.interval) {
                Task next = task;
                next.executeAt = task.executeAt + task.interval;
                entry->second.executeAt = next.executeAt;
                push(next);
            } else {
                entry->second.scheduled = false;
                pendingCount--;
            }
        }

        currentTime = time;
    }

    // Runs the single earliest task and advances the clock to its time.
    bool runNext() {
        while (!queue.empty()) {
            Task task = queue.top();
            queue.pop();

            auto entry = tasks.find(task.id);

            if (entry == tasks.end()) {
                continue;
            }
            if (!entry->second.scheduled || entry->second.cancelled) {
                continue;
            }

            currentTime = max(currentTime, task.executeAt);
            history.push_back({task.name, task.executeAt});

            if (task.interval > 0 &&
                task.executeAt <=
                    numeric_limits<long long>::max() - task.interval) {
                Task next = task;
                next.executeAt = task.executeAt + task.interval;
                entry->second.executeAt = next.executeAt;
                push(next);
            } else {
                entry->second.scheduled = false;
                pendingCount--;
            }

            return true;
        }

        return false;
    }

    int pending() const {
        return pendingCount;
    }

    long long time() const {
        return currentTime;
    }

    void printHistory() const {
        for (const Execution &execution : history) {
            cout << execution.time << " " << execution.name << endl;
        }
    }

    size_t executionCount() const {
        return history.size();
    }
};

int main() {
    Scheduler scheduler;

    // A heartbeat every 4 units starting at 3.
    int heartbeat = scheduler.scheduleRecurring("heartbeat", 3, 4);

    // Two tasks at time 5 with different priorities: alert runs first.
    int report = scheduler.schedule("report", 5, 0);
    int alert = scheduler.schedule("alert", 5, 5);

    int backup = scheduler.schedule("backup", 10, 1);
    int cleanup = scheduler.schedule("cleanup", 20, 0);

    cout << heartbeat << " " << report << " " << alert << " " << backup << " "
         << cleanup << endl;
    // 1 2 3 4 5

    cout << scheduler.pending() << endl;  // 5

    // Cancel the cleanup before it runs.
    cout << scheduler.cancel(cleanup) << endl;  // 1
    cout << scheduler.cancel(cleanup) << endl;  // 0 (already cancelled)
    cout << scheduler.pending() << endl;        // 4

    // Run through time 6: heartbeat@3, then alert@5 and report@5.
    scheduler.runUntil(6);
    scheduler.printHistory();
    // 3 heartbeat
    // 5 alert
    // 5 report

    cout << scheduler.time() << " " << scheduler.pending() << endl;
    // 6 2 (heartbeat and backup remain)

    // Run through time 15: heartbeat@7, backup@10, heartbeat@11, heartbeat@15.
    scheduler.runUntil(15);
    scheduler.printHistory();
    // 3 heartbeat
    // 5 alert
    // 5 report
    // 7 heartbeat
    // 10 backup
    // 11 heartbeat
    // 15 heartbeat

    cout << scheduler.pending() << endl;  // 1 (the recurring heartbeat)

    // runNext runs the next heartbeat at 19 and advances the clock.
    cout << scheduler.runNext() << endl;         // 1
    cout << scheduler.time() << endl;            // 19
    cout << scheduler.executionCount() << endl;  // 8

    // Cancel the recurring task; no more heartbeats.
    cout << scheduler.cancel(heartbeat) << endl;  // 1
    cout << scheduler.pending() << endl;          // 0

    scheduler.runUntil(100);
    cout << scheduler.executionCount() << endl;  // 8

    // A cancelled task never runs, even if its time has passed.
    cout << scheduler.runNext() << endl;  // 0

    // Same-time priority: high before low.
    Scheduler ties;
    int low = ties.schedule("low", 30, 2);
    int high = ties.schedule("high", 30, 5);
    int middle = ties.schedule("middle", 30, 2);

    cout << low << " " << high << " " << middle << endl;  // 1 2 3
    ties.runUntil(30);
    ties.printHistory();
    // 30 high
    // 30 low
    // 30 middle

    // Invalid schedules.
    cout << ties.schedule("bad", -1) << endl;              // -1
    cout << ties.schedule("", 10) << endl;                 // -1
    cout << ties.scheduleRecurring("bad", 1, 0) << endl;   // -1
    cout << ties.scheduleRecurring("bad", -1, 5) << endl;  // -1

    // Unknown cancellation.
    cout << ties.cancel(99) << endl;  // 0

    // A task scheduled in the past runs on the next tick.
    Scheduler past;
    past.schedule("late", 2);
    past.runUntil(10);
    past.printHistory();  // 2 late

    // Recurring catch-up: interval 2 from time 1, run until 7 executes 1,3,5,7.
    Scheduler catchUp;
    catchUp.scheduleRecurring("tick", 1, 2);
    catchUp.runUntil(7);
    catchUp.printHistory();
    // 1 tick
    // 3 tick
    // 5 tick
    // 7 tick
    cout << catchUp.pending() << endl;  // 1

    return 0;
}

/**
 * Time complexity:
 *   schedule / scheduleRecurring -> O(log n) heap push
 *   cancel -> O(log n) map lookup plus a lazy mark
 *   runUntil -> O(k log n) for k executions, plus O(log n) for each skipped
 *   cancelled entry
 *   runNext -> O(log n) amortized
 * Space complexity -> O(n) for the heap, registry, and history
 *
 * Special cases:
 *   - negative time -> rejected
 *   - empty name -> rejected
 *   - recurring interval <= 0 -> rejected
 *   - cancel an unknown or already executed task -> false
 *   - a cancelled task is skipped when it reaches the heap top
 *   - tasks at the same time run by priority, then id
 *   - runUntil with a time earlier than the current clock -> no-op
 *   - runUntil executes every missed occurrence of a recurring task
 *   - runNext advances the virtual clock to the task's scheduled time
 *   - a one-shot task is unscheduled after it runs
 *   - a recurring task stays pending until cancelled or its next timestamp
 *     would exceed the representable long long range
 *   - history is unbounded in this demo; a production scheduler samples or
 *     truncates it
 *
 * Why lazy cancellation: removing an arbitrary element from a binary heap is
 * O(n) unless the heap is indexed. Marking the task and discarding it later
 * keeps cancellation cheap, at the cost of a little wasted heap space. When
 * most tasks are cancelled, a periodic rebuild reclaims that space.
 *
 * Why recurring tasks keep their cadence: if a task is due at 3 and runs at 7,
 * scheduling the next run at 7 + interval would slowly drift. Using
 * 3 + interval keeps the long-term rate correct. The tradeoff is catch-up
 * storms after a long pause; a real scheduler chooses a missed-run policy.
 *
 * Extensions:
 *   - cron expressions and time zones
 *   - durable task storage and recovery
 *   - distributed leases so only one node runs a task
 *   - retries with exponential backoff and a dead-letter queue
 *   - dependencies (run B only after A succeeds)
 *   - concurrency limits per task group
 */

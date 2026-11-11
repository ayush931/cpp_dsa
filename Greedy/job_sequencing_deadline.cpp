/**
 * Job Sequencing with Deadlines
 *
 * Each job takes one unit of time, has a deadline, and earns a profit when it
 * is completed on or before that deadline. Only one job can run at a time.
 * The goal is to choose a feasible subset of jobs with maximum total profit.
 *
 * Greedy strategy:
 *   1. Sort jobs by profit in descending order.
 *   2. For each job, schedule it in the latest free slot at or before its
 *      deadline.
 *   3. If no slot is free, discard the job.
 *
 * Why it works: the highest-profit remaining job can always be moved into the
 * latest available slot without hurting any already scheduled job. This is an
 * exchange-argument proof of the greedy choice property.
 *
 * The implementation uses a disjoint-set "previous free slot" structure, so
 * finding a slot is almost O(1). Sorting dominates: O(n log n).
 *
 * A brute-force subset search is included for small inputs and is used to
 * verify the greedy result.
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

struct Job {
    int id;
    int deadline;
    int profit;
};

bool byProfit(const Job &a, const Job &b) {
    if (a.profit != b.profit) {
        return a.profit > b.profit;
    }
    if (a.deadline != b.deadline) {
        return a.deadline < b.deadline;
    }
    return a.id < b.id;
}

class DisjointSet {
   private:
    vector<int> parent;

   public:
    explicit DisjointSet(int n) : parent(n + 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] == x) {
            return x;
        }
        return parent[x] = find(parent[x]);
    }

    void occupy(int slot) {
        parent[slot] = find(slot - 1);
    }
};

struct Schedule {
    long long totalProfit;
    vector<int> jobIds;
};

Schedule jobSequencing(vector<Job> jobs) {
    sort(jobs.begin(), jobs.end(), byProfit);

    int maxDeadline = 0;
    for (const Job &job : jobs) {
        maxDeadline = max(maxDeadline, job.deadline);
    }

    Schedule result{0, {}};
    if (maxDeadline <= 0) {
        return result;
    }

    // At most jobs.size() slots can be occupied. Huge deadlines must not force
    // huge allocations or deep disjoint-set chains.
    maxDeadline = min(maxDeadline, static_cast<int>(jobs.size()));

    DisjointSet freeSlot(maxDeadline);

    for (const Job &job : jobs) {
        if (job.profit <= 0 || job.deadline <= 0) {
            continue;
        }

        int slot = freeSlot.find(min(job.deadline, maxDeadline));
        if (slot == 0) {
            continue;
        }

        result.totalProfit += job.profit;
        result.jobIds.push_back(job.id);
        freeSlot.occupy(slot);
    }

    sort(result.jobIds.begin(), result.jobIds.end());
    return result;
}

bool feasible(const vector<Job> &chosen) {
    vector<int> deadlines;
    for (const Job &job : chosen) {
        deadlines.push_back(job.deadline);
    }
    sort(deadlines.begin(), deadlines.end());

    for (int i = 0; i < static_cast<int>(deadlines.size()); i++) {
        if (deadlines[i] < i + 1) {
            return false;
        }
    }

    return true;
}

long long bruteForce(const vector<Job> &jobs) {
    int n = static_cast<int>(jobs.size());
    long long best = 0;

    for (int mask = 1; mask < (1 << n); mask++) {
        vector<Job> chosen;
        long long profit = 0;

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                chosen.push_back(jobs[i]);
                profit += jobs[i].profit;
            }
        }

        if (profit > best && feasible(chosen)) {
            best = profit;
        }
    }

    return best;
}

void printJobs(const vector<Job> &jobs) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(jobs.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << "{id=" << jobs[i].id << ", deadline=" << jobs[i].deadline
             << ", profit=" << jobs[i].profit << "}";
    }

    cout << "]";
}

int main() {
    vector<vector<Job>> tests = {
        {{1, 4, 20}, {2, 1, 10}, {3, 1, 40}, {4, 1, 30}},
        {{1, 2, 100}, {2, 1, 19}, {3, 2, 27}, {4, 1, 25}, {5, 3, 15}},
        {{1, 1, 5}, {2, 1, 4}, {3, 1, 3}},
        {{1, 0, 50}, {2, 2, 10}},
        {{1, 3, 0}, {2, 3, -5}},
        {},
        {{1, 5, 7}}};

    for (const vector<Job> &jobs : tests) {
        Schedule greedy = jobSequencing(jobs);
        long long optimal = bruteForce(jobs);

        printJobs(jobs);
        cout << endl;
        cout << "greedy profit: " << greedy.totalProfit << ", jobs: [";

        for (int i = 0; i < static_cast<int>(greedy.jobIds.size()); i++) {
            if (i > 0) {
                cout << ", ";
            }
            cout << greedy.jobIds[i];
        }

        cout << "]" << endl;
        cout << "brute-force profit: " << optimal << endl;
        cout << "matches: " << (greedy.totalProfit == optimal ? "yes" : "no")
             << endl;
        cout << "---" << endl;
    }

    return 0;
}

// Expected output (abridged):
// greedy profit: 60, jobs: [1, 3]
// brute-force profit: 60
// matches: yes
// ---
//
// Special cases: empty job list, a single job, all jobs sharing one deadline,
// zero or negative profit jobs, deadlines beyond the number of jobs, and
// duplicate deadlines.

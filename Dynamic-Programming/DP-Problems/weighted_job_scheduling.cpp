/**
 * Weighted interval scheduling: maximum profit from non-overlapping jobs.
 * Half-open jobs [start,end), start<end; touching endpoints are compatible.
 * Greedy earliest-finish works for COUNT, but can lose profitable jobs here.
 * Sort by finish. dp[i] = max(skip i, profit[i] + best compatible prefix).
 * upper_bound finds that prefix. O(n log n) time, O(n) space including copy.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

struct Job {
    int start, end, profit;
};

long long maximumJobProfit(vector<Job> jobs) {
    for (const auto &job : jobs) {
        if (job.start >= job.end) {
            throw invalid_argument("start must precede end");
        }
    }
    sort(jobs.begin(), jobs.end(),
         [](const Job &a, const Job &b) { return a.end < b.end; });
    vector<int> ends;
    vector<long long> best(jobs.size() + 1);
    for (size_t i = 0; i < jobs.size(); ++i) {
        size_t compatible =
            upper_bound(ends.begin(), ends.end(), jobs[i].start) - ends.begin();
        best[i + 1] = max(best[i], best[compatible] + jobs[i].profit);
        ends.push_back(jobs[i].end);
    }
    return best
        .back();  // choosing no jobs is allowed, including negative profits
}

int main() {
    assert(maximumJobProfit({{1, 3, 50}, {2, 4, 10}, {3, 5, 40}, {3, 6, 70}}) ==
           120);
    assert(maximumJobProfit({{1, 2, 10}, {2, 3, 20}}) == 30);
    assert(maximumJobProfit({{1, 3, 5}, {1, 3, 9}}) == 9);
    assert(maximumJobProfit({{1, 2, -3}}) == 0);
    assert(maximumJobProfit({}) == 0);
    cout << maximumJobProfit({{1, 3, 50}, {3, 6, 70}}) << '\n';  // 120
}

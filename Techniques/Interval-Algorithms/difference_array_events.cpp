/**
 * Difference array and offline events
 *
 * Range add, point query:
 *   Apply many updates [left, right] += value, then answer values at points.
 *
 * Dense difference array:
 *   diff[left] += value
 *   diff[right + 1] -= value
 *   prefix sum of diff gives every position in O(n + updates)
 *
 * Offline events:
 *   When coordinates are sparse or very large (including negative values), do
 *   not allocate an array over the coordinate range. Instead create events:
 *
 *     (left, +value) and (right + 1, -value)
 *
 *   Sort the events and the queries by coordinate, sweep once, and answer each
 *   query with the running sum. A query at x applies every event at position
 *   <= x, which correctly includes starts at x and excludes the range after its
 *   right endpoint.
 *
 * Brute force:
 *   For each query, scan every update and sum the values whose range contains
 *   the query point.
 *
 * Time        -> dense O(n + updates), events O((updates + queries) log(...)),
 *                brute O(queries * updates)
 * Space       -> dense O(n), events O(updates + queries)
 *
 * Edge cases: empty updates, single-point ranges, duplicate updates, negative
 * coordinates, full-range updates, and queries outside every range.
 */

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

struct Update {
    int left;
    int right;
    long long value;
};

vector<long long> applyUpdatesDense(int n, const vector<Update> &updates) {
    vector<long long> difference(n + 1, 0);

    for (const Update &update : updates) {
        if (update.left < 0 || update.right >= n ||
            update.left > update.right) {
            continue;
        }

        difference[update.left] += update.value;
        difference[update.right + 1] -= update.value;
    }

    vector<long long> values(n);
    long long running = 0;

    for (int i = 0; i < n; i++) {
        running += difference[i];
        values[i] = running;
    }

    return values;
}

long long pointQueryBrute(const vector<Update> &updates, int point) {
    long long sum = 0;

    for (const Update &update : updates) {
        if (update.left <= point && point <= update.right) {
            sum += update.value;
        }
    }

    return sum;
}

vector<long long> pointQueriesWithEvents(const vector<Update> &updates,
                                         const vector<int> &queries) {
    vector<pair<long long, long long>> events;

    for (const Update &update : updates) {
        events.push_back({(long long)update.left, update.value});
        events.push_back({(long long)update.right + 1, -update.value});
    }

    sort(events.begin(), events.end());

    vector<pair<int, int>> orderedQueries;

    for (int i = 0; i < (int)queries.size(); i++) {
        orderedQueries.push_back({queries[i], i});
    }

    sort(orderedQueries.begin(), orderedQueries.end());

    vector<long long> answers(queries.size(), 0);
    long long running = 0;
    int eventIndex = 0;

    for (const pair<int, int> &query : orderedQueries) {
        int point = query.first;
        int originalIndex = query.second;

        while (eventIndex < (int)events.size() &&
               events[eventIndex].first <= point) {
            running += events[eventIndex].second;
            eventIndex++;
        }

        answers[originalIndex] = running;
    }

    return answers;
}

void print(const vector<long long> &values) {
    cout << "[";

    for (int i = 0; i < (int)values.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << values[i];
    }

    cout << "]";
}

int main() {
    vector<Update> denseUpdates = {{0, 2, 3}, {1, 4, -1}, {3, 3, 5}, {0, 5, 2}};

    cout << "dense values: ";
    print(applyUpdatesDense(6, denseUpdates));
    cout << endl;
    cout << "dense point 3: " << pointQueryBrute(denseUpdates, 3) << endl;

    vector<Update> sparseUpdates = {
        {-5, -2, 4}, {0, 3, 7}, {-2, 0, -1}, {10, 10, 9}};
    vector<int> queries = {-6, -5, -2, -1, 0, 3, 4, 10, 11};

    vector<long long> eventAnswers =
        pointQueriesWithEvents(sparseUpdates, queries);
    cout << "event answers: ";
    print(eventAnswers);
    cout << endl;

    bool agree = true;

    for (int i = 0; i < (int)queries.size(); i++) {
        if (eventAnswers[i] != pointQueryBrute(sparseUpdates, queries[i])) {
            agree = false;
        }
    }

    cout << "events match brute: " << (agree ? "yes" : "no") << endl;

    vector<int> edgeQueries = {-2, 0, 5};
    cout << "empty updates: ";
    print(pointQueriesWithEvents({}, edgeQueries));
    cout << endl;

    vector<Update> singlePoint = {{3, 3, 7}};
    vector<int> singleQueries = {2, 3, 4};
    cout << "single point: ";
    print(pointQueriesWithEvents(singlePoint, singleQueries));
    cout << endl;

    vector<Update> fullRange = {{-100, 100, 5}};
    vector<int> fullQueries = {-100, 0, 100, 101};
    cout << "full range: ";
    print(pointQueriesWithEvents(fullRange, fullQueries));
    cout << endl;

    vector<Update> duplicateUpdates = {{1, 4, 2}, {1, 4, 2}};
    vector<int> duplicateQueries = {0, 1, 4, 5};
    cout << "duplicates: ";
    print(pointQueriesWithEvents(duplicateUpdates, duplicateQueries));
    cout << endl;

    return 0;
}

// Expected output:
// dense values: [5, 4, 4, 6, 1, 2]
// dense point 3: 6
// event answers: [0, 4, 3, 3, 10, 10, 3, 9, 0]
// events match brute: yes
// empty updates: [0, 0, 0]
// single point: [0, 7, 0]
// full range: [5, 5, 5, 0]
// duplicates: [0, 4, 4, 0]
//
// Special cases: empty updates, single-point ranges, duplicate updates,
// negative coordinates, full-range updates, and queries outside every range.
//
// The dense version needs coordinates in [0, n). The event version works for
// any coordinate, including negative values, because it stores only the
// coordinates that actually appear.

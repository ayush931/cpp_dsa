/**
 * Mo's algorithm -> answers offline range queries efficiently
 * Sort queries by block of left endpoint, then by right endpoint
 * Move the current window [currentLeft, currentRight] to each query
 * Total movement -> O((n + q) * sqrt(n))
 * Best for problems where add/remove operations are O(1)
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

struct Query {
    int left;
    int right;
    int index;
};

int main() {
    vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vector<pair<int, int>> inputQueries = {{0, 4}, {1, 3}, {2, 8}, {5, 8}};

    int n = arr.size();
    int q = inputQueries.size();
    int blockSize = sqrt(n) + 1;

    vector<Query> queries(q);

    for (int i = 0; i < q; i++) {
        queries[i] = {inputQueries[i].first, inputQueries[i].second, i};
    }

    sort(queries.begin(), queries.end(), [blockSize](Query &a, Query &b) {
        int blockA = a.left / blockSize;
        int blockB = b.left / blockSize;

        if (blockA != blockB) {
            return blockA < blockB;
        }

        // alternate the order to reduce right pointer movement
        if (blockA % 2 == 0) {
            return a.right < b.right;
        }
        return a.right > b.right;
    });

    vector<int> answers(q);
    int currentLeft = 0;
    int currentRight = -1;
    int currentSum = 0;

    for (Query &query : queries) {
        while (currentLeft > query.left) {
            currentLeft--;
            currentSum += arr[currentLeft];
        }

        while (currentRight < query.right) {
            currentRight++;
            currentSum += arr[currentRight];
        }

        while (currentLeft < query.left) {
            currentSum -= arr[currentLeft];
            currentLeft++;
        }

        while (currentRight > query.right) {
            currentSum -= arr[currentRight];
            currentRight--;
        }

        answers[query.index] = currentSum;
    }

    for (int answer : answers) {
        cout << answer << " ";
    }
    cout << endl;

    return 0;
}

// Special cases: empty range, duplicate queries, large q,
// updates between queries (use Mo's with updates), add/remove not O(1)

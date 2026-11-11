/**
 * Online algorithms
 *
 * An online algorithm must decide without knowing future requests. Its quality
 * is measured by the competitive ratio: the worst-case ratio between its cost
 * and the cost of an optimal offline algorithm that knows the future.
 *
 * 1. Ski rental
 *    Rent for 1 per day or buy for B. Deterministic strategy: rent until the
 *    total rental cost reaches B, then buy. This is 2-competitive.
 *
 * 2. Paging / caching
 *    FIFO and LRU are online. Belady's optimal algorithm evicts the page whose
 *    next use is farthest in the future; it is offline.
 *
 * Edge cases: zero requests, zero buy price, repeated pages, cache larger than
 * the number of distinct pages, and a request sequence with no future use.
 */

#include <climits>
#include <iostream>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

int skiRental(int days, int buyPrice) {
    int rentalCost = 0;

    for (int day = 0; day < days; day++) {
        if (rentalCost + 1 < buyPrice) {
            rentalCost++;
        } else {
            rentalCost = buyPrice;
            break;
        }
    }

    return min(rentalCost, days);
}

int optimalSkiRental(int days, int buyPrice) {
    return min(days, buyPrice);
}

int fifoPageFaults(vector<int> &requests, int capacity) {
    if (capacity <= 0) {
        return requests.size();
    }

    unordered_set<int> cache;
    list<int> order;
    int faults = 0;

    for (int page : requests) {
        if (cache.count(page)) {
            continue;
        }

        faults++;

        if ((int)cache.size() == capacity) {
            int evicted = order.front();
            order.pop_front();
            cache.erase(evicted);
        }

        cache.insert(page);
        order.push_back(page);
    }

    return faults;
}

int lruPageFaults(vector<int> &requests, int capacity) {
    if (capacity <= 0) {
        return requests.size();
    }

    list<int> order;
    unordered_map<int, list<int>::iterator> position;
    int faults = 0;

    for (int page : requests) {
        if (position.count(page)) {
            order.erase(position[page]);
            order.push_front(page);
            position[page] = order.begin();
            continue;
        }

        faults++;

        if ((int)order.size() == capacity) {
            int evicted = order.back();
            order.pop_back();
            position.erase(evicted);
        }

        order.push_front(page);
        position[page] = order.begin();
    }

    return faults;
}

int beladyPageFaults(vector<int> &requests, int capacity) {
    if (capacity <= 0) {
        return requests.size();
    }

    vector<int> cache;
    int faults = 0;

    for (int i = 0; i < (int)requests.size(); i++) {
        int page = requests[i];
        bool found = false;

        for (int cached : cache) {
            if (cached == page) {
                found = true;
                break;
            }
        }

        if (found) {
            continue;
        }

        faults++;

        if ((int)cache.size() < capacity) {
            cache.push_back(page);
            continue;
        }

        int farthest = -1;
        int victimIndex = 0;

        for (int j = 0; j < (int)cache.size(); j++) {
            int nextUse = INT_MAX;

            for (int k = i + 1; k < (int)requests.size(); k++) {
                if (requests[k] == cache[j]) {
                    nextUse = k;
                    break;
                }
            }

            if (nextUse > farthest) {
                farthest = nextUse;
                victimIndex = j;
            }
        }

        cache[victimIndex] = page;
    }

    return faults;
}

int main() {
    cout << skiRental(10, 5) << endl;         // 5 (rent 4 days, buy on day 5)
    cout << optimalSkiRental(10, 5) << endl;  // 5
    cout << skiRental(3, 5) << endl;          // 3 (rent all days)
    cout << skiRental(10, 0) << endl;         // 0 (buy immediately)

    vector<int> requests = {1, 2, 3, 1, 4, 2, 5, 1, 2, 3, 5};
    int capacity = 3;

    cout << fifoPageFaults(requests, capacity) << endl;    // 9
    cout << lruPageFaults(requests, capacity) << endl;     // 9
    cout << beladyPageFaults(requests, capacity) << endl;  // 6

    return 0;
}

// Special cases:
// - buying on day 0 is free only when the buy price is 0
// - a cache with capacity 0 faults on every request
// - a cache larger than the distinct page count faults once per distinct page
// - Belady's algorithm needs future knowledge and is only a comparison baseline

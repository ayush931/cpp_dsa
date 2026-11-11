/**
 * Capacity, reserve and complexity
 *
 * vector grows geometrically (usually 1.5x or 2x), so push_back is amortized
 * O(1). Without reserve, every growth copies or moves all elements.
 *
 * reserve(n) -> allocate capacity for at least n elements, no reallocation
 *              until size exceeds n
 * resize(n) -> change the size, constructing or destroying elements
 * shrink_to_fit() -> request to reduce capacity (non-binding)
 *
 * unordered_map reserve(n) -> allocate enough buckets for n elements and
 * avoid rehashing
 *
 * Complexity cheat sheet:-
 * vector: push_back amortized O(1), insert/erase in the middle O(n)
 * deque: push_front/push_back O(1), random access O(1)
 * list: insert/erase O(1) if you have the iterator, lookup O(n)
 * map/set: O(log n)
 * unordered_map/set: O(1) average, O(n) worst
 * priority_queue: push/pop O(log n), top O(1)
 */

#include <chrono>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;
using namespace chrono;

struct BigObject {
    int data[64];

    BigObject(int value = 0) {
        for (int i = 0; i < 64; i++) {
            data[i] = value;
        }
    }
};

int main() {
    const int count = 200000;

    vector<BigObject> withoutReserve;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < count; i++) {
        withoutReserve.push_back(BigObject(i));
    }

    auto end = high_resolution_clock::now();

    cout << "without reserve: "
         << duration_cast<milliseconds>(end - start).count() << " ms, capacity "
         << withoutReserve.capacity() << endl;

    vector<BigObject> withReserve;
    withReserve.reserve(count);

    start = high_resolution_clock::now();

    for (int i = 0; i < count; i++) {
        withReserve.push_back(BigObject(i));
    }

    end = high_resolution_clock::now();

    cout << "with reserve: " << duration_cast<milliseconds>(end - start).count()
         << " ms, capacity " << withReserve.capacity() << endl;

    // unordered_map rehashing
    unordered_map<int, int> withoutBucketReserve;

    start = high_resolution_clock::now();

    for (int i = 0; i < count; i++) {
        withoutBucketReserve[i] = i;
    }

    end = high_resolution_clock::now();
    cout << "map without reserve: "
         << duration_cast<milliseconds>(end - start).count() << " ms, buckets "
         << withoutBucketReserve.bucket_count() << endl;

    unordered_map<int, int> withBucketReserve;
    withBucketReserve.reserve(count);

    start = high_resolution_clock::now();

    for (int i = 0; i < count; i++) {
        withBucketReserve[i] = i;
    }

    end = high_resolution_clock::now();
    cout << "map with reserve: "
         << duration_cast<milliseconds>(end - start).count() << " ms, buckets "
         << withBucketReserve.bucket_count() << endl;

    // capacity growth is geometric, so the number of reallocations is O(log n)
    vector<int> growth;
    int lastCapacity = 0;

    for (int i = 0; i < 1000; i++) {
        growth.push_back(i);

        if (growth.capacity() != lastCapacity) {
            lastCapacity = growth.capacity();
            cout << i << ":" << lastCapacity << " ";
        }
    }
    cout << endl;

    return 0;
}

// Special cases: reserve with a huge n can throw bad_alloc or waste memory;
// reserve does not change size, so indexing beyond size is still UB;
// calling reserve after taking a reference/iterator invalidates it;
// unordered_map worst case is O(n) per operation with a bad hash;
// do not reserve based on an attacker-controlled count

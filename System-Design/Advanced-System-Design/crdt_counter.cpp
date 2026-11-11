/**
 * CRDT Counters: G-Counter and PN-Counter
 *
 * A CRDT (conflict-free replicated data type) can be merged across replicas
 * without coordination. The merge must be:
 *   - commutative: merge(A, B) == merge(B, A)
 *   - associative: merge(merge(A, B), C) == merge(A, merge(B, C))
 *   - idempotent: merge(A, A) == A
 *
 * G-Counter (grow-only counter)
 *   - each replica has its own counter
 *   - a replica may only increment its own entry
 *   - value = sum of all entries
 *   - merge = element-wise maximum
 *
 * Why maximum works: a replica's counter only grows, so the larger value is
 * always the more recent one. Two replicas cannot conflict on the same entry
 * because only the owner writes it.
 *
 * PN-Counter (positive-negative counter)
 *   - two G-Counters: one for increments, one for decrements
 *   - value = sum(increments) - sum(decrements)
 *   - merge = merge both components
 *   - it can go negative; if that is not allowed, enforce the invariant in the
 *     application or use a bounded counter
 *
 * These counters are state-based CRDTs: replicas exchange their full state and
 * merge it. Delta-state CRDTs send only the changed components to reduce
 * bandwidth.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <string>
using namespace std;

class GCounter {
    map<string, long long> counts;

   public:
    void increment(const string &replica, long long amount = 1) {
        assert(amount >= 0);
        counts[replica] += amount;
    }

    long long value() const {
        long long total = 0;

        for (const auto &[replica, count] : counts) {
            total += count;
        }

        return total;
    }

    void merge(const GCounter &other) {
        for (const auto &[replica, count] : other.counts) {
            counts[replica] = max(counts[replica], count);
        }
    }

    const map<string, long long> &state() const {
        return counts;
    }

    void print(const string &label) const {
        cout << label << " {";

        bool first = true;

        for (const auto &[replica, count] : counts) {
            if (!first) {
                cout << ", ";
            }
            cout << replica << ":" << count;
            first = false;
        }

        cout << "} value=" << value() << endl;
    }
};

class PNCounter {
    GCounter increments;
    GCounter decrements;

   public:
    void increment(const string &replica, long long amount = 1) {
        increments.increment(replica, amount);
    }

    void decrement(const string &replica, long long amount = 1) {
        decrements.increment(replica, amount);
    }

    long long value() const {
        return increments.value() - decrements.value();
    }

    void merge(const PNCounter &other) {
        increments.merge(other.increments);
        decrements.merge(other.decrements);
    }

    const GCounter &positive() const {
        return increments;
    }

    const GCounter &negative() const {
        return decrements;
    }

    void print(const string &label) const {
        cout << label << " value=" << value() << " ";
        increments.print("P");
        decrements.print("N");
    }
};

int main() {
    // Three replicas of a grow-only counter.
    GCounter a;
    GCounter b;
    GCounter c;

    a.increment("A", 3);
    b.increment("B", 2);
    c.increment("C", 5);

    a.print("A");  // {A:3} value=3
    b.print("B");  // {B:2} value=2
    c.print("C");  // {C:5} value=5

    // Merge order does not matter.
    GCounter first = a;
    first.merge(b);
    first.merge(c);
    first.print("first");  // {A:3, B:2, C:5} value=10

    GCounter second = c;
    second.merge(a);
    second.merge(b);
    second.print("second");  // {A:3, B:2, C:5} value=10

    cout << (first.value() == second.value()) << endl;  // 1

    // Idempotence: merging the same state twice changes nothing.
    GCounter before = first;
    first.merge(second);
    first.merge(second);
    cout << (first.value() == before.value()) << endl;  // 1

    // A PN-Counter: A adds 10, B adds 4, C removes 3.
    PNCounter pnA;
    PNCounter pnB;
    PNCounter pnC;

    pnA.increment("A", 10);
    pnB.increment("B", 4);
    pnC.decrement("C", 3);

    pnA.print("pnA");
    pnB.print("pnB");
    pnC.print("pnC");

    PNCounter merged = pnA;
    merged.merge(pnB);
    merged.merge(pnC);
    merged.print("merged");  // value=11

    PNCounter mergedReverse = pnC;
    mergedReverse.merge(pnA);
    mergedReverse.merge(pnB);
    mergedReverse.print("reverse");  // value=11

    cout << (merged.value() == mergedReverse.value()) << endl;  // 1

    // Concurrent increments on different replicas both survive.
    PNCounter concurrent = pnA;
    concurrent.merge(pnB);
    concurrent.increment("A", 1);
    concurrent.merge(pnC);
    concurrent.print("concurrent");  // value=12

    // A counter can go negative if decrements exceed increments.
    PNCounter negative;
    negative.decrement("X", 5);
    negative.increment("Y", 2);
    negative.print("negative");  // value=-3

    // Merging a replica with itself is a no-op.
    PNCounter copy = merged;
    copy.merge(merged);
    cout << (copy.value() == merged.value()) << endl;  // 1

    // State inspection: the positive and negative components are separate.
    cout << merged.positive().value() << " " << merged.negative().value()
         << endl;
    // 14 3

    return 0;
}

/**
 * Time complexity:
 *   increment -> O(log replicas) for the map
 *   value -> O(replicas)
 *   merge -> O(replicas log replicas)
 * Space complexity -> O(replicas) per counter
 *
 * Special cases:
 *   - two replicas incrementing independently: both increments survive
 *   - the same update delivered twice: the maximum makes it idempotent
 *   - a decrement on one replica and an increment on another: both are kept
 *   - decrements larger than increments: the value can be negative
 *   - a replica that has never incremented: its entry is absent, which is
 *     equivalent to 0
 *   - integer overflow: use a wider type or saturating arithmetic for very
 *     long-running counters
 *   - merge with a replica that has a smaller counter: the maximum keeps the
 *     larger value, so stale state cannot roll the counter back
 *
 * G-Counters are used for distributed counters, analytics, rate limiting, and
 * as the per-process components of vector clocks. PN-Counters are used for
 * inventory-like counters where both additions and removals happen. A real
 * system may combine them with a bounded counter, an OR-Set, or a
 * last-write-wins register depending on the semantics it needs.
 */

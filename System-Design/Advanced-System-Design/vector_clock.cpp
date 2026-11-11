/**
 * Vector Clocks
 *
 * A vector clock tracks causality in a distributed system. Each process keeps
 * one counter per process:
 *
 *   clock[A] = number of events at A known to this clock
 *   clock[B] = number of events at B known to this clock
 *   ...
 *
 * Rules:
 *   - before a local event, increment your own entry (tick)
 *   - when sending a message, attach the current clock
 *   - when receiving a message, merge the sender's clock with yours
 *     (element-wise maximum), then tick your own entry
 *
 * Comparison of two clocks X and Y:
 *   - X == Y: the same causal history
 *   - X < Y: X happens before Y (X <= Y component-wise, and at least one is
 *     strictly smaller)
 *   - X > Y: X happens after Y
 *   - otherwise: X and Y are concurrent
 *
 * A vector clock detects concurrency, but it does not resolve it. A conflict
 * resolver (last-write-wins, a CRDT merge, or an application rule) decides what
 * to do with concurrent versions.
 *
 * Cost: O(number of processes) space per clock. With a fixed process set, a
 * comparison is O(number of processes); the map-based version below is
 * O(processes log processes). For many processes, use a dotted version vector,
 * a hash of the process set, or a hybrid logical clock.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

enum class Relation { Equal, Before, After, Concurrent };

class VectorClock {
    map<string, int> clock;

   public:
    void tick(const string &process) {
        clock[process]++;
    }

    void merge(const VectorClock &other) {
        for (const auto &[process, counter] : other.clock) {
            clock[process] = max(clock[process], counter);
        }
    }

    // Receiving a message: merge the sender's history, then record the receive
    // event as a local event.
    void receive(const string &process, const VectorClock &sender) {
        merge(sender);
        tick(process);
    }

    int at(const string &process) const {
        auto entry = clock.find(process);
        return entry == clock.end() ? 0 : entry->second;
    }

    Relation compare(const VectorClock &other) const {
        bool smaller = false;
        bool larger = false;

        set<string> processes;

        for (const auto &[process, counter] : clock) {
            processes.insert(process);
        }
        for (const auto &[process, counter] : other.clock) {
            processes.insert(process);
        }

        for (const string &process : processes) {
            int left = at(process);
            int right = other.at(process);

            if (left < right) {
                smaller = true;
            }
            if (left > right) {
                larger = true;
            }
        }

        if (!smaller && !larger) {
            return Relation::Equal;
        }
        if (smaller && !larger) {
            return Relation::Before;
        }
        if (!smaller && larger) {
            return Relation::After;
        }

        return Relation::Concurrent;
    }

    const map<string, int> &state() const {
        return clock;
    }

    void print() const {
        cout << "{";

        bool first = true;

        for (const auto &[process, counter] : clock) {
            if (!first) {
                cout << ", ";
            }
            cout << process << ":" << counter;
            first = false;
        }

        cout << "}";
    }
};

string relationName(Relation relation) {
    switch (relation) {
        case Relation::Equal:
            return "equal";
        case Relation::Before:
            return "before";
        case Relation::After:
            return "after";
        case Relation::Concurrent:
            return "concurrent";
    }

    return "unknown";
}

void show(const string &label, const VectorClock &clock) {
    cout << label << " ";
    clock.print();
    cout << endl;
}

int main() {
    // A and B are independent processes.
    VectorClock a;
    VectorClock b;

    a.tick("A");
    show("A1", a);  // {A:1}

    // B receives A's message: merge {A:1}, then tick B.
    b.receive("B", a);
    show("B1", b);  // {A:1, B:1}

    // A performs another local event. A has not seen B's event, and B has not
    // seen this new A event, so the two clocks are concurrent.
    a.tick("A");
    show("A2", a);  // {A:2}

    cout << relationName(a.compare(b)) << endl;  // concurrent
    cout << relationName(b.compare(a)) << endl;  // concurrent

    // B performs another local event; still concurrent with A2.
    b.tick("B");
    show("B2", b);                               // {A:1, B:2}
    cout << relationName(a.compare(b)) << endl;  // concurrent

    // C receives from A2 and then from B2. It has seen both histories.
    VectorClock c;
    c.receive("C", a);
    c.receive("C", b);
    show("C", c);  // {A:2, B:2, C:2}

    cout << relationName(c.compare(a)) << endl;  // after
    cout << relationName(c.compare(b)) << endl;  // after
    cout << relationName(a.compare(c)) << endl;  // before

    // Equal clocks.
    VectorClock copy = a;
    cout << relationName(a.compare(copy)) << endl;  // equal

    // A clear happens-before chain: D ticks, E receives, so D < E.
    VectorClock d;
    VectorClock e;

    d.tick("D");
    e.receive("E", d);

    show("D", d);                                // {D:1}
    show("E", e);                                // {D:1, E:1}
    cout << relationName(d.compare(e)) << endl;  // before
    cout << relationName(e.compare(d)) << endl;  // after

    // Merging two concurrent clocks produces a clock that is after both.
    VectorClock merged = a;
    merged.merge(b);
    show("merged", merged);                           // {A:2, B:2}
    cout << relationName(merged.compare(a)) << endl;  // after
    cout << relationName(merged.compare(b)) << endl;  // after

    // Independent processes that never communicated are concurrent.
    VectorClock f;
    VectorClock g;
    f.tick("F");
    g.tick("G");
    cout << relationName(f.compare(g)) << endl;  // concurrent

    return 0;
}

/**
 * Time complexity:
 *   tick -> O(log processes) for the map
 *   merge -> O(processes log processes)
 *   compare -> O(processes log processes) to union the keys and compare
 * Space complexity -> O(processes) per clock
 *
 * Special cases:
 *   - an empty clock is the initial state; it is before any clock that has
 *     at least one event
 *   - two empty clocks are equal
 *   - a process that has never been seen has counter 0
 *   - concurrent updates must be resolved by the application; the clock only
 *     reports that they are concurrent
 *   - the map grows with every process that has ever been observed; a fixed
 *     process set can use a vector instead
 *   - integer counters can overflow after 2^31 events on one process; use a
 *     wider type or a hybrid logical clock for long-running systems
 *
 * Vector clocks are used for conflict detection in distributed databases
 * (Dynamo-style stores), version control, collaborative editors, and causal
 * broadcast. They are not a total order: concurrent events have no natural
 * winner. A total order needs an extra tie-breaker such as a timestamp or a
 * process id.
 */

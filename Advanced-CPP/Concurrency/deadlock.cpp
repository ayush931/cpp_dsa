/**
 * Deadlock -> two or more threads wait forever for locks held by each other
 *
 * Four conditions must all hold:
 * 1. Mutual exclusion
 * 2. Hold and wait
 * 3. No preemption
 * 4. Circular wait
 *
 * Prevention:-
 * - Lock ordering -> always acquire locks in the same global order
 * - std::lock / std::scoped_lock -> lock multiple mutexes without deadlock
 *   (the implementation uses a try-and-back-off algorithm)
 * - std::unique_lock with defer_lock + std::lock
 * - Try lock with a timeout, then release everything and retry
 * - Avoid calling unknown code while holding a lock
 *
 * Livelock -> threads keep changing state but make no progress
 * Starvation -> one thread never gets the lock
 */

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

struct Account {
    int balance;
    mutex mtx;

    Account(int value) {
        balance = value;
    }
};

// unsafe: lock order depends on the argument order
void transferUnsafe(Account &from, Account &to, int amount) {
    lock_guard<mutex> first(from.mtx);
    lock_guard<mutex> second(to.mtx);

    from.balance -= amount;
    to.balance += amount;
}

// safe: std::scoped_lock locks both mutexes without a circular wait
void transferSafe(Account &from, Account &to, int amount) {
    scoped_lock lock(from.mtx, to.mtx);

    from.balance -= amount;
    to.balance += amount;
}

// safe: explicit defer + std::lock
void transferSafeExplicit(Account &from, Account &to, int amount) {
    unique_lock<mutex> first(from.mtx, defer_lock);
    unique_lock<mutex> second(to.mtx, defer_lock);

    lock(first, second);  // deadlock-free acquisition

    from.balance -= amount;
    to.balance += amount;
}

int main() {
    Account alice(1000);
    Account bob(1000);

    thread first(transferSafe, ref(alice), ref(bob), 100);
    thread second(transferSafe, ref(bob), ref(alice), 200);

    first.join();
    second.join();

    cout << alice.balance << " " << bob.balance << endl;  // 900 1100

    thread third(transferSafeExplicit, ref(alice), ref(bob), 50);
    thread fourth(transferSafeExplicit, ref(bob), ref(alice), 25);

    third.join();
    fourth.join();

    cout << alice.balance << " " << bob.balance << endl;  // 875 1125

    return 0;
}

// Special cases: recursive locking on a non-recursive mutex (undefined),
// locking the same mutex twice in one thread (deadlock), a callback that
// locks another mutex while a lock is held, priority inversion,
// condition_variable wait without a predicate
// Debugging: attach a debugger and inspect each thread's lock, or use
// thread sanitizer (-fsanitize=thread)

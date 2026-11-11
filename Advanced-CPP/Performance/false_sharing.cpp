/**
 * False sharing -> two threads write different variables that happen to live
 * on the same cache line
 *
 * The cache coherence protocol invalidates the whole line for every write,
 * so the threads keep fighting even though they never touch the same object.
 * The program is correct but slow.
 *
 * Fix: pad each hot variable to a cache line (alignas(64)), or give each
 * thread its own local counter and merge at the end.
 *
 * True sharing -> both threads really access the same variable, needs atomics
 * or a mutex.
 */

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;
using namespace chrono;

struct SharedCounters {
    long long first = 0;
    long long second = 0;
};

struct PaddedCounter {
    alignas(64) long long value = 0;
};

struct PaddedCounters {
    PaddedCounter first;
    PaddedCounter second;
};

int main() {
    const int iterations = 50000000;

    // false sharing: both counters are likely on one cache line
    SharedCounters shared;

    auto start = high_resolution_clock::now();

    thread first([&shared]() {
        for (int i = 0; i < iterations; i++) {
            shared.first++;
        }
    });

    thread second([&shared]() {
        for (int i = 0; i < iterations; i++) {
            shared.second++;
        }
    });

    first.join();
    second.join();

    auto end = high_resolution_clock::now();

    cout << "false sharing: "
         << duration_cast<milliseconds>(end - start).count() << " ms" << endl;

    // padded: each counter owns its own cache line
    PaddedCounters padded;

    start = high_resolution_clock::now();

    thread third([&padded]() {
        for (int i = 0; i < iterations; i++) {
            padded.first.value++;
        }
    });

    thread fourth([&padded]() {
        for (int i = 0; i < iterations; i++) {
            padded.second.value++;
        }
    });

    third.join();
    fourth.join();

    end = high_resolution_clock::now();

    cout << "padded: " << duration_cast<milliseconds>(end - start).count()
         << " ms" << endl;

    // per-thread accumulation: each thread works on a stack-local counter and
    // publishes the result once, so there is no sharing in the hot loop
    long long firstTotal = 0;
    long long secondTotal = 0;
    long long total = 0;

    start = high_resolution_clock::now();

    thread fifth([&firstTotal]() {
        long long local = 0;
        for (int i = 0; i < iterations; i++) {
            local++;
        }
        firstTotal = local;
    });

    thread sixth([&secondTotal]() {
        long long local = 0;
        for (int i = 0; i < iterations; i++) {
            local++;
        }
        secondTotal = local;
    });

    fifth.join();
    sixth.join();

    total = firstTotal + secondTotal;

    end = high_resolution_clock::now();

    cout << "per-thread: " << duration_cast<milliseconds>(end - start).count()
         << " ms" << endl;
    cout << total << endl;

    return 0;
}

// Special cases: adjacent vector elements always share cache lines;
// std::hardware_destructive_interference_size (C++17) gives the cache line
// size, but it is only a hint; padding wastes memory (64 bytes per counter);
// false sharing also happens between a mutex and nearby data;
// measure with perf c2c or Instruments, not by guessing

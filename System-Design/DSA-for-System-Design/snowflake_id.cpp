/**
 * Snowflake ID Generator
 *
 * A distributed system needs unique, roughly time-ordered 64-bit IDs without
 * a central database sequence. Twitter's Snowflake layout:
 *
 *   | 1 unused sign bit | 41 bits timestamp | 5 bits datacenter |
 *   5 bits worker | 12 bits sequence |
 *
 *   - timestamp: milliseconds since a custom epoch (about 69 years)
 *   - datacenter + worker: 10 bits -> 1024 generators
 *   - sequence: 12 bits -> 4096 IDs per millisecond per generator
 *
 * That is up to about 4 million IDs per millisecond across the fleet, and the
 * IDs sort by creation time.
 *
 * Brute force -> a single auto-increment counter. Simple, but it is a single
 * point of failure, it leaks volume, and it is hard to shard.
 *
 * Random UUIDv4 identifiers are 128 bits and do not encode time order;
 * UUIDv7 is a different, time-ordered format.
 *
 * Snowflake -> 64 bits, time-ordered, generated independently on every node.
 */

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

class Snowflake {
    static const long long EPOCH = 1704067200000LL;  // 2024-01-01 UTC

    int datacenterId;
    int workerId;
    long long lastTimestamp;
    int sequence;

   public:
    Snowflake(int datacenter, int worker) {
        if (datacenter < 0 || datacenter > 31 || worker < 0 || worker > 31) {
            throw invalid_argument("worker and datacenter must be in 0..31");
        }
        datacenterId = datacenter;
        workerId = worker;
        lastTimestamp = -1;
        sequence = 0;
    }

    long long nextId(long long timestamp) {
        if (timestamp < EPOCH || timestamp - EPOCH >= (1LL << 41)) {
            throw out_of_range("timestamp exceeds the Snowflake epoch range");
        }
        if (timestamp < lastTimestamp) {
            throw invalid_argument("clock moved backward");
        }

        if (timestamp == lastTimestamp) {
            // Never invent a future wall-clock timestamp. The caller retries
            // when its actual clock advances; exhausted calls change no state.
            if (sequence == 4095) {
                throw overflow_error("millisecond capacity exhausted");
            }
            ++sequence;
        } else {
            sequence = 0;
        }

        lastTimestamp = timestamp;

        long long id = ((timestamp - EPOCH) << 22) |
                       ((long long)datacenterId << 17) |
                       ((long long)workerId << 12) | sequence;

        return id;
    }

    static void decode(long long id) {
        long long timestamp = (id >> 22) + EPOCH;
        int datacenter = (id >> 17) & 31;
        int worker = (id >> 12) & 31;
        int sequence = id & 4095;

        cout << "timestamp=" << timestamp << " datacenter=" << datacenter
             << " worker=" << worker << " sequence=" << sequence << endl;
    }
};

// Brute force: one global counter.
class CounterIds {
    long long next;

   public:
    CounterIds() {
        next = 1;
    }

    long long nextId() {
        return next++;
    }
};

int main() {
    CounterIds counter;
    cout << counter.nextId() << " " << counter.nextId() << " "
         << counter.nextId() << endl;  // 1 2 3

    Snowflake generator(3, 7);

    long long baseTime = 1704067200000LL + 1000;  // one second after the epoch

    long long first = generator.nextId(baseTime);
    long long second = generator.nextId(baseTime);
    long long third = generator.nextId(baseTime);

    cout << first << endl;
    cout << second << endl;
    cout << third << endl;

    Snowflake::decode(first);
    Snowflake::decode(third);

    cout << (first < second && second < third) << endl;  // 1 (time-ordered)

    // A different worker can generate IDs at the same millisecond.
    Snowflake otherWorker(3, 8);
    long long other = otherWorker.nextId(baseTime);
    cout << (other != first) << endl;  // 1 (worker bits differ)

    Snowflake::decode(other);

    // Move to the next millisecond: the sequence resets.
    long long nextMillisecond = generator.nextId(baseTime + 1);
    Snowflake::decode(nextMillisecond);
    cout << (nextMillisecond > third) << endl;  // 1

    // Show the bit layout of the first ID.
    cout << "binary: ";
    for (int bit = 63; bit >= 0; bit--) {
        cout << ((first >> bit) & 1);
        if (bit == 63 || bit == 22 || bit == 17 || bit == 12) {
            cout << " ";
        }
    }
    cout << endl;

    return 0;
}

/**
 * Time complexity -> O(1) per ID; exhausted capacity is rejected for caller
 * retry Space complexity -> O(1) per generator
 *
 * Special cases: more than 4096 IDs in one millisecond (reject until clock
 * advances), the clock moving backward (reject), generator restart (the
 * timestamp is behind; wait until it catches up), datacenter/worker ID
 * collisions, and running past the 41-bit timestamp range in about 69 years.
 *
 * Production notes:
 *   - get the worker ID from ZooKeeper, etcd, or Kubernetes ordinal numbers
 *   - use a monotonic clock for elapsed time, but wall-clock time for the
 *     timestamp component
 *   - a 64-bit ID fits in a signed `long long` and in a database BIGINT
 *   - IDs are unique per (timestamp, datacenter, worker, sequence)
 */

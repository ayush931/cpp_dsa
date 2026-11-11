/**
 * Write-Ahead Log (WAL)
 *
 * A WAL makes a mutation durable before it is applied to the main data
 * structure. Recovery replays the log from the last checkpoint:
 *
 *   append(record) -> assign LSN -> write to the active segment
 *   sync()         -> mark every record up to the last LSN durable
 *   recover()      -> replay valid records in LSN order
 *   checkpoint()   -> snapshot the durable state, then drop old segments
 *
 * Record layout:
 *   lsn | key | value | deleted flag | checksum
 *
 * The checksum covers the LSN, key, value, and delete flag. A torn write at the
 * tail fails the checksum. Recovery must stop at the first invalid record and
 * ignore everything after it: a later record may depend on the missing one, so
 * skipping ahead could apply an operation out of order.
 *
 * Segments:
 *   - a segment holds a bounded number of records
 *   - a full segment is immutable; the next append starts a new segment
 *   - a checkpoint can delete whole segments whose last LSN is at or below the
 *     snapshot LSN, but it must keep a partially covered segment
 *
 * Invariants:
 *   - LSNs are strictly increasing
 *   - a record is replayed only when its LSN is durable and its checksum passes
 *   - replay stops at the first torn or corrupt record
 *   - a checkpoint snapshot and its LSN are consistent
 *   - deleting a segment never loses a record newer than the snapshot LSN
 *
 * This is an in-memory model. A real WAL uses append-only files, fsync or
 * O_DSYNC for durability, a segment header with a magic number and version,
 * CRC32C checksums, and a group-commit path so many transactions share one
 * fsync. It also records the checkpoint in a separate manifest.
 */

#include <cstdint>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct Record {
    long long lsn;
    string key;
    string value;
    bool deleted;
    uint64_t checksum;
    bool torn;  // test hook: a deliberately incomplete write
};

struct Checkpoint {
    map<string, string> state;
    long long lsn;
    int removedSegments;
};

uint64_t checksumOf(long long lsn, const string &key, const string &value,
                    bool deleted) {
    uint64_t hash = 14695981039346656037ULL;
    auto mixByte = [&hash](unsigned char byte) {
        hash ^= byte;
        hash *= 1099511628211ULL;
    };

    for (int byte = 0; byte < 8; byte++) {
        mixByte(static_cast<unsigned char>((lsn >> (8 * byte)) & 0xff));
    }

    for (unsigned char byte : key) {
        mixByte(byte);
    }
    mixByte(0);
    for (unsigned char byte : value) {
        mixByte(byte);
    }
    mixByte(deleted ? 1 : 0);

    hash ^= hash >> 33;
    hash *= 0xff51afd7ed558ccdULL;
    hash ^= hash >> 33;
    hash *= 0xc4ceb9fe1a85ec53ULL;
    hash ^= hash >> 33;

    return hash;
}

class WriteAheadLog {
    int recordsPerSegment;
    vector<vector<Record>> segments;
    long long lastLsn = 0;
    long long durableLsn = 0;
    map<string, string> baseState;  // state at the last applied checkpoint
    long long baseLsn = 0;

    Record makeRecord(const string &key, const string &value, bool deleted,
                      bool torn) {
        Record record;
        record.lsn = ++lastLsn;
        record.key = key;
        record.value = value;
        record.deleted = deleted;
        record.torn = torn;
        record.checksum =
            torn ? 0 : checksumOf(record.lsn, key, value, deleted);
        return record;
    }

    void appendRecord(const Record &record) {
        if (segments.empty() ||
            static_cast<int>(segments.back().size()) >= recordsPerSegment) {
            segments.push_back({});
        }

        segments.back().push_back(record);
    }

    // Replays durable records, stopping at the first invalid one. Returns the
    // state and the last LSN that was actually applied.
    map<string, string> replay(
        const map<string, string> &base, long long fromLsn,
        long long &lastValidLsn,
        long long throughLsn = numeric_limits<long long>::max()) const {
        map<string, string> state = base;
        lastValidLsn = fromLsn;

        for (const vector<Record> &segment : segments) {
            for (const Record &record : segment) {
                if (record.lsn <= fromLsn) {
                    continue;
                }
                if (record.lsn > durableLsn || record.lsn > throughLsn) {
                    return state;
                }

                if (record.torn ||
                    record.checksum != checksumOf(record.lsn, record.key,
                                                  record.value,
                                                  record.deleted)) {
                    return state;  // stop at the first torn or corrupt record
                }

                if (record.deleted) {
                    state.erase(record.key);
                } else {
                    state[record.key] = record.value;
                }

                lastValidLsn = record.lsn;
            }
        }

        return state;
    }

   public:
    explicit WriteAheadLog(int recordsPerSegment = 4)
        : recordsPerSegment(recordsPerSegment < 1 ? 1 : recordsPerSegment) {}

    long long append(const string &key, const string &value) {
        Record record = makeRecord(key, value, false, false);
        appendRecord(record);
        return record.lsn;
    }

    long long erase(const string &key) {
        Record record = makeRecord(key, "", true, false);
        appendRecord(record);
        return record.lsn;
    }

    // Test hook: simulates a partial write at the tail. A real WAL gets this
    // from a crash in the middle of an append, not from a public API.
    long long appendTorn(const string &key, const string &value) {
        Record record = makeRecord(key, value, false, true);
        appendRecord(record);
        return record.lsn;
    }

    void sync() {
        durableLsn = lastLsn;
    }

    map<string, string> recover() const {
        long long lastValid = baseLsn;
        return replay(baseState, baseLsn, lastValid);
    }

    map<string, string> recoverFrom(const Checkpoint &checkpoint) const {
        if (checkpoint.lsn < baseLsn || checkpoint.lsn > durableLsn) {
            throw invalid_argument(
                "checkpoint is outside the retained log range");
        }
        long long lastValid = checkpoint.lsn;
        return replay(checkpoint.state, checkpoint.lsn, lastValid);
    }

    Checkpoint checkpoint() const {
        long long lastValid = baseLsn;
        Checkpoint result;
        result.state = replay(baseState, baseLsn, lastValid);
        result.lsn = lastValid;
        result.removedSegments = 0;

        // Drop only segments entirely covered by the snapshot. A segment that
        // contains a newer record must stay.
        for (const vector<Record> &segment : segments) {
            if (!segment.empty() && segment.back().lsn <= result.lsn) {
                result.removedSegments++;
            }
        }

        // checkpoint() is const for the demo, so the caller applies the
        // returned removal by constructing a new log or by calling
        // compactWith().
        return result;
    }

    void compactWith(const Checkpoint &checkpoint) {
        if (checkpoint.lsn < baseLsn || checkpoint.lsn > durableLsn) {
            throw invalid_argument(
                "checkpoint is outside the durable log range");
        }
        long long lastValid = baseLsn;
        auto expected = replay(baseState, baseLsn, lastValid, checkpoint.lsn);
        if (lastValid != checkpoint.lsn || expected != checkpoint.state) {
            throw invalid_argument(
                "checkpoint must match the valid durable prefix");
        }
        vector<vector<Record>> kept;

        for (const vector<Record> &segment : segments) {
            if (!segment.empty() && segment.back().lsn <= checkpoint.lsn) {
                continue;
            }

            kept.push_back(segment);
        }

        // Allocate the replacement snapshot before discarding any records.
        // If the copy throws, the old log and base state are still intact.
        auto snapshot = checkpoint.state;
        segments.swap(kept);
        // Removing records is safe only after their state becomes the base.
        baseState.swap(snapshot);
        baseLsn = checkpoint.lsn;
    }

    long long durable() const {
        return durableLsn;
    }

    long long last() const {
        return lastLsn;
    }

    int segmentCount() const {
        return static_cast<int>(segments.size());
    }

    int recordCount() const {
        int count = 0;

        for (const vector<Record> &segment : segments) {
            count += static_cast<int>(segment.size());
        }

        return count;
    }
};

void printState(const string &label, const map<string, string> &state) {
    cout << label << ":";

    for (const auto &[key, value] : state) {
        cout << " " << key << "=" << value;
    }

    cout << endl;
}

int main() {
    WriteAheadLog wal(2);  // two records per segment

    long long a1 = wal.append("a", "1");
    long long b2 = wal.append("b", "2");
    long long a3 = wal.append("a", "3");

    cout << a1 << " " << b2 << " " << a3 << endl;  // 1 2 3
    cout << wal.segmentCount() << " " << wal.recordCount() << endl;
    // 2 3

    // Nothing is durable until sync. Recovery ignores the unsynced tail.
    printState("before sync", wal.recover());  // before sync:
    wal.sync();
    printState("after sync", wal.recover());  // after sync: a=3 b=2

    // A later append is not durable yet.
    long long c4 = wal.append("c", "4");
    cout << c4 << " " << wal.durable() << endl;  // 4 3
    printState("crash", wal.recover());          // crash: a=3 b=2

    wal.sync();
    printState("after sync 2", wal.recover());  // after sync 2: a=3 b=2 c=4

    // Checkpoint at LSN 4 covers both segments, so both can be removed.
    Checkpoint first = wal.checkpoint();
    cout << first.lsn << " " << first.removedSegments << endl;  // 4 2
    wal.compactWith(first);
    cout << wal.segmentCount() << " " << wal.recordCount() << endl;  // 0 0
    printState("snapshot", first.state);  // snapshot: a=3 b=2 c=4

    // A new record starts a fresh segment after the checkpoint.
    long long d5 = wal.append("d", "5");
    wal.sync();
    printState("after d", wal.recoverFrom(first));
    // after d: a=3 b=2 c=4 d=5
    cout << d5 << " " << wal.segmentCount() << endl;  // 5 1

    // A torn tail stops recovery. Records after it are ignored, even if they
    // have valid checksums, because the missing record may be their dependency.
    wal.appendTorn("torn", "bad");
    wal.append("e", "6");
    wal.sync();

    printState("torn tail", wal.recoverFrom(first));
    // torn tail: a=3 b=2 c=4 d=5
    cout << wal.last() << endl;  // 7

    // A checkpoint taken with a torn tail snapshots only the valid prefix.
    Checkpoint second = wal.checkpoint();
    cout << second.lsn << " " << second.state.size() << endl;  // 5 4

    return 0;
}

/**
 * Time complexity:
 *   append -> O(1) plus O(1) checksum work over the record bytes
 *   sync -> O(1) in this model; a real fsync is O(durable bytes)
 *   recover -> O(records replayed), stopping at the first invalid record
 *   checkpoint -> O(records) to build the snapshot, O(segments) to select
 *   compactWith -> O(segments)
 * Space complexity -> O(records + snapshot) in this model
 *
 * Special cases:
 *   - no records -> recovery returns an empty state
 *   - an unsynced record -> ignored by recovery
 *   - a torn or corrupt record -> recovery stops at that point
 *   - a valid record after a torn record -> still ignored, because order
 * matters
 *   - a delete -> replayed as an erase; a later put can recreate the key
 *   - repeated updates -> the last valid value wins
 *   - a checkpoint with a torn tail -> snapshots only the valid prefix
 *   - a segment partially covered by a checkpoint -> kept intact
 *   - recordsPerSegment < 1 -> clamped to 1
 *
 * Why stop at the first bad record: a WAL is a sequence of dependent
 * operations. If record N is missing, applying record N+1 may compute a value
 * from a state that never existed. Stopping at N gives a consistent prefix; the
 * application can then decide whether to retry or fail.
 *
 * Extensions:
 *   - CRC32C instead of FNV for hardware-accelerated integrity checks
 *   - group commit: batch many appends behind one fsync
 *   - log sequence numbers in page headers for a buffer pool
 *   - a manifest that records the latest checkpoint and its segment list
 *   - compression and encryption of segments
 *   - replication of the log to a standby before acknowledging a write
 */

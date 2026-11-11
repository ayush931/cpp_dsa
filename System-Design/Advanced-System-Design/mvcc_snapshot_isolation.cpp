/**
 * MVCC and Snapshot Isolation
 *
 * A transaction reads the latest committed version <= its start snapshot,
 * plus its own staged writes. Commit rejects a write if that key changed after
 * the snapshot (first-committer-wins). All staged writes publish atomically.
 *
 * Snapshot isolation prevents these write/write conflicts but permits write
 * skew across different keys. The doctors-on-call example below demonstrates
 * why snapshot isolation is weaker than serializable isolation.
 *
 * Single-threaded, in-memory model. Staged copies make commit O(all retained
 * versions + writes*log keys); read O(log keys + log versions for one key).
 * History is unbounded; production needs active-snapshot tracking and vacuum.
 */
#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct Transaction {
    long long snapshot;
    map<string, string> writes;
    bool finished = false;
};

class SnapshotStore {
    map<string, vector<pair<long long, string>>> history;
    long long version = 0;

   public:
    Transaction begin() const {
        return Transaction{version, {}, false};
    }

    optional<string> read(const Transaction &transaction,
                          const string &key) const {
        if (transaction.finished) {
            throw logic_error("transaction already finished");
        }
        auto own = transaction.writes.find(key);
        if (own != transaction.writes.end()) {
            return own->second;
        }
        auto found = history.find(key);
        if (found == history.end()) {
            return nullopt;
        }
        const auto &versions = found->second;
        auto position =
            upper_bound(versions.begin(), versions.end(), transaction.snapshot,
                        [](long long snapshot, const auto &entry) {
                            return snapshot < entry.first;
                        });
        if (position == versions.begin()) {
            return nullopt;
        }
        return (--position)->second;
    }

    void put(Transaction &transaction, const string &key,
             const string &value) const {
        if (transaction.finished) {
            throw logic_error("transaction already finished");
        }
        transaction.writes[key] = value;
    }

    bool commit(Transaction &transaction) {
        if (transaction.finished) {
            throw logic_error("transaction already finished");
        }
        for (const auto &[key, value] : transaction.writes) {
            auto found = history.find(key);
            if (found != history.end() &&
                found->second.back().first > transaction.snapshot) {
                transaction.finished = true;
                return false;
            }
        }
        if (!transaction.writes.empty()) {
            auto staged = history;
            for (const auto &[key, value] : transaction.writes) {
                staged[key].emplace_back(version + 1, value);
            }
            history.swap(staged);
            ++version;
        }
        transaction.finished = true;
        return true;
    }
};

int main() {
    SnapshotStore database;
    auto setup = database.begin();
    database.put(setup, "alice", "on");
    database.put(setup, "bob", "on");
    assert(database.commit(setup));
    auto first = database.begin(), second = database.begin();
    assert(database.read(first, "bob") == "on" &&
           database.read(second, "alice") == "on");
    database.put(first, "alice", "off");
    database.put(second, "bob", "off");
    assert(database.commit(first));
    assert(database.read(second, "alice") == "on");  // stable old snapshot
    assert(database.commit(second));  // disjoint writes: write skew allowed
    auto current = database.begin();
    assert(database.read(current, "alice") == "off" &&
           database.read(current, "bob") == "off");
    auto left = database.begin(), right = database.begin();
    database.put(left, "alice", "on");
    database.put(right, "alice", "on");
    assert(database.commit(left) && !database.commit(right));
    cout
        << "Snapshot stability, write conflict, and write-skew checks passed\n";
}

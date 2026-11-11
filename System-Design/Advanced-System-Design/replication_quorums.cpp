/**
 * Read/Write Quorums and Partial-Write Ambiguity
 *
 * N fixed replicas, write acknowledgement threshold W, read threshold R.
 * R+W>N guarantees overlap with a completed write's replica set under these
 * assumptions: fixed membership, no lost acknowledged state, and ordered
 * versions. A model-local sequencer assigns versions; reads return the newest
 * observed value and repair reachable stale replicas.
 *
 * A failed write acknowledgement is NOT a rollback: some replicas may have
 * accepted it, and later reads can observe it. Quorum intersection by itself
 * is not consensus or a proof of linearizability. No concurrent writers,
 * elections, sloppy quorums, durable storage, or membership changes here.
 * Time O(N log keys), space O(N*keys), excluding string-copy costs.
 */
#include <cassert>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

struct VersionedValue {
    long long version;
    string value;
};

class ReplicatedStore {
    struct Replica {
        bool online = true;
        map<string, VersionedValue> values;
    };

    vector<Replica> replicas;
    int readThreshold, writeThreshold;
    long long nextVersion = 1;

   public:
    ReplicatedStore(int count, int reads, int writes)
        : readThreshold(reads), writeThreshold(writes) {
        if (count <= 0 || reads <= 0 || writes <= 0 || reads > count ||
            writes > count || static_cast<long long>(reads) + writes <= count) {
            throw invalid_argument("require 1<=R,W<=N and R+W>N");
        }
        replicas.resize(count);
    }

    void setOnline(int index, bool online) {
        replicas.at(index).online = online;
    }

    bool write(const string &key, const string &value) {
        VersionedValue update{nextVersion++, value};
        int acknowledgements = 0;
        for (auto &replica : replicas) {
            if (replica.online) {
                replica.values.insert_or_assign(key, update);
                ++acknowledgements;
            }
        }
        return acknowledgements >= writeThreshold;
    }

    optional<string> read(const string &key) {
        optional<VersionedValue> latest;
        int replies = 0;
        for (const auto &replica : replicas) {
            if (!replica.online) {
                continue;
            }
            ++replies;
            auto found = replica.values.find(key);
            if (found != replica.values.end() &&
                (!latest || found->second.version > latest->version)) {
                latest = found->second;
            }
        }
        if (replies < readThreshold) {
            throw runtime_error("read quorum unavailable");
        }
        if (!latest) {
            return nullopt;
        }
        for (auto &replica : replicas) {
            if (replica.online) {
                replica.values.insert_or_assign(key, *latest);
            }
        }
        return latest->value;
    }
};

int main() {
    ReplicatedStore store(3, 2, 2);
    store.setOnline(2, false);
    assert(store.write("item", "v1"));
    store.setOnline(0, false);
    store.setOnline(2, true);
    assert(store.read("item") ==
           "v1");  // read repairs previously offline replica
    store.setOnline(1, false);
    assert(
        !store.write("item", "uncertain"));  // one replica accepted, W not met
    store.setOnline(1, true);
    assert(store.read("item") ==
           "uncertain");  // failed ack did not erase the write
    store.setOnline(1, false);
    bool unavailable = false;
    try {
        store.read("item");
    } catch (const runtime_error &) {
        unavailable = true;
    }
    assert(unavailable);
    cout << "Quorum overlap, repair, and uncertain-write checks passed\n";
}

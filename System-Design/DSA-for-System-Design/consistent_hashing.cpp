/**
 * Consistent hashing -> distribute keys across servers so that adding or
 * removing one server moves only a small fraction of keys
 *
 * Naive hashing -> server = hash(key) % numberOfServers
 * When the server count changes, almost every key is remapped
 *
 * Consistent hashing -> place servers and keys on a hash ring (0 ... 2^64 - 1)
 * A key belongs to the first server clockwise from its position
 * Virtual nodes -> each server appears many times on the ring for balance
 *
 * Lookup -> O(log n) with a sorted map
 *
 * The hash must avalanche. A plain FNV-1a hash leaves the high bits weakly
 * mixed for structured keys such as "user:1", "user:2", which puts almost
 * every key in the same ring arc. The finalizer below spreads those bits.
 */

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

class ConsistentHash {
    map<uint64_t, string> ring;
    int virtualNodes;

    uint64_t hashValue(const string &key) {
        // FNV-1a, then a 64-bit avalanche finalizer.
        uint64_t hash = 14695981039346656037ULL;

        for (unsigned char ch : key) {
            hash ^= ch;
            hash *= 1099511628211ULL;
        }

        hash ^= hash >> 33;
        hash *= 0xff51afd7ed558ccdULL;
        hash ^= hash >> 33;
        hash *= 0xc4ceb9fe1a85ec53ULL;
        hash ^= hash >> 33;

        return hash;
    }

   public:
    ConsistentHash(int virtualNodeCount = 100) {
        virtualNodes = virtualNodeCount;
    }

    void addServer(string server) {
        for (int i = 0; i < virtualNodes; i++) {
            string nodeKey = server + "#" + to_string(i);
            ring[hashValue(nodeKey)] = server;
        }
    }

    void removeServer(string server) {
        for (int i = 0; i < virtualNodes; i++) {
            string nodeKey = server + "#" + to_string(i);
            ring.erase(hashValue(nodeKey));
        }
    }

    string getServer(const string &key) {
        if (ring.empty()) {
            return "";
        }

        uint64_t hash = hashValue(key);
        auto it = ring.lower_bound(hash);

        if (it == ring.end()) {
            it = ring.begin();  // wrap around the ring
        }

        return it->second;
    }

    int ringSize() {
        return ring.size();
    }
};

string naiveServer(const string &key, int serverCount) {
    // The same hash, but modulo the current server count: every change remaps
    // almost every key.
    uint64_t hash = 14695981039346656037ULL;

    for (unsigned char ch : key) {
        hash ^= ch;
        hash *= 1099511628211ULL;
    }

    hash ^= hash >> 33;
    hash *= 0xff51afd7ed558ccdULL;
    hash ^= hash >> 33;
    hash *= 0xc4ceb9fe1a85ec53ULL;
    hash ^= hash >> 33;

    return "server" + to_string(hash % serverCount);
}

int main() {
    vector<string> keys;

    for (int i = 0; i < 20; i++) {
        keys.push_back("user:" + to_string(i));
    }

    ConsistentHash hashRing(50);
    hashRing.addServer("server0");
    hashRing.addServer("server1");
    hashRing.addServer("server2");

    map<string, int> distribution;
    map<string, string> before;

    for (const string &key : keys) {
        before[key] = hashRing.getServer(key);
        distribution[before[key]]++;
    }

    for (auto &entry : distribution) {
        cout << entry.first << " -> " << entry.second << endl;
    }
    // server0 -> 7
    // server1 -> 7
    // server2 -> 6

    string sample = "user:3";
    string beforeServer = before[sample];

    hashRing.addServer("server3");
    string afterServer = hashRing.getServer(sample);

    cout << beforeServer << " " << afterServer << endl;  // server1 server3

    int moved = 0;

    for (const string &key : keys) {
        if (before[key] != hashRing.getServer(key)) {
            moved++;
        }
    }

    cout << "moved " << moved << " of " << keys.size()
         << endl;  // moved 4 of 20

    // Naive modulo hashing remaps most keys when the server count changes.
    cout << naiveServer("user:7", 3) << " " << naiveServer("user:7", 4) << endl;
    // server0 server3

    return 0;
}

// Special cases: empty ring, one server, server removed while keys exist,
// duplicate virtual node hashes, hot keys (use more virtual nodes or a
// bounded-load variant), replication (walk the ring N times)

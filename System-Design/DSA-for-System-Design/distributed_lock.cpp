/**
 * Lease and Fencing Simulation
 *
 * Independent in-process nodes grant TTL leases; a client requires a majority.
 * This is NOT a consensus protocol or a production Redlock implementation.
 * There is no network, persistent token allocator, restart recovery, clock
 * drift model, or proof of safety under partitions. A majority count alone
 * does not establish linearizability.
 *
 * Each acquisition gets a process-local atomic fencing token. Release and
 * renewal match BOTH owner and token, so an old handle cannot change a later
 * acquisition by the same owner. Failed acquisition rolls back only its own
 * grants. Storage rejects tokens older than its high-water mark; repeated
 * writes by the current holder may use the same token.
 *
 * Fencing rejects a stale holder after a newer token reaches storage. It does
 * not itself detect lease expiration. Real fencing needs an authoritative,
 * durable monotonic allocator and enforcement by the protected resource.
 * Time: O(nodes * log(resources)) per acquisition/release in this model.
 * Space: O(nodes * resources). Single protected storage value in this demo.
 */

#include <atomic>
#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
using namespace std;

// Monotonically increasing global counter for fencing tokens
static atomic<int64_t> g_fencing_token_sequence{100};

// Representation of an acquired lock token
struct LockHandle {
    bool acquired;
    string resource;
    string owner_id;
    int64_t fencing_token;
    chrono::milliseconds validity_time;

    LockHandle()
        : acquired(false),
          resource(""),
          owner_id(""),
          fencing_token(0),
          validity_time(0) {}
};

// A single independent Lock Server Node (e.g., a Redis or etcd instance)
class LockServerNode {
   private:
    int node_id;
    mutex mtx;

    struct LockEntry {
        string owner;
        int64_t fencing_token;
        chrono::steady_clock::time_point expires_at;
    };

    map<string, LockEntry> locks;

   public:
    explicit LockServerNode(int id) : node_id(id) {}

    int getId() const {
        return node_id;
    }

    // Attempt to acquire lock on this specific node
    bool acquire(const string &resource, const string &owner, int64_t token,
                 chrono::milliseconds ttl) {
        lock_guard<mutex> lock(mtx);
        auto now = chrono::steady_clock::now();

        auto it = locks.find(resource);
        if (it != locks.end()) {
            // Check if existing lock is expired
            if (it->second.expires_at > now) {
                return false;  // Still active and held by someone else
            }
        }

        // Grant or overwrite expired lock
        locks[resource] = LockEntry{owner, token, now + ttl};
        return true;
    }

    // Release lock safely only if the caller is the current owner
    bool release(const string &resource, const string &owner, int64_t token) {
        lock_guard<mutex> lock(mtx);
        auto it = locks.find(resource);
        if (it != locks.end() && it->second.owner == owner &&
            it->second.fencing_token == token) {
            locks.erase(it);
            return true;
        }
        return false;
    }

    // Extend lock TTL (heartbeat)
    bool renew(const string &resource, const string &owner, int64_t token,
               chrono::milliseconds ttl) {
        lock_guard<mutex> lock(mtx);
        auto now = chrono::steady_clock::now();
        auto it = locks.find(resource);
        if (ttl.count() > 0 && it != locks.end() && it->second.owner == owner &&
            it->second.fencing_token == token && it->second.expires_at > now) {
            it->second.expires_at = now + ttl;
            return true;
        }
        return false;
    }
};

// Client collecting majority lease grants across in-process nodes.
class DistributedLockClient {
   private:
    vector<shared_ptr<LockServerNode>> nodes;
    int quorum_size;

   public:
    DistributedLockClient(const vector<shared_ptr<LockServerNode>> &cluster)
        : nodes(cluster), quorum_size(cluster.size() / 2 + 1) {
        if (cluster.empty()) {
            throw invalid_argument("cluster must contain nodes");
        }
        set<int> identities;
        for (const auto &node : nodes) {
            if (!node || !identities.insert(node->getId()).second) {
                throw invalid_argument(
                    "cluster nodes need distinct identities");
            }
        }
    }

    LockHandle acquireLock(const string &resource, const string &owner,
                           chrono::milliseconds ttl) {
        LockHandle handle;
        if (resource.empty() || owner.empty() || ttl.count() <= 0) {
            return handle;
        }
        handle.resource = resource;
        handle.owner_id = owner;

        auto start_time = chrono::steady_clock::now();
        int64_t token = ++g_fencing_token_sequence;
        int granted_count = 0;

        for (auto &node : nodes) {
            if (node->acquire(resource, owner, token, ttl)) {
                granted_count++;
            }
        }

        auto elapsed = chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now() - start_time);
        auto remaining_validity = ttl - elapsed;

        // Check quorum and validity constraint
        if (granted_count >= quorum_size && remaining_validity.count() > 0) {
            handle.acquired = true;
            handle.fencing_token = token;
            handle.validity_time = remaining_validity;
            return handle;
        }

        // Failed to achieve quorum in time -> rollback / release any partial
        // grants
        for (auto &node : nodes) {
            node->release(resource, owner, token);
        }
        handle.acquired = false;
        return handle;
    }

    bool releaseLock(const LockHandle &handle) {
        if (!handle.acquired) {
            return false;
        }
        int released_count = 0;
        for (auto &node : nodes) {
            if (node->release(handle.resource, handle.owner_id,
                              handle.fencing_token)) {
                released_count++;
            }
        }
        return released_count >= quorum_size;
    }
};

// Storage Service that validates Fencing Tokens to prevent stale writes
class SharedStorageService {
   private:
    mutex mtx;
    int64_t last_fencing_token = 0;
    string stored_data = "";

   public:
    bool write(const string &data, int64_t client_token,
               const string &client_id) {
        lock_guard<mutex> lock(mtx);
        if (client_token <= 0 || client_token < last_fencing_token) {
            cout << "  [Storage REJECT] Stale write from " << client_id
                 << " (Token: " << client_token
                 << " < Last: " << last_fencing_token << ")" << endl;
            return false;
        }
        last_fencing_token = client_token;
        stored_data = data;
        cout << "  [Storage ACCEPT] Write from " << client_id << " with Token "
             << client_token << ": \"" << data << "\"" << endl;
        return true;
    }
};

int main() {
    cout << "=== Distributed Lock Manager with Fencing Tokens Demo ===" << endl;

    // Setup a 3-node distributed lock cluster (Quorum requires 2 of 3)
    vector<shared_ptr<LockServerNode>> cluster = {
        make_shared<LockServerNode>(1), make_shared<LockServerNode>(2),
        make_shared<LockServerNode>(3)};

    DistributedLockClient client(cluster);
    SharedStorageService storage;

    string resource = "account_update_mutex";
    chrono::milliseconds lock_ttl(300);  // 300 ms TTL

    cout << "\n--- Scenario 1: Successful Lock & Write ---" << endl;
    LockHandle lockA = client.acquireLock(resource, "Worker-A", lock_ttl);
    if (lockA.acquired) {
        cout << "Worker-A acquired lock! Fencing Token = "
             << lockA.fencing_token << endl;
        storage.write("Balance = $500", lockA.fencing_token, "Worker-A");
    }

    cout << "\n--- Scenario 2: Contention (Worker-B attempts while Worker-A "
            "holds lock) ---"
         << endl;
    LockHandle lockB = client.acquireLock(resource, "Worker-B", lock_ttl);
    cout << "Worker-B acquire result: "
         << (lockB.acquired ? "SUCCESS" : "REJECTED (Contention)") << endl;

    cout << "\nWorker-A releases lock..." << endl;
    client.releaseLock(lockA);

    cout << "\n--- Scenario 3: Worker-B acquires lock after release ---"
         << endl;
    lockB = client.acquireLock(resource, "Worker-B", lock_ttl);
    if (lockB.acquired) {
        cout << "Worker-B acquired lock! Fencing Token = "
             << lockB.fencing_token << endl;
        storage.write("Balance = $700", lockB.fencing_token, "Worker-B");
        client.releaseLock(lockB);
    }

    cout << "\n--- Scenario 4: Fencing Token Defense against Stale / Zombie "
            "Writes ---"
         << endl;
    // Suppose Worker-A paused for a long garbage collection cycle, and woke up
    // now trying to write with its old fencing token:
    cout
        << "Worker-A wakes up from GC pause and attempts write with old token ("
        << lockA.fencing_token << ")..." << endl;
    storage.write("Balance = $400 (Stale)", lockA.fencing_token, "Worker-A");

    return 0;
}

/**
 * Key Takeaways:
 *   - Lease TTL bounds failure modes and prevents permanent deadlocks.
 *   - Majority lease grants here simulate coordination, not consensus.
 *   - A stale handle cannot release a newer lease from the same owner.
 *   - Storage fencing depends on a newer token reaching the protected store.
 */

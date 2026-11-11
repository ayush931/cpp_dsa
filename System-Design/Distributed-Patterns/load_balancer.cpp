/**
 * Health-Aware Load Balancing
 *
 * Least-connections routing chooses a healthy backend with the smallest number
 * of active requests. Round-robin tie breaking avoids always favoring index 0.
 * A request ticket binds completion to its backend; double completion cannot
 * decrement a counter twice. Unhealthy backends receive no new requests,
 * while existing requests can finish. No healthy backend -> no ticket.
 *
 * Single-threaded in-memory model; health is supplied by a test/control plane.
 * Real routing also needs deadlines, connection draining, discovery, and
 * concurrency protection. Active requests are a proxy, not a CPU-cost measure.
 * Time: acquire O(backends + log(active requests)); complete O(log requests).
 * Space: O(backends + active requests).
 */
#include <cassert>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <vector>
using namespace std;

struct Backend {
    bool healthy = true;
    int active = 0;
};

struct RequestTicket {
    long long id;
    int backend;
};

class LoadBalancer {
    vector<Backend> backends;
    map<long long, int> activeRequests;
    long long nextId = 1;
    size_t nextTie = 0;

   public:
    explicit LoadBalancer(int count) {
        if (count <= 0) {
            throw invalid_argument("at least one backend required");
        }
        backends.resize(count);
    }

    void setHealthy(int index, bool healthy) {
        backends.at(index).healthy = healthy;
    }

    optional<RequestTicket> acquire() {
        int best = -1;
        for (size_t offset = 0; offset < backends.size(); ++offset) {
            size_t index = (nextTie + offset) % backends.size();
            if (!backends[index].healthy) {
                continue;
            }
            if (best == -1 || backends[index].active < backends[best].active) {
                best = static_cast<int>(index);
            }
        }
        if (best == -1) {
            return nullopt;
        }
        long long id = nextId++;
        activeRequests.emplace(id, best);
        ++backends[best].active;
        nextTie = (best + 1) % backends.size();
        return RequestTicket{id, best};
    }

    bool complete(long long ticketId) {
        auto found = activeRequests.find(ticketId);
        if (found == activeRequests.end()) {
            return false;
        }
        --backends[found->second].active;
        activeRequests.erase(found);
        return true;
    }
};

int main() {
    LoadBalancer balancer(2);
    auto first = balancer.acquire(), second = balancer.acquire();
    assert(first && second && first->backend != second->backend);
    balancer.setHealthy(0, false);
    auto third = balancer.acquire();
    assert(third && third->backend == 1);
    assert(balancer.complete(first->id));
    assert(!balancer.complete(first->id));
    balancer.setHealthy(1, false);
    assert(!balancer.acquire());
    balancer.setHealthy(0, true);
    assert(balancer.acquire()->backend == 0);
    cout << "Load balancing checks passed\n";
}

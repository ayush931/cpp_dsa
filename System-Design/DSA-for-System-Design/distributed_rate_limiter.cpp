/**
 * Distributed Rate Limiter
 *
 * A single-node token bucket is easy. A fleet of gateways is not: each gateway
 * can enforce its own bucket, but N gateways then allow N times the intended
 * burst. The limiter below combines two layers:
 *
 *   local token bucket  -> cheap fast-path rejection and burst shaping
 *   global sliding log  -> authoritative cluster-wide limit per client
 *
 * Every request first takes a token from the gateway's local bucket. It then
 * checks the global sliding window. If the global check rejects, the local
 * token is refunded, so a rejection never permanently consumes local capacity.
 *
 * Invariants:
 *   - a request is allowed only when both layers allow it
 *   - the global log contains at most `limit` timestamps in any window
 *   - the local bucket never holds more than `capacity` tokens
 *   - a global rejection restores exactly one local token
 *   - time is supplied by the caller; a timestamp behind the coordinator is
 *     rejected rather than allowed to refill a bucket
 *
 * This is a single-threaded model. A production implementation runs the global
 * check as one atomic Redis Lua script (or an equivalent compare-and-set loop),
 * uses a monotonic clock, and shards the key space by client id. Local buckets
 * reduce round trips but are only an optimization: the global check is the
 * source of truth.
 *
 * Edge cases:
 *   - capacity 0 or limit 0 rejects everything
 *   - a refill rate of 0 never restores tokens
 *   - a clock that moves backward must not create tokens
 *   - a window boundary is half-open: timestamps at `now - window` expire
 *   - many clients need separate global logs; this demo models one client
 */

#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>
using namespace std;

class TokenBucket {
    int capacity;
    double refillRate;
    double tokens;
    double lastRefill;

   public:
    TokenBucket(int capacity, double refillRate)
        : capacity(capacity),
          refillRate(refillRate),
          tokens(capacity),
          lastRefill(0) {}

    bool take(double now) {
        // A backward clock jump must not refill the bucket.
        if (now < lastRefill) {
            now = lastRefill;
        }

        tokens = min(static_cast<double>(capacity),
                     tokens + (now - lastRefill) * refillRate);
        lastRefill = now;

        if (tokens < 1.0) {
            return false;
        }

        tokens -= 1.0;
        return true;
    }

    void refund() {
        tokens = min(static_cast<double>(capacity), tokens + 1.0);
    }

    double available() const {
        return tokens;
    }
};

class DistributedRateLimiter {
    int globalLimit;
    double windowSeconds;
    vector<TokenBucket> nodes;
    deque<double> globalLog;
    vector<int> allowedPerNode;
    vector<int> rejectedPerNode;
    double lastGlobalTime = 0;

    void expire(double now) {
        while (!globalLog.empty() && globalLog.front() <= now - windowSeconds) {
            globalLog.pop_front();
        }
    }

   public:
    DistributedRateLimiter(int nodeCount, int capacity, double refillRate,
                           int limit, double windowSeconds)
        : globalLimit(limit), windowSeconds(windowSeconds) {
        for (int i = 0; i < nodeCount; i++) {
            nodes.push_back(TokenBucket(capacity, refillRate));
            allowedPerNode.push_back(0);
            rejectedPerNode.push_back(0);
        }
    }

    bool allow(int node, double now) {
        if (node < 0 || node >= static_cast<int>(nodes.size())) {
            return false;
        }
        if (globalLimit <= 0) {
            rejectedPerNode[node]++;
            return false;
        }

        // The coordinator owns the clock for the global window. A request whose
        // clock is behind the coordinator is rejected: accepting it could
        // refill a local bucket and keep old global timestamps alive.
        if (now < lastGlobalTime) {
            rejectedPerNode[node]++;
            return false;
        }
        lastGlobalTime = now;

        if (!nodes[node].take(now)) {
            rejectedPerNode[node]++;
            return false;
        }

        expire(now);

        if (static_cast<int>(globalLog.size()) >= globalLimit) {
            nodes[node].refund();  // the local fast path must not lose capacity
            rejectedPerNode[node]++;
            return false;
        }

        globalLog.push_back(now);
        allowedPerNode[node]++;
        return true;
    }

    int globalCount() const {
        return static_cast<int>(globalLog.size());
    }

    int allowed(int node) const {
        return allowedPerNode[node];
    }

    int rejected(int node) const {
        return rejectedPerNode[node];
    }

    double localTokens(int node) const {
        return nodes[node].available();
    }
};

int main() {
    // Three gateways, two local tokens each, slow refill, and a cluster-wide
    // limit of five requests per ten seconds.
    DistributedRateLimiter limiter(3, 2, 0.1, 5, 10);

    cout << limiter.allow(0, 0) << " " << limiter.allow(0, 0) << " "
         << limiter.allow(0, 0) << endl;
    // 1 1 0 (third request is rejected by the local bucket)

    cout << limiter.allow(1, 0) << " " << limiter.allow(1, 0) << " "
         << limiter.allow(1, 0) << endl;
    // 1 1 0

    cout << limiter.allow(2, 0) << " " << limiter.allow(2, 0) << endl;
    // 1 0 (the second request is rejected by the global window and refunded)

    cout << limiter.globalCount() << endl;  // 5
    cout << limiter.allowed(0) << " " << limiter.allowed(1) << " "
         << limiter.allowed(2) << endl;
    // 2 2 1
    cout << limiter.rejected(0) << " " << limiter.rejected(1) << " "
         << limiter.rejected(2) << endl;
    // 1 1 1

    // At t=5 the local bucket has only half a token, so the request fails
    // before the global window is consulted.
    cout << limiter.allow(0, 5) << endl;    // 0
    cout << limiter.globalCount() << endl;  // 5

    // At t=10 the t=0 timestamps expire and each local bucket has one token.
    cout << limiter.allow(0, 10) << " " << limiter.allow(1, 10) << " "
         << limiter.allow(2, 10) << endl;
    // 1 1 1
    cout << limiter.globalCount() << endl;  // 3
    cout << limiter.allowed(0) << " " << limiter.allowed(1) << " "
         << limiter.allowed(2) << endl;
    // 3 3 2

    // A second request at the same instant is limited by the local bucket.
    cout << limiter.allow(0, 10) << endl;  // 0

    // The window has fully drained by t=21; node 2 can spend its refilled
    // token.
    cout << limiter.allow(2, 21) << endl;    // 1
    cout << limiter.globalCount() << endl;   // 1
    cout << limiter.localTokens(2) << endl;  // 1

    // A timestamp behind the coordinator is rejected: it cannot create tokens.
    cout << limiter.allow(0, 5) << endl;    // 0
    cout << limiter.globalCount() << endl;  // 1

    return 0;
}

/**
 * Rate limiter -> control how many requests a client can make in a time window
 *
 * Token bucket
 * - A bucket holds up to `capacity` tokens and refills at `rate` tokens/second
 * - Every request takes one token; if the bucket is empty the request is
 * rejected
 * - Allows short bursts up to the capacity while enforcing a long-term rate
 *
 * Sliding window log (implemented below)
 * - Keep accepted timestamps in a deque and expire entries outside the window
 * - An interpolated counter is a different, approximate variant, not this log
 *
 * Brute force -> store the timestamp of every request, O(n) memory per client
 * Token bucket -> O(1) time and memory per client
 */

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class TokenBucket {
    int capacity;
    double refillRate;  // tokens per second
    double tokens;
    double lastRefillTime;

   public:
    TokenBucket(int cap, double rate) {
        if (cap < 0 || rate < 0 || !isfinite(rate)) {
            throw invalid_argument("invalid bucket policy");
        }
        capacity = cap;
        refillRate = rate;
        tokens = cap;
        lastRefillTime = 0;
    }

    bool allowRequest(double currentTime) {
        if (!isfinite(currentTime) || currentTime < lastRefillTime) {
            return false;
        }
        double elapsed = currentTime - lastRefillTime;
        tokens = min((double)capacity, tokens + elapsed * refillRate);
        lastRefillTime = currentTime;

        if (tokens >= 1) {
            tokens -= 1;
            return true;
        }

        return false;
    }

    double availableTokens() {
        return tokens;
    }
};

class SlidingWindowLog {
    int limit;
    int windowSeconds;
    unordered_map<string, deque<double>> requests;
    unordered_map<string, double> lastSeen;

   public:
    SlidingWindowLog(int lim, int window) {
        if (lim < 0 || window <= 0) {
            throw invalid_argument("invalid window policy");
        }
        limit = lim;
        windowSeconds = window;
    }

    bool allowRequest(string client, double currentTime) {
        if (!isfinite(currentTime) || currentTime < 0) {
            return false;
        }
        auto previous = lastSeen.find(client);
        if (previous != lastSeen.end() && currentTime < previous->second) {
            return false;
        }
        lastSeen[client] = currentTime;
        deque<double> &log = requests[client];

        while (!log.empty() && log.front() <= currentTime - windowSeconds) {
            log.pop_front();
        }

        if (log.size() < limit) {
            log.push_back(currentTime);
            return true;
        }

        return false;
    }
};

int main() {
    TokenBucket bucket(3, 1);  // capacity 3, refill 1 token per second

    cout << bucket.allowRequest(0.0) << endl;  // 1
    cout << bucket.allowRequest(0.1) << endl;  // 1
    cout << bucket.allowRequest(0.2) << endl;  // 1
    cout << bucket.allowRequest(0.3) << endl;  // 0 (bucket empty)

    cout << bucket.allowRequest(1.1) << endl;  // 1 (one token refilled)

    SlidingWindowLog window(2, 10);

    cout << window.allowRequest("alice", 0) << endl;   // 1
    cout << window.allowRequest("alice", 1) << endl;   // 1
    cout << window.allowRequest("alice", 2) << endl;   // 0
    cout << window.allowRequest("alice", 11) << endl;  // 1 (window moved)

    return 0;
}

// Special cases: capacity 0 (reject everything), rate 0 (never refills),
// clock going backwards, multiple clients, distributed limiting
// (use Redis + Lua for atomicity), bursts vs sustained rate
// Other algorithms: fixed window counter, leaky bucket, sliding window counter

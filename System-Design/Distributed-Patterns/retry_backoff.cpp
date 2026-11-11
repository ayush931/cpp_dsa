/**
 * Retry Policy: Exponential Backoff, Full Jitter, and a Deadline Budget
 *
 * failedAttempt is zero-based: failure of the initial call is attempt 0.
 * The maximum delay is min(cap, base*2^failedAttempt). Full jitter samples a
 * delay in [0, maximum]. Retry only a transient error AND a retry-safe action.
 * Reserve a full per-attempt timeout before admitting the next attempt.
 *
 * A timeout does not prove that a remote write failed; use idempotency keys
 * for repeatable effects. Retry at one layer and bound total retries.
 * This planner does not sleep, perform I/O, or forcibly cancel a remote call.
 * Time O(min(failedAttempt, log(cap/base))) per plan, O(1) space.
 */
#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <random>
#include <stdexcept>
using namespace std;

class RetryPolicy {
    int maximumRetries;
    long long baseDelay, maximumDelay, attemptTimeout;
    mt19937 generator;

   public:
    RetryPolicy(int retries, long long base, long long cap, long long timeout,
                unsigned seed = 42)
        : maximumRetries(retries),
          baseDelay(base),
          maximumDelay(cap),
          attemptTimeout(timeout),
          generator(seed) {
        if (retries < 0 || base <= 0 || cap < base || timeout <= 0) {
            throw invalid_argument("invalid retry policy");
        }
    }

    optional<long long> nextDelay(int failedAttempt, bool transient,
                                  bool retrySafe, long long remainingMs) {
        if (failedAttempt < 0 || failedAttempt >= maximumRetries ||
            !transient || !retrySafe || remainingMs < attemptTimeout) {
            return nullopt;
        }
        long long cap = baseDelay;
        for (int i = 0; i < failedAttempt && cap < maximumDelay; ++i) {
            cap = cap > maximumDelay / 2 ? maximumDelay : cap * 2;
        }
        long long delay =
            uniform_int_distribution<long long>(0, cap)(generator);
        if (delay > remainingMs - attemptTimeout) {
            return nullopt;
        }
        return delay;
    }
};

int main() {
    RetryPolicy policy(3, 10, 100, 50);
    auto delay = policy.nextDelay(0, true, true, 1000);
    assert(delay && *delay >= 0 && *delay <= 10);
    assert(!policy.nextDelay(3, true, true, 1000));
    assert(!policy.nextDelay(0, false, true, 1000));
    assert(!policy.nextDelay(0, true, false, 1000));
    assert(!policy.nextDelay(0, true, true, 49));
    cout << "Retry budget checks passed\n";
}

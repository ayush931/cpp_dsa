/**
 * Circuit Breaker: Closed -> Open -> HalfOpen -> Closed/Open
 *
 * Closed admits calls. Consecutive failures open the circuit. After a cooldown
 * exactly one half-open probe is admitted; success closes, failure reopens.
 * Tickets carry a generation so late results from a previous circuit state
 * cannot close a newer open circuit. A missing probe completion keeps the
 * circuit half-open: the caller must report its deadline/timeout as failure.
 *
 * Deterministic virtual milliseconds; caller time must never go backward.
 * Single-threaded model. Uses consecutive failures rather than an error-rate
 * sliding window. Each admitted ticket must complete at most once.
 * Time/space: O(1); bounded outstanding-ticket bookkeeping is caller-owned.
 */
#include <cassert>
#include <iostream>
#include <optional>
#include <stdexcept>
using namespace std;

enum class CircuitState { Closed, Open, HalfOpen };

struct CallTicket {
    unsigned long long generation;
    bool probe;
};

class CircuitBreaker {
    CircuitState state = CircuitState::Closed;
    int threshold, failures = 0;
    long long cooldown, openedAt = 0, lastTime = 0;
    unsigned long long generation = 0;

    void checkTime(long long now) {
        if (now < lastTime) {
            throw invalid_argument("clock moved backward");
        }
        lastTime = now;
    }

    void open(long long now) {
        state = CircuitState::Open;
        openedAt = now;
        ++generation;
    }

   public:
    CircuitBreaker(int failureThreshold, long long cooldownMs)
        : threshold(failureThreshold), cooldown(cooldownMs) {
        if (threshold <= 0 || cooldown <= 0) {
            throw invalid_argument("positive policy required");
        }
    }

    optional<CallTicket> acquire(long long now) {
        checkTime(now);
        if (state == CircuitState::Open) {
            if (now - openedAt < cooldown) {
                return nullopt;
            }
            state = CircuitState::HalfOpen;
            return CallTicket{generation, true};
        }
        if (state == CircuitState::HalfOpen) {
            return nullopt;
        }
        return CallTicket{generation, false};
    }

    void complete(CallTicket ticket, bool success, long long now) {
        checkTime(now);
        if (ticket.generation != generation) {
            return;
        }
        if (ticket.probe && state == CircuitState::HalfOpen) {
            if (success) {
                state = CircuitState::Closed;
                failures = 0;
                ++generation;
            } else {
                open(now);
            }
        } else if (!ticket.probe && state == CircuitState::Closed) {
            failures = success ? 0 : failures + 1;
            if (failures >= threshold) {
                open(now);
            }
        }
    }
};

int main() {
    CircuitBreaker breaker(2, 100);
    auto slow = breaker.acquire(0);
    auto first = breaker.acquire(0);
    breaker.complete(*first, false, 1);
    breaker.complete(*breaker.acquire(2), false, 3);
    assert(!breaker.acquire(50));
    breaker.complete(*slow, true, 60);  // late success cannot close the circuit
    assert(!breaker.acquire(99));
    auto probe = breaker.acquire(103);
    assert(probe && probe->probe && !breaker.acquire(104));
    breaker.complete(*probe, false, 105);
    assert(!breaker.acquire(150));
    probe = breaker.acquire(205);
    breaker.complete(*probe, true, 206);
    assert(breaker.acquire(207));
    cout << "Circuit breaker checks passed\n";
}

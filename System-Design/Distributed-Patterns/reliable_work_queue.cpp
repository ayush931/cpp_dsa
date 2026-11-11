/**
 * Bounded Work Queue: Visibility Timeout, Acknowledgement, and Dead Letters
 *
 * poll leases a ready job and assigns a fresh delivery token. If processing
 * fails, the lease expires and the job becomes visible again. ack requires
 * the current token before expiry, so an old worker cannot acknowledge a new
 * worker's delivery. After maximumAttempts expired deliveries, move to a DLQ.
 * Capacity bounds live jobs; publish fails quickly when that capacity is full.
 *
 * At-least-once delivery within this process lifetime; duplicate effects still
 * require an idempotent consumer. No persistence/network, FIFO fairness across
 * consumers, or automatic worker cancellation. Virtual nondecreasing time.
 * Time O(live jobs) per poll, O(log jobs) ack/publish; space O(live jobs +
 * DLQ). The demo retains dead letters; real systems bound/archive that storage
 * too.
 */
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

struct Delivery {
    long long jobId, token;
    string payload;
};

class ReliableQueue {
    struct Job {
        string payload;
        int attempts = 0;
        bool leased = false;
        long long token = 0, expiresAt = 0;
    };

    map<long long, Job> jobs;
    vector<string> deadLetters;
    size_t capacity;
    int maximumAttempts;
    long long visibility, lastTime = 0, nextJob = 1, nextToken = 1;

    void checkTime(long long now) {
        if (now < lastTime) {
            throw invalid_argument("clock moved backward");
        }
        lastTime = now;
    }

   public:
    ReliableQueue(size_t limit, long long visibilityMs, int attempts)
        : capacity(limit), maximumAttempts(attempts), visibility(visibilityMs) {
        if (!limit || visibility <= 0 || attempts <= 0) {
            throw invalid_argument("positive queue limits required");
        }
    }

    optional<long long> publish(const string &payload) {
        if (jobs.size() >= capacity) {
            return nullopt;
        }
        long long id = nextJob++;
        jobs.emplace(id, Job{payload});
        return id;
    }

    optional<Delivery> poll(long long now) {
        checkTime(now);
        for (auto it = jobs.begin(); it != jobs.end();) {
            Job &job = it->second;
            if (job.leased && now >= job.expiresAt) {
                job.leased = false;
                if (job.attempts >= maximumAttempts) {
                    deadLetters.push_back(job.payload);
                    it = jobs.erase(it);
                    continue;
                }
            }
            ++it;
        }
        for (auto &[id, job] : jobs) {
            if (job.leased) {
                continue;
            }
            if (now > numeric_limits<long long>::max() - visibility) {
                throw overflow_error("lease expiry overflow");
            }
            // Build a returned payload copy before changing the lease state.
            Delivery delivery{id, nextToken, job.payload};
            job.leased = true;
            job.token = nextToken++;
            job.expiresAt = now + visibility;
            ++job.attempts;
            return delivery;
        }
        return nullopt;
    }

    bool ack(long long id, long long token, long long now) {
        checkTime(now);
        auto found = jobs.find(id);
        if (found == jobs.end()) {
            return false;
        }
        const Job &job = found->second;
        if (!job.leased || job.token != token || now >= job.expiresAt) {
            return false;
        }
        jobs.erase(found);
        return true;
    }

    size_t deadLetterCount() const {
        return deadLetters.size();
    }
};

int main() {
    ReliableQueue queue(1, 10, 2);
    assert(queue.publish("send-receipt"));
    assert(!queue.publish("over-capacity"));
    auto first = queue.poll(0);
    assert(first && !queue.poll(1));
    auto redelivery = queue.poll(10);
    assert(redelivery && redelivery->jobId == first->jobId &&
           redelivery->token != first->token);
    assert(!queue.ack(first->jobId, first->token, 11));
    assert(queue.ack(redelivery->jobId, redelivery->token, 11));
    assert(!queue.ack(redelivery->jobId, redelivery->token, 11));
    assert(queue.publish("poison-job"));
    assert(queue.poll(12));
    assert(queue.poll(22));
    assert(!queue.poll(32) && queue.deadLetterCount() == 1);
    cout << "Redelivery, stale acknowledgement, capacity, and DLQ checks "
            "passed\n";
}

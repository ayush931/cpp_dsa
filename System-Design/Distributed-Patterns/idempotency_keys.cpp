/**
 * Idempotency Keys: One Charge, Repeatable Responses
 *
 * Scope a request key by customer and bind it to the original amount. Same
 * key+payload returns the saved result; same key+different payload is a
 * conflict. Balance mutation and result recording share one critical section. A
 * failed charge is cached too, so retries return its original result.
 *
 * This is an in-memory transaction model, not a payment gateway. Persistence,
 * a uniqueness constraint, and an atomic database transaction are needed to
 * preserve the guarantee across processes/crashes. Keeping the key record and
 * an external payment effect in separate stores would leave a crash window.
 * Keys have no expiry here; production retention defines a replay window.
 * Time O(log customers + log request keys), space O(customers + request keys).
 */
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>
using namespace std;

struct ChargeResult {
    long long receipt;
    bool accepted;
};

struct SavedCharge {
    long long amount;
    ChargeResult result;
};

class PaymentLedger {
    map<string, long long> balances;
    map<pair<string, string>, SavedCharge> requests;
    long long nextReceipt = 1;
    mutable mutex guard;

   public:
    void fund(const string &customer, long long amount) {
        lock_guard<mutex> lock(guard);
        if (customer.empty() || amount <= 0) {
            throw invalid_argument("positive funding required");
        }
        long long &balance = balances[customer];
        if (balance > numeric_limits<long long>::max() - amount) {
            throw overflow_error("balance overflow");
        }
        balance += amount;
    }

    ChargeResult charge(const string &customer, const string &key,
                        long long amount) {
        lock_guard<mutex> lock(guard);
        if (customer.empty() || key.empty() || amount <= 0) {
            throw invalid_argument("invalid charge");
        }
        auto identity = make_pair(customer, key);
        auto previous = requests.find(identity);
        if (previous != requests.end()) {
            if (previous->second.amount != amount) {
                throw invalid_argument("idempotency key payload conflict");
            }
            return previous->second.result;
        }
        auto account = balances.find(customer);
        bool accepted = account != balances.end() && account->second >= amount;
        if (nextReceipt == numeric_limits<long long>::max()) {
            throw overflow_error("receipt limit");
        }
        ChargeResult result{nextReceipt, accepted};
        // Allocate the replay record before changing money; no throwing work
        // follows the balance mutation in this local model.
        requests.emplace(identity, SavedCharge{amount, result});
        if (accepted) {
            account->second -= amount;
        }
        ++nextReceipt;
        return result;
    }

    long long balance(const string &customer) const {
        lock_guard<mutex> lock(guard);
        auto found = balances.find(customer);
        return found == balances.end() ? 0 : found->second;
    }
};

int main() {
    PaymentLedger ledger;
    ledger.fund("alice", 1000);
    vector<ChargeResult> results(8);
    vector<thread> callers;
    for (size_t i = 0; i < results.size(); ++i) {
        callers.emplace_back(
            [&, i] { results[i] = ledger.charge("alice", "checkout-1", 300); });
    }
    for (auto &caller : callers) {
        caller.join();
    }
    for (auto result : results) {
        assert(result.accepted && result.receipt == results[0].receipt);
    }
    assert(ledger.balance("alice") == 700);
    bool conflict = false;
    try {
        ledger.charge("alice", "checkout-1", 301);
    } catch (const invalid_argument &) {
        conflict = true;
    }
    assert(conflict);
    auto failed = ledger.charge("alice", "checkout-2", 900);
    ledger.fund("alice", 1000);
    assert(!failed.accepted &&
           !ledger.charge("alice", "checkout-2", 900).accepted);
    cout << "Concurrent retry and payload-conflict checks passed\n";
}

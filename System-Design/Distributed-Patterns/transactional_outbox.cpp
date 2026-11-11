/**
 * Transactional Outbox and Consumer Deduplication
 *
 * Commit an order and its event in ONE local transaction. A relay publishes
 * pending events, then marks them sent. If it crashes after publication but
 * before marking, it republishes; the consumer atomically records event IDs
 * together with its effect to prevent duplicate effects in its own store.
 *
 * Maps model transactions using copy-and-swap, with O(n) transaction cost.
 * This deliberately shows atomicity, not database performance or durability.
 * Real deployment needs durable transactions, retries, cleanup, and an index
 * on unsent rows. Delivery is at-least-once; arbitrary external effects are
 * NOT made exactly-once by merely recording a consumed event ID.
 */
#include <cassert>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
using namespace std;

struct OrderEvent {
    int id;
    string customer;
    bool sent = false;
};

class OrderDatabase {
    struct State {
        map<int, string> orders;
        map<int, OrderEvent> outbox;
    } state;

   public:
    bool placeOrder(int id, const string &customer,
                    bool failBeforeCommit = false) {
        if (id <= 0 || customer.empty()) {
            throw invalid_argument("invalid order");
        }
        if (state.orders.count(id)) {
            return false;
        }
        State transaction = state;
        transaction.orders.emplace(id, customer);
        transaction.outbox.emplace(id, OrderEvent{id, customer});
        if (failBeforeCommit) {
            return false;
        }
        state.orders.swap(transaction.orders);
        state.outbox.swap(transaction.outbox);
        return true;
    }

    template <typename Consumer>
    void relay(Consumer &consumer, bool crashAfterPublish = false) {
        for (auto &[id, event] : state.outbox) {
            if (event.sent) {
                continue;
            }
            consumer.consume(event);
            if (crashAfterPublish) {
                return;
            }
            event.sent = true;
        }
    }

    size_t orderCount() const {
        return state.orders.size();
    }
};

class OrderProjection {
    map<int, string> processedOrders;

   public:
    bool consume(const OrderEvent &event) {
        // Event-ID uniqueness and the local projection are one map mutation.
        // No external email or charge is performed by this model.
        return processedOrders.emplace(event.id, event.customer).second;
    }

    size_t size() const {
        return processedOrders.size();
    }
};

int main() {
    OrderDatabase database;
    OrderProjection projection;
    assert(!database.placeOrder(1, "alice", true));
    assert(database.orderCount() == 0);
    assert(database.placeOrder(1, "alice"));
    database.relay(projection, true);  // broker accepted, relay crashed
    assert(projection.size() == 1);
    database.relay(projection);  // duplicate delivery, one projection effect
    assert(projection.size() == 1);
    assert(database.placeOrder(2, "bob"));
    database.relay(projection);
    assert(database.orderCount() == 2 && projection.size() == 2);
    cout << "Atomic outbox and duplicate delivery checks passed\n";
}

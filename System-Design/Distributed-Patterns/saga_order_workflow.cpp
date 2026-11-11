/**
 * Order Saga: Forward Steps and Retryable Compensation
 *
 * Reserve inventory -> capture payment -> confirm fulfillment. A failure after
 * payment starts compensation: refund payment and release inventory. If that
 * compensation is unavailable, keep an explicit Compensating state for retry.
 * Terminal retries have no effects. Compensation is a business operation, not
 * an ACID rollback; external observers may see intermediate states.
 *
 * This sequential state-machine model keeps all service state in memory.
 * A real orchestrator persists each transition and uses idempotent participant
 * operations, an outbox, timeouts, reconciliation, and manual intervention.
 * Monetary values are integer minor units; amounts fit in long long.
 * Time O(log orders) per transition, space O(orders).
 */
#include <cassert>
#include <iostream>
#include <map>
#include <stdexcept>
using namespace std;

enum class SagaState {
    New,
    Reserved,
    Paid,
    Compensating,
    Completed,
    Cancelled
};

struct SagaOrder {
    int quantity;
    long long price;
    SagaState state = SagaState::New;
};

class OrderSaga {
    int availableStock;
    long long wallet;
    map<int, SagaOrder> orders;

   public:
    OrderSaga(int stock, long long funds)
        : availableStock(stock), wallet(funds) {
        if (stock < 0 || funds < 0) {
            throw invalid_argument("negative resources");
        }
    }

    bool start(int id, int quantity, long long totalPrice) {
        if (id <= 0 || quantity <= 0 || totalPrice <= 0) {
            throw invalid_argument("invalid order");
        }
        return orders.emplace(id, SagaOrder{quantity, totalPrice}).second;
    }

    SagaState advance(int id, bool paymentAvailable = true,
                      bool fulfillmentAccepts = true,
                      bool refundAvailable = true) {
        auto &order = orders.at(id);
        switch (order.state) {
            case SagaState::New:
                if (availableStock < order.quantity) {
                    order.state = SagaState::Cancelled;
                } else {
                    availableStock -= order.quantity;
                    order.state = SagaState::Reserved;
                }
                break;
            case SagaState::Reserved:
                if (!paymentAvailable) {
                    break;  // retry later; no payment submitted
                }
                if (wallet < order.price) {
                    availableStock += order.quantity;
                    order.state = SagaState::Cancelled;
                } else {
                    wallet -= order.price;
                    order.state = SagaState::Paid;
                }
                break;
            case SagaState::Paid:
                order.state = fulfillmentAccepts ? SagaState::Completed
                                                 : SagaState::Compensating;
                break;
            case SagaState::Compensating:
                if (!refundAvailable) {
                    break;
                }
                wallet += order.price;
                availableStock += order.quantity;
                order.state = SagaState::Cancelled;
                break;
            case SagaState::Completed:
            case SagaState::Cancelled:
                break;
        }
        return order.state;
    }

    int stock() const {
        return availableStock;
    }

    long long balance() const {
        return wallet;
    }
};

int main() {
    OrderSaga saga(3, 1000);
    assert(saga.start(1, 2, 400));
    assert(saga.advance(1) == SagaState::Reserved);
    assert(saga.advance(1, false) == SagaState::Reserved);
    assert(saga.advance(1) == SagaState::Paid);
    assert(saga.advance(1, true, false) == SagaState::Compensating);
    assert(saga.advance(1, true, true, false) == SagaState::Compensating);
    assert(saga.stock() == 1 && saga.balance() == 600);
    assert(saga.advance(1) == SagaState::Cancelled);
    assert(saga.advance(1) == SagaState::Cancelled);
    assert(saga.stock() == 3 && saga.balance() == 1000);
    assert(saga.start(2, 1, 200));
    saga.advance(2);
    saga.advance(2);
    assert(saga.advance(2) == SagaState::Completed);
    assert(saga.advance(2) == SagaState::Completed);
    assert(saga.stock() == 2 && saga.balance() == 800);
    cout << "Saga success, partial failure, and compensation retry checks "
            "passed\n";
}

/**
 * Food Delivery
 *
 * A delivery platform connects customers, restaurants, menus, carts, orders,
 * and delivery partners. The model has four important boundaries:
 *
 *   1. A cart belongs to exactly one restaurant. Mixing restaurants would make
 *      pickup, pricing, and delivery times incoherent.
 *   2. Menu items can be toggled unavailable without deleting them, so past
 *      orders keep their names and prices.
 *   3. An order is a price snapshot. Later menu changes must not rewrite an
 *      order total.
 *   4. Order status moves forward through a strict state machine, and a
 *      partner is freed only on delivery or cancellation.
 *
 * Order lifecycle:
 *   Placed -> Accepted -> Preparing -> OutForDelivery -> Delivered
 *   Placed or Accepted -> Cancelled
 *
 * Pricing:
 *   subtotal     = sum of item price * quantity
 *   delivery fee = 40, waived when subtotal >= 500
 *   discount     = coupon percentage or flat amount, capped at subtotal
 *   total        = subtotal + delivery fee - discount
 *
 * Coupons:
 *   SAVE10  -> 10% off, no minimum
 *   FLAT50  -> 50 off, minimum subtotal 400
 *
 * A failed placement leaves the cart untouched so the customer can fix the
 * problem, remove an item, or try another coupon.
 *
 * A real platform adds restaurant prep times, partner location and routing,
 * surge pricing, taxes, refunds, ratings, and idempotent payment capture. The
 * state machine and the price snapshot are the parts that must be right before
 * any of those features can be added safely.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct MenuItem {
    int id;
    string name;
    long long price;
    bool available = true;
};

struct Restaurant {
    int id;
    string name;
    map<int, MenuItem> menu;
    bool open = true;
};

struct OrderItem {
    string name;
    long long price;
    int quantity;
};

enum class OrderStatus {
    Placed,
    Accepted,
    Preparing,
    OutForDelivery,
    Delivered,
    Cancelled
};

string statusName(OrderStatus status) {
    switch (status) {
        case OrderStatus::Placed:
            return "placed";
        case OrderStatus::Accepted:
            return "accepted";
        case OrderStatus::Preparing:
            return "preparing";
        case OrderStatus::OutForDelivery:
            return "out-for-delivery";
        case OrderStatus::Delivered:
            return "delivered";
        case OrderStatus::Cancelled:
            return "cancelled";
    }

    return "unknown";
}

struct Cart {
    int restaurantId = -1;
    map<int, int> quantities;  // item id -> quantity
};

struct Order {
    int id;
    string customer;
    int restaurantId;
    string restaurantName;
    vector<OrderItem> items;
    long long subtotal = 0;
    long long deliveryFee = 0;
    long long discount = 0;
    long long total = 0;
    OrderStatus status = OrderStatus::Placed;
    int partnerId = -1;
};

struct DeliveryPartner {
    int id;
    string name;
    bool available = true;
    int activeOrders = 0;
};

struct Coupon {
    string code;
    int percent;
    long long flat;
    long long minimumSubtotal;
};

class FoodDeliverySystem {
    map<int, Restaurant> restaurants;
    map<int, DeliveryPartner> partners;
    map<string, Cart> carts;
    map<int, Order> orders;
    map<string, Coupon> coupons;
    int nextOrder = 1;

    const Coupon *findCoupon(const string &code) const {
        auto entry = coupons.find(code);
        return entry == coupons.end() ? nullptr : &entry->second;
    }

   public:
    bool addRestaurant(int id, const string &name) {
        if (restaurants.count(id)) {
            return false;
        }

        restaurants[id] = {id, name, {}, true};
        return true;
    }

    bool addMenuItem(int restaurantId, int itemId, const string &name,
                     long long price) {
        auto entry = restaurants.find(restaurantId);
        if (entry == restaurants.end()) {
            return false;
        }
        if (price <= 0) {
            return false;
        }
        if (entry->second.menu.count(itemId)) {
            return false;
        }

        entry->second.menu[itemId] = {itemId, name, price, true};
        return true;
    }

    bool setItemAvailable(int restaurantId, int itemId, bool available) {
        auto restaurant = restaurants.find(restaurantId);
        if (restaurant == restaurants.end()) {
            return false;
        }

        auto item = restaurant->second.menu.find(itemId);
        if (item == restaurant->second.menu.end()) {
            return false;
        }

        item->second.available = available;
        return true;
    }

    bool setRestaurantOpen(int restaurantId, bool open) {
        auto entry = restaurants.find(restaurantId);
        if (entry == restaurants.end()) {
            return false;
        }

        entry->second.open = open;
        return true;
    }

    bool addPartner(int id, const string &name) {
        if (partners.count(id)) {
            return false;
        }

        partners[id] = {id, name, true, 0};
        return true;
    }

    bool addCoupon(const string &code, int percent, long long flat,
                   long long minimumSubtotal) {
        if (code.empty() || coupons.count(code)) {
            return false;
        }
        if (percent < 0 || percent > 100) {
            return false;
        }
        if (flat < 0) {
            return false;
        }
        if (percent == 0 && flat == 0) {
            return false;
        }

        coupons[code] = {code, percent, flat, minimumSubtotal};
        return true;
    }

    // Adds an item to the customer's cart. Rejects a different restaurant.
    bool addToCart(const string &customer, int restaurantId, int itemId,
                   int quantity = 1) {
        if (quantity <= 0) {
            return false;
        }

        auto restaurant = restaurants.find(restaurantId);
        if (restaurant == restaurants.end()) {
            return false;
        }

        auto item = restaurant->second.menu.find(itemId);
        if (item == restaurant->second.menu.end()) {
            return false;
        }

        Cart &cart = carts[customer];

        if (!cart.quantities.empty() && cart.restaurantId != restaurantId) {
            return false;  // one restaurant per cart
        }

        cart.restaurantId = restaurantId;
        cart.quantities[itemId] += quantity;

        return true;
    }

    bool removeFromCart(const string &customer, int itemId) {
        auto entry = carts.find(customer);
        if (entry == carts.end()) {
            return false;
        }

        Cart &cart = entry->second;

        auto item = cart.quantities.find(itemId);
        if (item == cart.quantities.end()) {
            return false;
        }

        cart.quantities.erase(item);

        if (cart.quantities.empty()) {
            cart.restaurantId = -1;
        }

        return true;
    }

    long long cartSubtotal(const string &customer) const {
        auto entry = carts.find(customer);
        if (entry == carts.end()) {
            return 0;
        }

        const Cart &cart = entry->second;
        auto restaurant = restaurants.find(cart.restaurantId);

        if (restaurant == restaurants.end()) {
            return 0;
        }

        long long subtotal = 0;

        for (const auto &[itemId, quantity] : cart.quantities) {
            auto item = restaurant->second.menu.find(itemId);
            if (item == restaurant->second.menu.end()) {
                continue;
            }

            subtotal += item->second.price * quantity;
        }

        return subtotal;
    }

    // Returns the order id, or -1 when the cart is empty, the restaurant is
    // closed, an item is unavailable, or the coupon is invalid.
    int placeOrder(const string &customer, const string &couponCode = "") {
        auto cartEntry = carts.find(customer);

        if (cartEntry == carts.end() || cartEntry->second.quantities.empty()) {
            return -1;
        }

        Cart &cart = cartEntry->second;

        auto restaurantEntry = restaurants.find(cart.restaurantId);
        if (restaurantEntry == restaurants.end()) {
            return -1;
        }

        Restaurant &restaurant = restaurantEntry->second;

        if (!restaurant.open) {
            return -1;
        }

        vector<OrderItem> items;
        long long subtotal = 0;

        for (const auto &[itemId, quantity] : cart.quantities) {
            auto itemEntry = restaurant.menu.find(itemId);

            if (itemEntry == restaurant.menu.end()) {
                return -1;
            }
            if (!itemEntry->second.available) {
                return -1;
            }

            items.push_back(
                {itemEntry->second.name, itemEntry->second.price, quantity});

            subtotal += itemEntry->second.price * quantity;
        }

        long long discount = 0;

        if (!couponCode.empty()) {
            const Coupon *coupon = findCoupon(couponCode);

            if (coupon == nullptr) {
                return -1;
            }
            if (subtotal < coupon->minimumSubtotal) {
                return -1;
            }

            if (coupon->percent > 0) {
                discount = subtotal * coupon->percent / 100;
            } else {
                discount = coupon->flat;
            }

            discount = min(discount, subtotal);
        }

        long long deliveryFee = subtotal >= 500 ? 0 : 40;
        long long total = subtotal + deliveryFee - discount;

        int id = nextOrder++;

        orders[id] = {id,
                      customer,
                      restaurant.id,
                      restaurant.name,
                      items,
                      subtotal,
                      deliveryFee,
                      discount,
                      total,
                      OrderStatus::Placed,
                      -1};

        carts.erase(customer);

        return id;
    }

    // Picks an available partner. A partner is available only when it has no
    // active order, so this takes the first free one.
    int assignPartner(int orderId) {
        auto entry = orders.find(orderId);
        if (entry == orders.end()) {
            return -1;
        }

        Order &order = entry->second;

        if (order.status != OrderStatus::Placed) {
            return -1;
        }

        DeliveryPartner *chosen = nullptr;

        for (auto &[id, partner] : partners) {
            if (!partner.available) {
                continue;
            }

            if (chosen == nullptr ||
                partner.activeOrders < chosen->activeOrders) {
                chosen = &partner;
            }
        }

        if (chosen == nullptr) {
            return -1;
        }

        chosen->available = false;
        chosen->activeOrders++;
        order.partnerId = chosen->id;
        order.status = OrderStatus::Accepted;

        return chosen->id;
    }

    // Moves an order one step forward. Returns false for an invalid transition.
    bool advance(int orderId) {
        auto entry = orders.find(orderId);
        if (entry == orders.end()) {
            return false;
        }

        Order &order = entry->second;

        if (order.status == OrderStatus::Accepted) {
            order.status = OrderStatus::Preparing;
            return true;
        }

        if (order.status == OrderStatus::Preparing) {
            order.status = OrderStatus::OutForDelivery;
            return true;
        }

        if (order.status == OrderStatus::OutForDelivery) {
            order.status = OrderStatus::Delivered;
            releasePartner(order.partnerId);
            return true;
        }

        return false;
    }

    // Cancellation is allowed before pickup. Returns false otherwise.
    bool cancel(int orderId) {
        auto entry = orders.find(orderId);
        if (entry == orders.end()) {
            return false;
        }

        Order &order = entry->second;

        if (order.status != OrderStatus::Placed &&
            order.status != OrderStatus::Accepted) {
            return false;
        }

        order.status = OrderStatus::Cancelled;
        releasePartner(order.partnerId);

        return true;
    }

    void releasePartner(int partnerId) {
        auto entry = partners.find(partnerId);
        if (entry == partners.end()) {
            return;
        }

        DeliveryPartner &partner = entry->second;

        if (partner.activeOrders > 0) {
            partner.activeOrders--;
        }
        partner.available = partner.activeOrders == 0;
    }

    string status(int orderId) const {
        auto entry = orders.find(orderId);
        return entry == orders.end() ? "unknown"
                                     : statusName(entry->second.status);
    }

    long long total(int orderId) const {
        auto entry = orders.find(orderId);
        return entry == orders.end() ? -1 : entry->second.total;
    }

    void printCart(const string &customer) const {
        auto entry = carts.find(customer);

        if (entry == carts.end() || entry->second.quantities.empty()) {
            cout << customer << " cart empty" << endl;
            return;
        }

        const Cart &cart = entry->second;
        const Restaurant &restaurant = restaurants.at(cart.restaurantId);

        cout << customer << " cart from " << restaurant.name << ":";

        for (const auto &[itemId, quantity] : cart.quantities) {
            auto item = restaurant.menu.find(itemId);
            if (item == restaurant.menu.end()) {
                continue;
            }

            cout << " " << item->second.name << "x" << quantity;
        }

        cout << " subtotal=" << cartSubtotal(customer) << endl;
    }

    void printOrder(int orderId) const {
        auto entry = orders.find(orderId);

        if (entry == orders.end()) {
            cout << orderId << " unknown" << endl;
            return;
        }

        const Order &order = entry->second;

        cout << "O" << order.id << " " << order.customer << " "
             << order.restaurantName << " subtotal=" << order.subtotal
             << " delivery=" << order.deliveryFee
             << " discount=" << order.discount << " total=" << order.total
             << " " << statusName(order.status);

        if (order.partnerId != -1) {
            cout << " partner=" << order.partnerId;
        }

        cout << endl;
    }
};

int main() {
    FoodDeliverySystem system;

    system.addRestaurant(1, "Spice House");
    system.addRestaurant(2, "Sweet Corner");

    system.addMenuItem(1, 11, "Paneer Tikka", 220);
    system.addMenuItem(1, 12, "Naan", 40);
    system.addMenuItem(1, 13, "Biryani", 300);
    system.addMenuItem(2, 21, "Cake", 150);

    cout << system.addRestaurant(1, "Duplicate") << endl;       // 0
    cout << system.addMenuItem(1, 11, "Duplicate", 1) << endl;  // 0
    cout << system.addMenuItem(9, 91, "Ghost", 100) << endl;    // 0

    system.addPartner(1, "Ravi");
    system.addPartner(2, "Mira");

    cout << system.addCoupon("SAVE10", 10, 0, 0) << endl;    // 1
    cout << system.addCoupon("FLAT50", 0, 50, 400) << endl;  // 1
    cout << system.addCoupon("SAVE10", 10, 0, 0) << endl;    // 0
    cout << system.addCoupon("BAD", 0, 0, 0) << endl;        // 0

    // Asha builds a cart from Spice House: 220 + 2 * 40 = 300.
    cout << system.addToCart("Asha", 1, 11) << endl;     // 1
    cout << system.addToCart("Asha", 1, 12, 2) << endl;  // 1
    system.printCart("Asha");                            // ... subtotal=300

    // A cart cannot mix restaurants.
    cout << system.addToCart("Asha", 2, 21) << endl;  // 0

    // Remove and re-add an item.
    cout << system.removeFromCart("Asha", 12) << endl;   // 1
    cout << system.removeFromCart("Asha", 12) << endl;   // 0
    cout << system.addToCart("Asha", 1, 12, 2) << endl;  // 1

    // SAVE10: 10% of 300 = 30; delivery 40; total 310.
    int first = system.placeOrder("Asha", "SAVE10");
    cout << first << endl;  // 1
    system.printOrder(first);
    // O1 Asha Spice House subtotal=300 delivery=40 discount=30 total=310 placed

    // The cart is cleared after a successful order.
    system.printCart("Asha");  // Asha cart empty

    // Assign the least-loaded partner.
    cout << system.assignPartner(first) << endl;  // 1
    system.printOrder(first);                     // ... accepted partner=1

    // A second order waits for another partner if the first is busy.
    system.addToCart("Ben", 1, 13, 1);
    int second = system.placeOrder("Ben");
    cout << second << endl;                        // 2
    cout << system.assignPartner(second) << endl;  // 2
    system.printOrder(second);                     // ... accepted partner=2

    // A third order finds no free partner.
    system.addToCart("Chen", 1, 13, 1);
    int third = system.placeOrder("Chen");
    cout << third << endl;                        // 3
    cout << system.assignPartner(third) << endl;  // -1

    // Advance the first order through the state machine.
    cout << system.advance(first) << endl;  // 1 (preparing)
    cout << system.advance(first) << endl;  // 1 (out-for-delivery)
    system.printOrder(first);               // ... out-for-delivery partner=1

    // It cannot be cancelled after pickup.
    cout << system.cancel(first) << endl;  // 0

    // Deliver it and free Ravi.
    cout << system.advance(first) << endl;  // 1 (delivered)
    system.printOrder(first);               // ... delivered partner=1

    // Ravi is free again, so the third order can now be assigned.
    cout << system.assignPartner(third) << endl;  // 1

    // Cancel the second order before pickup; Mira is freed.
    cout << system.cancel(second) << endl;  // 1
    system.printOrder(second);              // ... cancelled partner=2

    // Cancelling twice fails.
    cout << system.cancel(second) << endl;  // 0

    // An order that has not been accepted cannot advance.
    system.addToCart("Dev", 1, 12, 1);
    int fourth = system.placeOrder("Dev");
    cout << fourth << " " << system.advance(fourth) << endl;  // 4 0

    // FLAT50 requires a subtotal of at least 400.
    system.addToCart("Eve", 1, 11, 1);                   // 220
    cout << system.placeOrder("Eve", "FLAT50") << endl;  // -1
    system.printCart("Eve");                             // cart preserved

    // An unknown coupon also fails and preserves the cart.
    cout << system.placeOrder("Eve", "NOPE") << endl;  // -1

    // The cart can be fixed by adding more food: 220 + 3 * 40 = 340, still
    // short.
    system.addToCart("Eve", 1, 12, 3);
    cout << system.placeOrder("Eve", "FLAT50") << endl;  // -1

    // Add a biryani: 340 + 300 = 640, now the coupon applies and delivery is
    // free.
    system.addToCart("Eve", 1, 13, 1);
    int fifth = system.placeOrder("Eve", "FLAT50");
    cout << fifth << endl;  // 5
    system.printOrder(fifth);
    // subtotal=640 delivery=0 discount=50 total=590

    // Empty cart.
    cout << system.placeOrder("Nobody") << endl;  // -1

    // A closed restaurant rejects new orders but keeps its menu.
    system.setRestaurantOpen(1, false);
    system.addToCart("Farah", 1, 11, 1);
    cout << system.placeOrder("Farah") << endl;  // -1
    system.setRestaurantOpen(1, true);
    cout << system.placeOrder("Farah") << endl;  // 6

    // An unavailable item blocks the order.
    system.setItemAvailable(1, 11, false);
    system.addToCart("Gita", 1, 11, 1);
    cout << system.placeOrder("Gita") << endl;  // -1
    system.setItemAvailable(1, 11, true);
    cout << system.placeOrder("Gita") << endl;  // 7

    // Free delivery starts exactly at 500: two biryanis cost 600.
    system.addToCart("Hari", 1, 13, 2);
    int eighth = system.placeOrder("Hari");
    system.printOrder(eighth);
    // subtotal=600 delivery=0 discount=0 total=600

    // Invalid quantities and ids.
    cout << system.addToCart("Ira", 1, 11, 0) << endl;    // 0
    cout << system.addToCart("Ira", 1, 999, 1) << endl;   // 0
    cout << system.addToCart("Ira", 999, 11, 1) << endl;  // 0
    cout << system.removeFromCart("Ira", 11) << endl;     // 0

    return 0;
}

/**
 * Time complexity:
 *   addToCart / removeFromCart -> O(log restaurants + log menu + log cart)
 *   cartSubtotal -> O(items in cart)
 *   placeOrder -> O(items in cart + log coupons)
 *   assignPartner -> O(partners)
 *   advance / cancel -> O(log orders + log partners)
 * Space complexity -> O(restaurants + menu items + carts + orders + partners)
 *
 * Special cases:
 *   - empty cart -> order rejected
 *   - cart from a different restaurant -> item rejected
 *   - unknown restaurant or item -> rejected
 *   - quantity <= 0 -> rejected
 *   - closed restaurant -> order rejected
 *   - unavailable item -> order rejected
 *   - invalid coupon or unmet minimum -> order rejected and the cart is kept
 *   - a percentage discount is capped at the subtotal
 *   - delivery is free at subtotal >= 500
 *   - a cart is cleared only after a successful order
 *   - no free partner -> assignPartner returns -1 and the order stays Placed
 *   - an order cannot advance before a partner accepts it
 *   - cancellation is allowed only before pickup
 *   - delivery or cancellation frees the partner
 *   - a partner takes one order at a time and becomes available again only
 *     when that order is delivered or cancelled
 *   - menu changes after an order do not change the order snapshot
 *
 * Why the price snapshot matters: if an order stores only item ids, a later
 * price change or a deleted item corrupts the order history and the payment
 * amount. Storing name and price at placement keeps the order immutable.
 *
 * Why the one-restaurant cart rule matters: a single delivery partner cannot
 * pick up from two kitchens at once, and a combined bill would split the
 * delivery fee and prep time incorrectly. The platform can still support
 * multi-restaurant orders later by splitting them into one sub-order per
 * restaurant under a parent order.
 *
 * Extensions:
 *   - per-restaurant prep time and promised delivery time
 *   - partner location, routing, and batching
 *   - item options and modifiers (size, spice level)
 *   - taxes, tips, and payment capture
 *   - refunds with a partial-cancellation policy
 *   - ratings and restaurant availability schedules
 */

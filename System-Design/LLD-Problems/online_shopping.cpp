/**
 * Online Shopping
 *
 * A small storefront with products, carts, coupons, orders, and stock
 * reservation.
 *
 * The critical inventory rule:
 *   stock is reserved at checkout, not when an item is added to the cart
 *
 * Adding to a cart is only an intent. Another shopper can buy the last unit
 * before checkout, so checkout must revalidate every quantity and decrement
 * stock atomically. If any item is short, the whole checkout fails and no stock
 * is touched.
 *
 * Order lifecycle:
 *   Created -> Paid -> Shipped -> Delivered
 *   Created or Paid -> Cancelled
 *
 * Cancellation restores stock. Once an order is shipped, cancellation is not
 * allowed in this model; a real store would use a return flow instead.
 *
 * Pricing:
 *   subtotal  = sum of price * quantity
 *   shipping  = 50, free when subtotal >= 999
 *   discount  = coupon percentage or flat amount, capped at subtotal
 *   total     = subtotal + shipping - discount
 *
 * Coupons:
 *   SAVE10  -> 10% off, no minimum
 *   FLAT100 -> 100 off, minimum subtotal 1000
 *
 * Orders store a snapshot of names and prices. Later price changes or product
 * edits do not rewrite an existing order.
 *
 * A production system adds payment capture, idempotency keys, reservations
 * with a timeout for unpaid orders, warehouse allocation, tax, returns, and a
 * stock ledger that records every movement instead of only the current count.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct Product {
    int id;
    string name;
    long long price;
    int stock;
};

struct Cart {
    map<int, int> quantities;  // product id -> quantity
};

struct OrderItem {
    int productId;
    string name;
    long long price;
    int quantity;
};

enum class OrderStatus { Created, Paid, Shipped, Delivered, Cancelled };

string statusName(OrderStatus status) {
    switch (status) {
        case OrderStatus::Created:
            return "created";
        case OrderStatus::Paid:
            return "paid";
        case OrderStatus::Shipped:
            return "shipped";
        case OrderStatus::Delivered:
            return "delivered";
        case OrderStatus::Cancelled:
            return "cancelled";
    }

    return "unknown";
}

struct Order {
    int id;
    string user;
    vector<OrderItem> items;
    long long subtotal = 0;
    long long shipping = 0;
    long long discount = 0;
    long long total = 0;
    OrderStatus status = OrderStatus::Created;
};

struct Coupon {
    string code;
    int percent;
    long long flat;
    long long minimumSubtotal;
};

class ShoppingSystem {
    map<int, Product> products;
    map<string, Cart> carts;
    map<int, Order> orders;
    map<string, Coupon> coupons;
    int nextOrder = 1;

    const Coupon *findCoupon(const string &code) const {
        auto entry = coupons.find(code);
        return entry == coupons.end() ? nullptr : &entry->second;
    }

   public:
    bool addProduct(int id, const string &name, long long price, int stock) {
        if (products.count(id)) {
            return false;
        }
        if (price <= 0 || stock < 0) {
            return false;
        }

        products[id] = {id, name, price, stock};
        return true;
    }

    bool restock(int productId, int quantity) {
        if (quantity <= 0) {
            return false;
        }

        auto entry = products.find(productId);
        if (entry == products.end()) {
            return false;
        }

        entry->second.stock += quantity;
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

    // Adding to the cart checks current stock but does not reserve it.
    bool addToCart(const string &user, int productId, int quantity = 1) {
        if (quantity <= 0) {
            return false;
        }

        auto product = products.find(productId);
        if (product == products.end()) {
            return false;
        }
        if (product->second.stock < quantity) {
            return false;
        }

        carts[user].quantities[productId] += quantity;
        return true;
    }

    bool removeFromCart(const string &user, int productId) {
        auto entry = carts.find(user);
        if (entry == carts.end()) {
            return false;
        }

        auto item = entry->second.quantities.find(productId);
        if (item == entry->second.quantities.end()) {
            return false;
        }

        entry->second.quantities.erase(item);
        return true;
    }

    long long cartSubtotal(const string &user) const {
        auto entry = carts.find(user);
        if (entry == carts.end()) {
            return 0;
        }

        long long subtotal = 0;

        for (const auto &[productId, quantity] : entry->second.quantities) {
            auto product = products.find(productId);
            if (product == products.end()) {
                continue;
            }

            subtotal += product->second.price * quantity;
        }

        return subtotal;
    }

    // Returns the order id, or -1 when the cart is empty, stock is short, or
    // the coupon is invalid. No stock is changed on failure.
    int checkout(const string &user, const string &couponCode = "") {
        auto cartEntry = carts.find(user);

        if (cartEntry == carts.end() || cartEntry->second.quantities.empty()) {
            return -1;
        }

        Cart &cart = cartEntry->second;
        vector<OrderItem> items;
        long long subtotal = 0;

        // Validate everything first.
        for (const auto &[productId, quantity] : cart.quantities) {
            auto product = products.find(productId);

            if (product == products.end()) {
                return -1;
            }
            if (product->second.stock < quantity) {
                return -1;
            }

            items.push_back({productId, product->second.name,
                             product->second.price, quantity});
            subtotal += product->second.price * quantity;
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

        long long shipping = subtotal >= 999 ? 0 : 50;
        long long total = subtotal + shipping - discount;

        // Reserve stock only after every check passed.
        for (const OrderItem &item : items) {
            products.at(item.productId).stock -= item.quantity;
        }

        int id = nextOrder++;

        orders[id] = {id,       user,     items, subtotal,
                      shipping, discount, total, OrderStatus::Created};

        carts.erase(user);

        return id;
    }

    bool pay(int orderId) {
        auto entry = orders.find(orderId);
        if (entry == orders.end()) {
            return false;
        }
        if (entry->second.status != OrderStatus::Created) {
            return false;
        }

        entry->second.status = OrderStatus::Paid;
        return true;
    }

    bool ship(int orderId) {
        auto entry = orders.find(orderId);
        if (entry == orders.end()) {
            return false;
        }
        if (entry->second.status != OrderStatus::Paid) {
            return false;
        }

        entry->second.status = OrderStatus::Shipped;
        return true;
    }

    bool deliver(int orderId) {
        auto entry = orders.find(orderId);
        if (entry == orders.end()) {
            return false;
        }
        if (entry->second.status != OrderStatus::Shipped) {
            return false;
        }

        entry->second.status = OrderStatus::Delivered;
        return true;
    }

    // Cancellation restores stock for Created and Paid orders.
    bool cancel(int orderId) {
        auto entry = orders.find(orderId);
        if (entry == orders.end()) {
            return false;
        }

        Order &order = entry->second;

        if (order.status != OrderStatus::Created &&
            order.status != OrderStatus::Paid) {
            return false;
        }

        for (const OrderItem &item : order.items) {
            auto product = products.find(item.productId);
            if (product == products.end()) {
                continue;
            }

            product->second.stock += item.quantity;
        }

        order.status = OrderStatus::Cancelled;
        return true;
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

    int stock(int productId) const {
        auto entry = products.find(productId);
        return entry == products.end() ? -1 : entry->second.stock;
    }

    void printCart(const string &user) const {
        auto entry = carts.find(user);

        if (entry == carts.end() || entry->second.quantities.empty()) {
            cout << user << " cart empty" << endl;
            return;
        }

        cout << user << " cart:";

        for (const auto &[productId, quantity] : entry->second.quantities) {
            auto product = products.find(productId);
            if (product == products.end()) {
                continue;
            }

            cout << " " << product->second.name << "x" << quantity;
        }

        cout << " subtotal=" << cartSubtotal(user) << endl;
    }

    void printOrder(int orderId) const {
        auto entry = orders.find(orderId);

        if (entry == orders.end()) {
            cout << orderId << " unknown" << endl;
            return;
        }

        const Order &order = entry->second;

        cout << "O" << order.id << " " << order.user << " items=";

        for (std::size_t i = 0; i < order.items.size(); i++) {
            if (i > 0) {
                cout << ",";
            }
            cout << order.items[i].name << "x" << order.items[i].quantity;
        }

        cout << " subtotal=" << order.subtotal << " shipping=" << order.shipping
             << " discount=" << order.discount << " total=" << order.total
             << " " << statusName(order.status) << endl;
    }
};

int main() {
    ShoppingSystem shop;

    shop.addProduct(1, "Laptop", 800, 2);
    shop.addProduct(2, "Mouse", 40, 10);
    shop.addProduct(3, "Monitor", 300, 1);

    cout << shop.addProduct(1, "Duplicate", 1, 1) << endl;         // 0
    cout << shop.addProduct(4, "Bad", 0, 1) << endl;               // 0
    cout << shop.addProduct(5, "Negative stock", 10, -1) << endl;  // 0

    cout << shop.addCoupon("SAVE10", 10, 0, 0) << endl;       // 1
    cout << shop.addCoupon("FLAT100", 0, 100, 1000) << endl;  // 1
    cout << shop.addCoupon("SAVE10", 10, 0, 0) << endl;       // 0
    cout << shop.addCoupon("BAD", 0, 0, 0) << endl;           // 0

    // Asha: laptop 800 + 2 mice at 40 = 880. SAVE10 gives 88 off; shipping 50.
    shop.addToCart("Asha", 1, 1);
    shop.addToCart("Asha", 2, 2);
    shop.printCart("Asha");  // Asha cart: Laptopx1 Mousex2 subtotal=880

    int first = shop.checkout("Asha", "SAVE10");
    cout << first << endl;  // 1
    shop.printOrder(first);
    // O1 Asha items=Laptopx1,Mousex2 subtotal=880 shipping=50 discount=88
    // total=842 created

    // Stock is reserved at checkout.
    cout << shop.stock(1) << " " << shop.stock(2) << endl;  // 1 8

    // Fulfil the order.
    cout << shop.pay(first) << " " << shop.ship(first) << " "
         << shop.deliver(first) << endl;  // 1 1 1
    cout << shop.cancel(first) << endl;   // 0 (already delivered)

    // Ben adds the last laptop to his cart. The cart does not reserve it.
    cout << shop.addToCart("Ben", 1, 1) << endl;  // 1
    cout << shop.stock(1) << endl;                // 1 (still available)

    // Chen also adds the last laptop.
    cout << shop.addToCart("Chen", 1, 1) << endl;  // 1

    // Chen checks out first and takes the unit.
    int second = shop.checkout("Chen");
    cout << second << " " << shop.stock(1) << endl;  // 2 0
    shop.printOrder(second);
    // O2 Chen items=Laptopx1 subtotal=800 shipping=50 discount=0 total=850

    // Ben's checkout fails because the stock changed after he added the item.
    cout << shop.checkout("Ben") << endl;  // -1
    shop.printCart("Ben");                 // cart preserved

    // Chen cancels before paying: stock returns.
    cout << shop.cancel(second) << endl;  // 1
    cout << shop.stock(1) << endl;        // 1

    // Ben can now check out.
    int third = shop.checkout("Ben");
    cout << third << " " << shop.stock(1) << endl;  // 3 0
    shop.printOrder(third);
    // O3 Ben items=Laptopx1 subtotal=800 shipping=50 discount=0 total=850

    // Cancel Ben's order and restore the laptop.
    cout << shop.cancel(third) << endl;  // 1
    cout << shop.stock(1) << endl;       // 1

    // FLAT100 needs subtotal 1000. Laptop alone is 800, so checkout fails and
    // the cart is kept.
    shop.addToCart("Ben", 1, 1);
    cout << shop.checkout("Ben", "FLAT100") << endl;  // -1

    // Add the only monitor: 800 + 300 = 1100. Shipping is free and the coupon
    // applies, so total = 1100 - 100 = 1000.
    shop.addToCart("Ben", 3, 1);
    int fourth = shop.checkout("Ben", "FLAT100");
    cout << fourth << endl;  // 4
    shop.printOrder(fourth);
    // O4 Ben items=Laptopx1,Monitorx1 subtotal=1100 shipping=0 discount=100
    // total=1000 created

    cout << shop.stock(1) << " " << shop.stock(3) << endl;  // 0 0

    // State transitions are strict.
    cout << shop.ship(fourth) << endl;     // 0 (not paid)
    cout << shop.pay(fourth) << endl;      // 1
    cout << shop.pay(fourth) << endl;      // 0 (already paid)
    cout << shop.ship(fourth) << endl;     // 1
    cout << shop.cancel(fourth) << endl;   // 0 (already shipped)
    cout << shop.deliver(fourth) << endl;  // 1
    shop.printOrder(fourth);               // delivered

    // Stock is gone after delivery.
    cout << shop.addToCart("Chen", 3, 1) << endl;  // 0 (out of stock)

    // Restock and buy.
    cout << shop.restock(3, 2) << endl;  // 1
    cout << shop.stock(3) << endl;       // 2
    shop.addToCart("Chen", 3, 1);

    int fifth = shop.checkout("Chen");
    cout << fifth << endl;          // 5
    cout << shop.stock(3) << endl;  // 1
    shop.pay(fifth);
    cout << shop.cancel(fifth) << endl;  // 1
    cout << shop.stock(3) << endl;       // 2

    // Invalid cart operations.
    cout << shop.addToCart("Dev", 1, 0) << endl;    // 0
    cout << shop.addToCart("Dev", 99, 1) << endl;   // 0
    cout << shop.addToCart("Dev", 1, 5) << endl;    // 0 (out of stock)
    cout << shop.removeFromCart("Dev", 1) << endl;  // 0
    cout << shop.checkout("Dev") << endl;           // -1 (empty cart)

    // Unknown coupon.
    shop.addToCart("Eve", 2, 1);
    cout << shop.checkout("Eve", "NOPE") << endl;  // -1
    shop.printCart("Eve");                         // cart preserved

    // Invalid order ids and totals.
    cout << shop.status(99) << " " << shop.total(99) << endl;  // unknown -1
    cout << shop.pay(99) << " " << shop.ship(99) << " " << shop.deliver(99)
         << " " << shop.cancel(99) << endl;
    // 0 0 0 0

    // Unknown product stock.
    cout << shop.stock(99) << endl;  // -1

    // A coupon can never make the total negative; the discount is capped at the
    // subtotal. FLAT100 on a 1000+ order is 100, well below the subtotal here.
    cout << shop.total(fourth) << endl;  // 1000

    return 0;
}

/**
 * Time complexity:
 *   addProduct / restock / addCoupon -> O(log n)
 *   addToCart / removeFromCart -> O(log products + log cart)
 *   cartSubtotal -> O(items in cart)
 *   checkout -> O(items in cart + log coupons)
 *   pay / ship / deliver / cancel -> O(log orders) plus O(items) for a cancel
 *   that restores stock
 * Space complexity -> O(products + carts + orders + coupons)
 *
 * Special cases:
 *   - duplicate product id -> rejected
 *   - non-positive price or negative stock -> rejected
 *   - quantity <= 0 -> rejected
 *   - adding a quantity greater than the current stock -> rejected
 *   - stock can change after an item is added; checkout revalidates
 *   - empty cart -> checkout rejected
 *   - any short item -> the whole checkout fails with no stock change
 *   - invalid coupon or unmet minimum -> checkout rejected and the cart kept
 *   - a cart is cleared only after a successful checkout
 *   - discount is capped at the subtotal
 *   - shipping is free at subtotal >= 999
 *   - pay only from Created; ship only from Paid; deliver only from Shipped
 *   - cancel only from Created or Paid, and restores every item
 *   - cancel after shipping -> rejected
 *   - a cancelled order cannot be paid, shipped, or cancelled again
 *   - a delivered order keeps its stock consumed
 *   - order items keep their name and price snapshot
 *
 * Why stock is reserved at checkout: a cart is not a commitment. Two shoppers
 * can hold the same last unit in their carts. Reserving at checkout, inside one
 * atomic step, is what prevents overselling. The tradeoff is that an unpaid
 * order holds stock; production systems add an expiry that cancels unpaid
 * orders and releases the reservation.
 *
 * Why the order stores a snapshot: the product row can change price, be
 * renamed, or be discontinued. The order must remain a faithful record of what
 * the customer bought and what they paid.
 *
 * Extensions:
 *   - payment capture and refunds
 *   - reservation expiry for unpaid orders
 *   - tax by jurisdiction
 *   - warehouse and shipment allocation
 *   - returns and exchanges
 *   - a stock ledger with every movement for auditing
 *   - idempotency keys so a retried checkout does not create two orders
 */

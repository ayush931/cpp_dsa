/**
 * Vending Machine
 *
 * A vending machine accepts coins, lets the buyer pick a product, returns
 * change, and dispenses the item. Two inventories matter:
 *
 *   products -> code, name, price, and remaining quantity
 *   coins    -> the machine's change float, per denomination
 *
 * Session model:
 *   - inserted coins are held separately from the float
 *   - credit is the sum of inserted coins
 *   - cancel returns every inserted coin untouched
 *   - a successful purchase moves the inserted coins into the float, pays
 *     change from the float, decrements the product, and resets the session
 *   - a failed purchase changes nothing; the buyer can add more coins, pick a
 *     different product, or cancel
 *
 * Change making uses a memoized search over {10, 5, 2, 1}. Greedy is optimal
 * for this canonical system, but the search also handles a float with gaps,
 * such as no 5s and no 1s, and reports failure instead of silently rounding.
 *
 * State is derived: credit == 0 means idle, credit > 0 means a session is in
 * progress. Keeping one source of truth avoids state/credit mismatches.
 *
 * A real machine adds a coin validator, a bill acceptor, a dispenser motor with
 * a jam sensor, a temperature controller for perishables, and a telemetry link
 * that reports low stock and coin shortages.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

const vector<int> COIN_VALUES = {10, 5, 2, 1};

struct Product {
    string code;
    string name;
    int price;
    int quantity;
};

struct PurchaseResult {
    bool success = false;
    string message;
    map<int, int> change;
};

class VendingMachine {
    map<string, Product> products;
    map<int, int> floatCoins;  // coins available for change
    map<int, int> inserted;    // coins in the current session
    int credit = 0;

    bool searchChange(int index, int remaining, map<int, int> &chosen,
                      set<pair<int, int>> &failed) const {
        if (remaining == 0) {
            return true;
        }
        if (index == static_cast<int>(COIN_VALUES.size())) {
            return false;
        }

        pair<int, int> state = {index, remaining};
        if (failed.count(state)) {
            return false;
        }

        int coin = COIN_VALUES[index];
        auto available = floatCoins.find(coin);
        int limit = available == floatCoins.end() ? 0 : available->second;
        int maximum = min(limit, remaining / coin);

        for (int count = maximum; count >= 0; count--) {
            if (searchChange(index + 1, remaining - count * coin, chosen,
                             failed)) {
                chosen[coin] = count;
                return true;
            }
        }

        failed.insert(state);
        return false;
    }

    bool makeChange(int amount, map<int, int> &change) const {
        if (amount == 0) {
            return true;
        }

        set<pair<int, int>> failed;
        return searchChange(0, amount, change, failed);
    }

   public:
    void addProduct(const string &code, const string &name, int price,
                    int quantity) {
        products[code] = {code, name, price, quantity};
    }

    void restock(const string &code, int quantity) {
        if (!products.count(code) || quantity <= 0) {
            return;
        }

        products[code].quantity += quantity;
    }

    void addFloat(int coin, int count) {
        if (count <= 0) {
            return;
        }

        for (int value : COIN_VALUES) {
            if (value == coin) {
                floatCoins[coin] += count;
                return;
            }
        }
    }

    bool insertCoin(int coin) {
        for (int value : COIN_VALUES) {
            if (value == coin) {
                inserted[coin]++;
                credit += coin;
                return true;
            }
        }

        return false;  // unknown coin
    }

    // Returns the coins handed back on cancel, or an empty map when idle.
    map<int, int> cancel() {
        map<int, int> returned = inserted;

        inserted.clear();
        credit = 0;

        return returned;
    }

    PurchaseResult select(const string &code) {
        PurchaseResult result;

        auto entry = products.find(code);

        if (entry == products.end()) {
            result.message = "unknown product";
            return result;
        }

        Product &product = entry->second;

        if (product.quantity <= 0) {
            result.message = "sold out";
            return result;
        }

        if (credit < product.price) {
            result.message = "insufficient credit";
            return result;
        }

        int changeAmount = credit - product.price;

        // The inserted coins become part of the float for this transaction, so
        // change can include the coins the buyer just inserted.
        for (const auto &[coin, count] : inserted) {
            floatCoins[coin] += count;
        }

        map<int, int> change;

        if (!makeChange(changeAmount, change)) {
            // Roll back the temporary float merge. The session stays alive.
            for (const auto &[coin, count] : inserted) {
                floatCoins[coin] -= count;
            }

            result.message = "cannot make change";
            return result;
        }

        // Commit: pay change, remove the sold item, end the session.
        for (const auto &[coin, count] : change) {
            floatCoins[coin] -= count;
        }

        product.quantity--;
        inserted.clear();
        credit = 0;

        result.success = true;
        result.message = "dispensed";
        result.change = change;

        return result;
    }

    int getCredit() const {
        return credit;
    }

    int quantity(const string &code) const {
        auto entry = products.find(code);
        return entry == products.end() ? -1 : entry->second.quantity;
    }

    int floatCount(int coin) const {
        auto entry = floatCoins.find(coin);
        return entry == floatCoins.end() ? 0 : entry->second;
    }

    void printChange(const map<int, int> &coins) const {
        if (coins.empty()) {
            cout << "none" << endl;
            return;
        }

        bool first = true;

        for (auto it = coins.rbegin(); it != coins.rend(); ++it) {
            if (it->second == 0) {
                continue;
            }
            if (!first) {
                cout << ", ";
            }
            cout << it->first << "x" << it->second;
            first = false;
        }

        cout << endl;
    }
};

int main() {
    VendingMachine machine;

    machine.addProduct("A1", "Chips", 25, 2);
    machine.addProduct("B1", "Soda", 35, 1);
    machine.addProduct("C1", "Candy", 10, 0);  // starts sold out

    machine.addFloat(10, 2);
    machine.addFloat(5, 3);
    machine.addFloat(2, 5);
    machine.addFloat(1, 5);

    cout << machine.insertCoin(3) << endl;  // 0 (unknown coin)
    cout << machine.insertCoin(1) << endl;  // 1

    // Not enough credit for chips (25).
    PurchaseResult low = machine.select("A1");
    cout << low.message << " credit=" << machine.getCredit() << endl;
    // insufficient credit credit=1

    cout << machine.insertCoin(10) << " " << machine.insertCoin(10) << " "
         << machine.insertCoin(5) << endl;  // 1 1 1
    cout << machine.getCredit() << endl;    // 26

    // 26 - 25 = 1 change. The inserted coins are merged into the float, so the
    // machine can return a 1 coin.
    PurchaseResult chips = machine.select("A1");
    cout << chips.success << " " << chips.message << endl;  // 1 dispensed
    machine.printChange(chips.change);                      // 1x1
    cout << machine.getCredit() << " " << machine.quantity("A1") << endl;
    // 0 1

    // Sold out product.
    PurchaseResult candy = machine.select("C1");
    cout << candy.message << endl;  // sold out

    // Exact payment: 35 with no change.
    machine.insertCoin(10);
    machine.insertCoin(10);
    machine.insertCoin(10);
    machine.insertCoin(5);
    PurchaseResult soda = machine.select("B1");
    cout << soda.success << " " << machine.getCredit() << " "
         << machine.quantity("B1") << endl;
    // 1 0 0
    machine.printChange(soda.change);  // none

    // Sold out after the last soda.
    machine.insertCoin(10);
    cout << machine.select("B1").message << endl;  // sold out
    cout << machine.getCredit() << endl;           // 10

    // Cancel returns the inserted coins and clears the session.
    map<int, int> cancelled = machine.cancel();
    machine.printChange(cancelled);       // 10x1
    cout << machine.getCredit() << endl;  // 0

    // Restock and buy another chip.
    machine.restock("A1", 2);
    cout << machine.quantity("A1") << endl;  // 3

    machine.insertCoin(10);
    machine.insertCoin(10);
    machine.insertCoin(5);
    PurchaseResult last = machine.select("A1");
    cout << last.success << " " << machine.quantity("A1") << endl;  // 1 2
    machine.printChange(last.change);                               // none

    // A machine whose float cannot make change: price 25, only 10 coins inside.
    VendingMachine rigid;
    rigid.addProduct("X1", "Tea", 25, 5);
    rigid.addFloat(10, 10);  // no 5, 2, or 1 coins

    rigid.insertCoin(10);
    rigid.insertCoin(10);
    rigid.insertCoin(10);

    PurchaseResult noChange = rigid.select("X1");
    cout << noChange.message << " credit=" << rigid.getCredit() << endl;
    // cannot make change credit=30
    cout << rigid.quantity("X1") << " " << rigid.floatCount(10) << endl;
    // 5 10 (nothing changed)

    // Cancel still returns the exact coins.
    map<int, int> rigidCancel = rigid.cancel();
    rigid.printChange(rigidCancel);     // 10x3
    cout << rigid.getCredit() << endl;  // 0

    // Add smaller coins and the same purchase succeeds.
    rigid.addFloat(5, 1);
    rigid.insertCoin(10);
    rigid.insertCoin(10);
    rigid.insertCoin(10);

    PurchaseResult fixed = rigid.select("X1");
    cout << fixed.success << " " << rigid.quantity("X1") << endl;  // 1 4
    rigid.printChange(fixed.change);                               // 5x1

    // A zero-credit session cannot select anything.
    cout << rigid.select("X1").message << endl;  // insufficient credit

    return 0;
}

/**
 * Time complexity:
 *   insertCoin / cancel -> O(denominations) for the coin lookup
 *   select -> O(amount^2) worst case for the memoized search, where amount is
 *   the change due
 *   restock / addFloat -> O(log products) or O(denominations)
 * Space complexity -> O(products + coins) plus a small memo during change
 *
 * Special cases:
 *   - unknown coin -> rejected and no credit is added
 *   - unknown product code -> "unknown product"
 *   - sold-out product -> "sold out", even with enough credit
 *   - insufficient credit -> the session stays alive so the buyer can add coins
 *   - exact payment -> no change
 *   - change cannot be made -> the purchase is rejected and the float is
 *     restored exactly
 *   - cancel -> every inserted coin is returned, even after a failed purchase
 *   - restocking a sold-out product makes it purchasable again
 *   - a product price of zero would always succeed; reject non-positive prices
 *     when adding products in a real implementation
 *   - a float with no small coins can fail to make change even when its total
 *     value is larger than the change amount
 *
 * Why inserted coins are merged into the float only during the transaction:
 * the machine must not spend the buyer's coins before the sale is committed.
 * Merging temporarily lets the change algorithm use those coins, which is
 * exactly what a real machine does. The rollback path removes them again when
 * the sale fails.
 *
 * Extensions:
 *   - card and mobile payments
 *   - expiry dates and temperature monitoring
 *   - a "best before" discount
 *   - telemetry for low stock and coin shortages
 *   - a refund path for a failed dispense after payment
 */

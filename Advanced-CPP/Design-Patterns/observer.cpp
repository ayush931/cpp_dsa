/**
 * Observer pattern -> one subject notifies many observers when it changes
 *
 * Subject keeps a list of callbacks and calls each one on notify()
 * Observers can subscribe and unsubscribe
 *
 * Push model -> the subject passes the new value to the observer
 * Pull model -> the subject only says "changed", the observer reads the state
 *
 * Issues:-
 * - An observer must be removed before it is destroyed (dangling callback)
 * - Notifying while modifying the observer list can invalidate iterators
 * - Reentrant notifications can cause infinite loops
 * - In multithreaded code, the observer list needs synchronization
 */

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Subject {
    unordered_map<int, function<void(const string &)>> observers;
    int nextId;

   public:
    Subject() {
        nextId = 1;
    }

    int subscribe(function<void(const string &)> callback) {
        int id = nextId++;
        observers[id] = move(callback);
        return id;
    }

    void unsubscribe(int id) {
        observers.erase(id);
    }

    void notify(const string &event) {
        // copy the callbacks so a callback can unsubscribe safely
        vector<function<void(const string &)>> snapshot;

        for (auto &entry : observers) {
            snapshot.push_back(entry.second);
        }

        for (auto &callback : snapshot) {
            callback(event);
        }
    }

    int observerCount() const {
        return observers.size();
    }
};

class StockTicker : public Subject {
    string symbol;
    double price;

   public:
    StockTicker(string s) {
        symbol = s;
        price = 0;
    }

    void setPrice(double newPrice) {
        price = newPrice;
        notify(symbol + " = " + to_string(price));
    }
};

int main() {
    StockTicker ticker("ACME");

    int first = ticker.subscribe(
        [](const string &event) { cout << "logger: " << event << endl; });

    int second = ticker.subscribe(
        [](const string &event) { cout << "alert: " << event << endl; });

    ticker.setPrice(100);

    ticker.unsubscribe(first);
    cout << ticker.observerCount() << endl;  // 1

    ticker.setPrice(105);  // only the alert observer runs

    ticker.unsubscribe(second);
    cout << ticker.observerCount() << endl;  // 0

    return 0;
}

// Special cases: unsubscribe during notify (iterate over a snapshot);
// duplicate subscriptions (use ids, as here); an observer that throws
// stops the remaining notifications unless each call is wrapped;
// an observer that outlives the subject holds a dangling reference;
// prefer weak_ptr or explicit unsubscribe in the observer's destructor

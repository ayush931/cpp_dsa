/**
 * Strategy pattern -> encapsulate interchangeable algorithms behind an
 * interface and select one at runtime
 *
 * Context holds a strategy and delegates work to it
 * Adding a new strategy does not modify the context
 *
 * Three ways to implement it:-
 * 1. Virtual functions -> classic OOP, one allocation per strategy
 * 2. std::function -> flexible, can hold lambdas, small overhead
 * 3. Templates / concepts -> static polymorphism, zero overhead, chosen at
 *    compile time (see CRTP)
 */

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class SortStrategy {
   public:
    virtual ~SortStrategy() {}

    virtual void sort(vector<int> &data) = 0;
    virtual string name() const = 0;
};

class BubbleSort : public SortStrategy {
   public:
    void sort(vector<int> &data) override {
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j + 1 < data.size() - i; j++) {
                if (data[j] > data[j + 1]) {
                    swap(data[j], data[j + 1]);
                }
            }
        }
    }

    string name() const override {
        return "bubble";
    }
};

class StdSort : public SortStrategy {
   public:
    void sort(vector<int> &data) override {
        std::sort(data.begin(), data.end());
    }

    string name() const override {
        return "std::sort";
    }
};

class Sorter {
    unique_ptr<SortStrategy> strategy;

   public:
    Sorter(unique_ptr<SortStrategy> s) {
        strategy = move(s);
    }

    void setStrategy(unique_ptr<SortStrategy> s) {
        strategy = move(s);
    }

    void run(vector<int> &data) {
        strategy->sort(data);
        cout << strategy->name() << ":";

        for (int value : data) {
            cout << " " << value;
        }
        cout << endl;
    }
};

int main() {
    vector<int> data = {5, 2, 8, 1, 9, 3};

    Sorter sorter(make_unique<BubbleSort>());
    sorter.run(data);

    sorter.setStrategy(make_unique<StdSort>());
    sorter.run(data);

    // std::function variant: no class hierarchy needed
    function<bool(int, int)> comparator = [](int a, int b) { return a > b; };

    vector<int> values = {4, 1, 7, 2};
    sort(values.begin(), values.end(), comparator);

    for (int value : values) {
        cout << value << " ";  // 7 4 2 1
    }
    cout << endl;

    // strategy chosen at runtime from a string
    string choice = "ascending";

    auto order = (choice == "ascending")
                     ? function<bool(int, int)>(less<int>())
                     : function<bool(int, int)>(greater<int>());

    sort(values.begin(), values.end(), order);

    for (int value : values) {
        cout << value << " ";  // 1 2 4 7
    }
    cout << endl;

    return 0;
}

// Special cases: an empty strategy (check before use); strategy lifetime must
// exceed the context; virtual dispatch cost in a hot loop (prefer templates);
// a strategy that needs context data (pass it as an argument or store a
// reference); prefer lambdas for one-off behavior

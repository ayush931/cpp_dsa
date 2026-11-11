/**
 * Time-based key-value store: append (timestamp,value) versions for each key.
 * get(key,t) returns the latest value with timestamp <=t, if one exists.
 * Timestamps must strictly increase PER KEY; different keys are independent.
 * upper_bound finds the first version newer than the query, then step back.
 * set: expected amortized O(1); get: expected O(log versions); O(total
 * versions) space, excluding string costs. unordered_map has linear worst-case
 * lookup. An optional distinguishes missing data from a stored empty string.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

class TimeMap {
    unordered_map<string, vector<pair<int, string>>> history;

   public:
    void set(const string &key, string value, int timestamp) {
        auto &versions = history[key];
        if (!versions.empty() && timestamp <= versions.back().first) {
            throw invalid_argument("timestamps must strictly increase per key");
        }
        versions.emplace_back(timestamp, std::move(value));
    }

    optional<string> get(const string &key, int timestamp) const {
        auto found = history.find(key);  // queries must not insert absent keys
        if (found == history.end()) {
            return nullopt;
        }
        const auto &versions = found->second;
        auto position = upper_bound(
            versions.begin(), versions.end(), timestamp,
            [](int time, const auto &version) { return time < version.first; });
        if (position == versions.begin()) {
            return nullopt;
        }
        return (--position)->second;
    }
};

int main() {
    TimeMap store;
    store.set("foo", "bar", 1);
    store.set("foo", "later", 4);
    store.set("empty", "", 2);
    assert(store.get("foo", 3) == "bar");
    assert(store.get("foo", 4) == "later");
    assert(store.get("foo", 100) == "later");
    assert(store.get("empty", 2) == "");
    assert(!store.get("foo", 0));
    assert(!store.get("missing", 5));
    bool rejected = false;
    try {
        store.set("foo", "old", 3);
    } catch (const invalid_argument &) {
        rejected = true;
    }
    assert(rejected);
    cout << *store.get("foo", 3) << '\n';  // bar
}

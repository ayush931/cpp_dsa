/**
 * Cache-Aside: TTL, Negative Caching, and Stale-Fill Prevention
 *
 * A slow reader can capture v1, then refill it after a writer commits v2 and
 * invalidates the cache. Keep a per-key version floor to reject that stale
 * refill. Deletion is a new version containing no value; an absent key can
 * also be cached briefly. An empty string is a value, not a cache miss.
 *
 * This single-threaded model delivers invalidations synchronously. A deployed
 * cache needs a reliable invalidation mechanism and atomic version checks.
 * TTL limits staleness; it does not guarantee strong consistency. Single-flight
 * loading and stampede control are separate extensions.
 * Time O(log keys), space O(keys), caller-supplied nondecreasing milliseconds.
 */
#include <cassert>
#include <iostream>
#include <limits>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
using namespace std;

struct DataVersion {
    long long version;
    optional<string> value;
};

class VersionedCache {
    struct Cached {
        DataVersion data;
        long long expiresAt;
    };

    map<string, DataVersion> database;
    map<string, Cached> cache;
    map<string, long long> minimumVersion;
    long long sequence = 0, lastTime = 0, ttl;

    void checkTime(long long now) {
        if (now < lastTime) {
            throw invalid_argument("clock moved backward");
        }
        lastTime = now;
    }

   public:
    explicit VersionedCache(long long ttlMs) : ttl(ttlMs) {
        if (ttl <= 0) {
            throw invalid_argument("TTL must be positive");
        }
    }

    void write(const string &key, optional<string> value) {
        long long version = ++sequence;
        database.insert_or_assign(key, DataVersion{version, std::move(value)});
        minimumVersion[key] = version;
        cache.erase(key);
    }

    DataVersion load(const string &key) const {
        auto found = database.find(key);
        return found == database.end() ? DataVersion{0, nullopt}
                                       : found->second;
    }

    bool populate(const string &key, const DataVersion &snapshot,
                  long long now) {
        checkTime(now);
        auto floor = minimumVersion.find(key);
        if (floor != minimumVersion.end() && snapshot.version < floor->second) {
            return false;
        }
        auto existing = cache.find(key);
        if (existing != cache.end() &&
            existing->second.data.version > snapshot.version) {
            return false;
        }
        if (now > numeric_limits<long long>::max() - ttl) {
            throw overflow_error("TTL overflow");
        }
        cache.insert_or_assign(key, Cached{snapshot, now + ttl});
        return true;
    }

    optional<string> get(const string &key, long long now) {
        checkTime(now);
        auto found = cache.find(key);
        if (found != cache.end() && now < found->second.expiresAt) {
            return found->second.data.value;
        }
        DataVersion snapshot = load(key);
        populate(key, snapshot, now);
        return snapshot.value;
    }
};

int main() {
    VersionedCache cache(10);
    cache.write("product", "v1");
    auto slowReader = cache.load("product");
    cache.write("product", "v2");
    assert(!cache.populate("product", slowReader, 0));
    assert(cache.get("product", 1) == "v2");
    cache.write("product", nullopt);
    assert(!cache.get("product", 2));
    assert(!cache.populate("product", slowReader, 3));
    assert(
        !cache.get("product", 12));  // expiry boundary, database still absent
    cache.write("product", "");
    assert(cache.get("product", 13) == "");
    cout << "Stale refill, negative caching, deletion, and TTL checks passed\n";
}

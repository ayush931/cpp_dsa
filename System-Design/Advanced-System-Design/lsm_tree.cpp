/**
 * Log-Structured Merge Tree (LSM Tree)
 *
 * An LSM tree is a write-optimized storage structure. Writes go to an
 * in-memory sorted table (the memtable) and are flushed to immutable sorted
 * files (SSTables) when the memtable fills up.
 *
 * Read path:
 *   1. check the memtable
 *   2. check SSTables from newest to oldest
 *   3. the first version found wins
 *   4. a tombstone means the key was deleted; stop searching
 *
 * Delete:
 *   - write a tombstone instead of removing data immediately
 *   - the tombstone hides older values in older SSTables
 *   - compaction drops tombstones only when no older table remains
 *
 * Compaction:
 *   - merge several SSTables into one sorted table
 *   - keep only the newest version of each key
 *   - drop tombstones if the compaction covers every older version
 *
 * Tradeoffs:
 *   - writes are fast (sequential appends, no in-place updates)
 *   - reads may touch several tables -> read amplification
 *   - compaction rewrites data -> write amplification
 *   - a bloom filter per SSTable avoids most unnecessary reads
 *   - range scans merge the memtable and all tables
 *
 * This is a simplified in-memory model. A real LSM tree adds a write-ahead log
 * for durability, block indexes, bloom filters, compression, levels, and
 * background compaction.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
using namespace std;

struct Entry {
    bool deleted;
    string value;
};

using Table = vector<pair<string, Entry>>;  // sorted by key

class LSMTree {
    size_t memtableLimit;
    map<string, Entry> memtable;
    vector<Table> tables;  // oldest first, newest last

    static bool lookup(const Table &table, const string &key, Entry &result) {
        auto it = lower_bound(
            table.begin(), table.end(), key,
            [](const pair<string, Entry> &entry, const string &target) {
                return entry.first < target;
            });

        if (it == table.end() || it->first != key) {
            return false;
        }

        result = it->second;
        return true;
    }

    void maybeFlush() {
        if (memtable.size() >= memtableLimit) {
            flush();
        }
    }

   public:
    explicit LSMTree(size_t limit = 2) : memtableLimit(limit) {
        if (limit == 0) {
            throw invalid_argument("memtable limit must be positive");
        }
    }

    void put(const string &key, const string &value) {
        memtable[key] = {false, value};
        maybeFlush();
    }

    void erase(const string &key) {
        memtable[key] = {true, ""};
        maybeFlush();
    }

    bool get(const string &key, string &value) const {
        auto memoryEntry = memtable.find(key);

        if (memoryEntry != memtable.end()) {
            if (memoryEntry->second.deleted) {
                return false;
            }

            value = memoryEntry->second.value;
            return true;
        }

        for (int i = static_cast<int>(tables.size()) - 1; i >= 0; i--) {
            Entry entry;

            if (lookup(tables[i], key, entry)) {
                if (entry.deleted) {
                    return false;
                }

                value = entry.value;
                return true;
            }
        }

        return false;
    }

    void flush() {
        if (memtable.empty()) {
            return;
        }

        Table table(memtable.begin(), memtable.end());
        tables.push_back(table);
        memtable.clear();
    }

    void compact() {
        flush();

        map<string, Entry> merged;

        // Newest table first: the first version of a key wins.
        for (int i = static_cast<int>(tables.size()) - 1; i >= 0; i--) {
            for (const auto &[key, entry] : tables[i]) {
                if (!merged.count(key)) {
                    merged[key] = entry;
                }
            }
        }

        tables.clear();

        Table result;

        for (const auto &[key, entry] : merged) {
            // The compaction covers every older table, so tombstones can go.
            if (!entry.deleted) {
                result.push_back({key, entry});
            }
        }

        if (!result.empty()) {
            tables.push_back(result);
        }
    }

    vector<pair<string, string>> scan() const {
        map<string, Entry> merged;

        // Memtable is the newest source.
        for (const auto &[key, entry] : memtable) {
            merged[key] = entry;
        }

        for (int i = static_cast<int>(tables.size()) - 1; i >= 0; i--) {
            for (const auto &[key, entry] : tables[i]) {
                if (!merged.count(key)) {
                    merged[key] = entry;
                }
            }
        }

        vector<pair<string, string>> result;

        for (const auto &[key, entry] : merged) {
            if (!entry.deleted) {
                result.push_back({key, entry.value});
            }
        }

        return result;
    }

    size_t tableCount() const {
        return tables.size();
    }

    size_t memtableSize() const {
        return memtable.size();
    }
};

void show(LSMTree &tree, const string &key) {
    string value;

    if (tree.get(key, value)) {
        cout << key << "=" << value << endl;
    } else {
        cout << key << "=missing" << endl;
    }
}

void showScan(LSMTree &tree) {
    cout << "[";

    vector<pair<string, string>> entries = tree.scan();

    for (int i = 0; i < entries.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << entries[i].first << "=" << entries[i].second;
    }

    cout << "]" << endl;
}

int main() {
    LSMTree tree(2);

    // These two puts fill the memtable and trigger a flush.
    tree.put("a", "1");
    tree.put("b", "2");

    // These two trigger a second flush with a newer version of a.
    tree.put("a", "10");
    tree.put("c", "3");

    cout << tree.tableCount() << " " << tree.memtableSize() << endl;
    // 2 0

    show(tree, "a");  // a=10 (newest table wins)
    show(tree, "b");  // b=2 (only the older table has it)
    show(tree, "c");  // c=3
    show(tree, "d");  // d=missing

    // A tombstone hides the older value.
    tree.erase("b");
    tree.flush();

    show(tree, "b");                    // b=missing
    cout << tree.tableCount() << endl;  // 3

    // Compaction keeps the newest versions and drops the tombstone.
    tree.compact();

    show(tree, "b");                    // b=missing (the old value was dropped)
    show(tree, "a");                    // a=10
    cout << tree.tableCount() << endl;  // 1
    showScan(tree);                     // [a=10, c=3]

    // A new memtable write is visible immediately and in scans.
    tree.put("d", "4");
    show(tree, "d");  // d=4
    showScan(tree);   // [a=10, c=3, d=4]

    // Overwriting a key creates a newer version that hides the SSTable value.
    tree.put("a", "100");
    show(tree, "a");  // a=100

    // Delete a key: the tombstone hides the value from older tables.
    tree.erase("d");
    show(tree, "d");  // d=missing

    // Compact everything.
    tree.compact();
    showScan(tree);  // [a=100, c=3]
    cout << tree.tableCount() << " " << tree.memtableSize() << endl;
    // 1 0

    return 0;
}

/**
 * Time complexity:
 *   put / erase -> O(log m) for the memtable map, plus O(m) for a flush
 *   get -> O(log m) for the memtable, then O(log s) per SSTable, where s is the
 *   table size; the worst case is O(number of tables * log s)
 *   scan -> O(total entries log total entries) to merge and sort
 *   compact -> O(total entries log total entries)
 * Space complexity -> O(total entries) for the memtable and SSTables
 *
 * Special cases:
 *   - a key deleted in the newest table must stop the search; continuing
 *     would resurrect an older value
 *   - a tombstone in an old table is still meaningful until compaction covers
 *     every older table; dropping it too early resurrects data
 *   - back-to-back flushes can create many small tables; compaction controls
 *     read amplification
 *   - a memtable limit of 0 would flush on every write; require limit >= 1
 *   - duplicate keys in the same table are impossible because the memtable is
 *     a map
 *   - an empty scan returns an empty vector
 *
 * Why LSM trees are popular for write-heavy workloads: appending to a log and
 * flushing a sorted table is sequential I/O, while a B-tree performs random
 * in-place writes. The cost is paid later during compaction. LevelDB,
 * RocksDB, Cassandra, HBase, and ScyllaDB all use variations of this design.
 */

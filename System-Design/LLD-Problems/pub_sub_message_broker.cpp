/**
 * Low-Level Design: Single-Process Partitioned Pub-Sub Log
 *
 * This model advances offsets on poll, before application processing. A
 * consumer failure after poll can therefore lose work (at-most-once processing
 * per group/partition). It has no durable storage, replication, consumer
 * membership/rebalancing, or exactly-once effects. See reliable_work_queue.cpp
 * for explicit acknowledgement and redelivery.
 *
 * Core Architectural Components:
 *   1. Message:
 *      Immutable unit of data containing id, topic, partition_key, payload, and
 * timestamp.
 *
 *   2. Partition:
 *      An append-only, ordered log of messages. Every message in a partition
 *      is assigned a sequential integer identifier called an "offset".
 *
 *   3. Topic:
 *      Logical category to which records are published. A topic consists of
 *      one or more partitions to allow horizontal scaling and parallel
 * consumption.
 *
 *   4. Producer:
 *      Publishes messages to topics. Uses a Partitioning Strategy (Key Hash or
 *      Round-Robin) to distribute messages across topic partitions.
 *
 *   5. Consumer & Consumer Group:
 *      - Each consumer belongs to a Consumer Group.
 *      - Within a group, each partition is consumed by exactly one consumer
 * (work distribution).
 *      - Across different groups, all groups receive their own independent copy
 * (broadcast/pub-sub).
 *      - Tracks offsets committed per consumer group per partition.
 *
 *   6. Concurrency & Thread-Safety:
 *      Thread-safe partitions with readers-writer or mutex locking for
 * high-throughput concurrent producers and consumers.
 */

#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
using namespace std;

// ============================================================================
// 1. Data Model
// ============================================================================

struct Message {
    int64_t offset;
    string key;
    string payload;
    chrono::system_clock::time_point timestamp;

    Message(int64_t off, string k, string p)
        : offset(off),
          key(std::move(k)),
          payload(std::move(p)),
          timestamp(chrono::system_clock::now()) {}
};

// ============================================================================
// 2. Partition: Thread-safe Append-Only Commit Log
// ============================================================================

class Partition {
   private:
    int partition_id;
    vector<Message> log;
    mutable mutex mtx;

   public:
    explicit Partition(int id) : partition_id(id) {}

    int getId() const {
        return partition_id;
    }

    // Append a new message to the log and assign sequential offset
    int64_t append(const string &key, const string &payload) {
        lock_guard<mutex> lock(mtx);
        int64_t offset = static_cast<int64_t>(log.size());
        log.emplace_back(offset, key, payload);
        return offset;
    }

    // Read message at a specific offset
    bool read(int64_t offset, Message &out_msg) const {
        lock_guard<mutex> lock(mtx);
        if (offset < 0 || offset >= static_cast<int64_t>(log.size())) {
            return false;  // Out of range or no new messages
        }
        out_msg = log[static_cast<size_t>(offset)];
        return true;
    }

    int64_t getLatestOffset() const {
        lock_guard<mutex> lock(mtx);
        return static_cast<int64_t>(log.size());
    }
};

// ============================================================================
// 3. Topic: Collection of Partitions
// ============================================================================

class Topic {
   private:
    string name;
    vector<shared_ptr<Partition>> partitions;

   public:
    Topic(string topic_name, int num_partitions) : name(std::move(topic_name)) {
        for (int i = 0; i < num_partitions; ++i) {
            partitions.push_back(make_shared<Partition>(i));
        }
    }

    const string &getName() const {
        return name;
    }

    int getPartitionCount() const {
        return static_cast<int>(partitions.size());
    }

    shared_ptr<Partition> getPartition(int partition_id) {
        if (partition_id >= 0 &&
            partition_id < static_cast<int>(partitions.size())) {
            return partitions[partition_id];
        }
        return nullptr;
    }
};

// ============================================================================
// 4. Broker: Central Registry and Offset Manager
// ============================================================================

class MessageBroker {
   private:
    map<string, shared_ptr<Topic>> topics;
    // Offset tracker: [group_id][topic_name][partition_id] -> committed offset
    map<string, map<string, map<int, int64_t>>> group_offsets;
    mutex broker_mtx;

   public:
    bool createTopic(const string &topic_name, int num_partitions) {
        lock_guard<mutex> lock(broker_mtx);
        if (topic_name.empty() || num_partitions <= 0) {
            return false;
        }
        if (topics.find(topic_name) != topics.end()) {
            return false;
        }
        topics[topic_name] = make_shared<Topic>(topic_name, num_partitions);
        return true;
    }

    shared_ptr<Topic> getTopic(const string &topic_name) {
        lock_guard<mutex> lock(broker_mtx);
        auto it = topics.find(topic_name);
        return (it != topics.end()) ? it->second : nullptr;
    }

    // Publish message to topic using hash partitioning on key
    int64_t publish(const string &topic_name, const string &key,
                    const string &payload) {
        auto topic = getTopic(topic_name);
        if (!topic) {
            cout << "Error: Topic " << topic_name << " not found!" << endl;
            return -1;
        }

        // Partition routing: hash(key) % partition_count
        int num_parts = topic->getPartitionCount();
        size_t hash_val = hash<string>{}(key);
        int selected_partition = static_cast<int>(hash_val % num_parts);

        auto part = topic->getPartition(selected_partition);
        int64_t offset = part->append(key, payload);
        return offset;
    }

    // Fetch message for a consumer group from a partition
    bool poll(const string &group_id, const string &topic_name,
              int partition_id, Message &out_msg) {
        auto topic = getTopic(topic_name);
        if (!topic) {
            return false;
        }
        auto part = topic->getPartition(partition_id);
        if (!part) {
            return false;
        }

        lock_guard<mutex> lock(broker_mtx);
        int64_t current_offset =
            group_offsets[group_id][topic_name][partition_id];

        if (part->read(current_offset, out_msg)) {
            // Advance committed offset
            group_offsets[group_id][topic_name][partition_id] =
                current_offset + 1;
            return true;
        }
        return false;
    }
};

int main() {
    cout << "=== Low-Level Design: Pub-Sub Message Broker Demo ===" << endl;

    MessageBroker broker;

    // 1. Create Topics with multiple partitions
    string orderTopic = "orders";
    broker.createTopic(orderTopic, 3);  // 3 partitions: 0, 1, 2
    cout << "Created topic '" << orderTopic << "' with 3 partitions." << endl;

    // 2. Producers publish messages with keys
    cout << "\n--- Publishing Messages ---" << endl;
    broker.publish(orderTopic, "user_101", "Order #1: Laptop");
    broker.publish(orderTopic, "user_202", "Order #2: Headphones");
    broker.publish(orderTopic, "user_101",
                   "Order #3: Mouse (routed to same partition as Order #1)");
    broker.publish(orderTopic, "user_303", "Order #4: Keyboard");

    // 3. Consumers from Consumer Group 'payment-service' poll messages
    cout << "\n--- Consumer Group: payment-service polling ---" << endl;
    string paymentGroup = "payment-service";
    for (int p = 0; p < 3; ++p) {
        Message msg(0, "", "");
        while (broker.poll(paymentGroup, orderTopic, p, msg)) {
            cout << "  [payment-service] Partition " << p << " | Offset "
                 << msg.offset << " | Key: " << msg.key
                 << " | Payload: " << msg.payload << endl;
        }
    }

    // 4. Independent Consumer Group 'analytics-service' also reads all messages
    // (Pub-Sub fan-out)
    cout << "\n--- Consumer Group: analytics-service polling (Fan-Out) ---"
         << endl;
    string analyticsGroup = "analytics-service";
    for (int p = 0; p < 3; ++p) {
        Message msg(0, "", "");
        while (broker.poll(analyticsGroup, orderTopic, p, msg)) {
            cout << "  [analytics-service] Partition " << p << " | Offset "
                 << msg.offset << " | Payload: " << msg.payload << endl;
        }
    }

    // 5. Subsequent poll yields no new messages until newly produced
    Message emptyMsg(0, "", "");
    bool hasMore = broker.poll(paymentGroup, orderTopic, 0, emptyMsg);
    cout << "\nChecking more messages for payment-service in partition 0: "
         << (hasMore ? "YES" : "NO (All caught up)") << endl;

    return 0;
}

/**
 * Architectural Highlights:
 *   - Scalability: Horizontally partitioned commit log per topic.
 *   - Ordering Guarantee: Strict total ordering per partition.
 *   - Decoupled Consumers: Offset tracking per consumer group enables
 * replayability and fan-out broadcasting without duplicating storage.
 */

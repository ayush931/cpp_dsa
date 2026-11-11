/**
 * Logger
 *
 * A logger has three separate responsibilities:
 *
 *   1. level filtering -> drop records below the configured severity
 *   2. fan-out        -> send each surviving record to every sink
 *   3. retention      -> keep a bounded ring of recent records for debugging
 *
 * Levels, from least to most severe:
 *   Trace < Debug < Info < Warn < Error < Fatal
 *
 * Sinks:
 *   ConsoleSink -> writes one formatted line to stdout
 *   MemorySink  -> keeps records in a vector for tests and diagnostics
 *
 * The `Sink` interface makes the output target replaceable. A file sink, a
 * syslog sink, or a network sink can be added without changing the logger.
 *
 * Retention:
 *   the logger keeps the last `capacity` records in a deque
 *   when the deque is full, the oldest record is evicted
 *   the ring is independent of the sinks, so it still works when a sink is
 *   slow or disconnected
 *
 * Ordering:
 *   every record gets a monotonically increasing sequence number
 *   the ring preserves insertion order
 *   sinks are called in registration order
 *
 * Thread safety:
 *   this implementation is single-threaded. A production logger guards the
 *   sequence counter, the ring, and the sink list with a mutex, or uses a
 *   lock-free queue and a background writer. Logging from a signal handler or
 *   a crash path needs an async-signal-safe sink instead.
 *
 * A logger should never throw from the logging path. A full disk or a broken
 * socket must not take down the application; production sinks catch errors,
 * count them, and fall back to a stderr sink.
 */

#include <deque>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
using namespace std;

enum class LogLevel { Trace, Debug, Info, Warn, Error, Fatal };

string levelName(LogLevel level) {
    switch (level) {
        case LogLevel::Trace:
            return "TRACE";
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warn:
            return "WARN";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Fatal:
            return "FATAL";
    }

    return "UNKNOWN";
}

struct LogRecord {
    long long sequence;
    LogLevel level;
    string message;
};

class Sink {
   public:
    virtual ~Sink() = default;
    virtual void write(const LogRecord &record) = 0;
};

class ConsoleSink : public Sink {
   public:
    void write(const LogRecord &record) override {
        cout << "[" << levelName(record.level) << "] " << record.message
             << endl;
    }
};

class MemorySink : public Sink {
    vector<LogRecord> records;

   public:
    void write(const LogRecord &record) override {
        records.push_back(record);
    }

    const vector<LogRecord> &entries() const {
        return records;
    }

    size_t size() const {
        return records.size();
    }

    void clear() {
        records.clear();
    }
};

class Logger {
    LogLevel threshold = LogLevel::Info;
    vector<unique_ptr<Sink>> sinks;
    deque<LogRecord> recent;
    size_t capacity;
    long long nextSequence = 1;

   public:
    explicit Logger(size_t capacity = 100) : capacity(capacity) {}

    void setLevel(LogLevel level) {
        threshold = level;
    }

    LogLevel level() const {
        return threshold;
    }

    void addSink(unique_ptr<Sink> sink) {
        if (sink) {
            sinks.push_back(std::move(sink));
        }
    }

    bool enabled(LogLevel level) const {
        return static_cast<int>(level) >= static_cast<int>(threshold);
    }

    void log(LogLevel level, const string &message) {
        if (!enabled(level)) {
            return;
        }

        LogRecord record{nextSequence++, level, message};

        if (capacity > 0) {
            recent.push_back(record);

            while (recent.size() > capacity) {
                recent.pop_front();
            }
        }

        for (unique_ptr<Sink> &sink : sinks) {
            if (sink) {
                sink->write(record);
            }
        }
    }

    void trace(const string &message) {
        log(LogLevel::Trace, message);
    }

    void debug(const string &message) {
        log(LogLevel::Debug, message);
    }

    void info(const string &message) {
        log(LogLevel::Info, message);
    }

    void warn(const string &message) {
        log(LogLevel::Warn, message);
    }

    void error(const string &message) {
        log(LogLevel::Error, message);
    }

    void fatal(const string &message) {
        log(LogLevel::Fatal, message);
    }

    const deque<LogRecord> &recentRecords() const {
        return recent;
    }

    size_t recentCount() const {
        return recent.size();
    }

    void printRecent() const {
        for (const LogRecord &record : recent) {
            cout << record.sequence << " " << levelName(record.level) << " "
                 << record.message << endl;
        }
    }
};

int main() {
    Logger logger(3);

    auto console = make_unique<ConsoleSink>();
    auto memory = make_unique<MemorySink>();

    ConsoleSink *consolePtr = console.get();
    MemorySink *memoryPtr = memory.get();

    logger.addSink(std::move(console));
    logger.addSink(std::move(memory));

    // Default level is Info, so Trace and Debug are dropped.
    logger.trace("trace dropped");
    logger.debug("debug dropped");
    logger.info("server started");
    logger.warn("disk at 80 percent");
    logger.error("request failed");

    cout << memoryPtr->size() << endl;     // 3
    cout << logger.recentCount() << endl;  // 3

    // Lower the threshold and log everything.
    logger.setLevel(LogLevel::Trace);
    logger.trace("trace kept");
    logger.debug("debug kept");

    cout << memoryPtr->size() << endl;  // 5

    // The ring keeps only the last three records.
    cout << logger.recentCount() << endl;  // 3
    logger.printRecent();
    // 3 ERROR request failed
    // 4 TRACE trace kept
    // 5 DEBUG debug kept

    // Clear one sink without affecting the other.
    memoryPtr->clear();
    cout << memoryPtr->size() << endl;  // 0

    logger.info("after clear");
    cout << memoryPtr->size() << endl;  // 1

    // A high threshold suppresses everything except Fatal.
    logger.setLevel(LogLevel::Fatal);
    logger.error("dropped");
    logger.fatal("shutting down");
    cout << memoryPtr->size() << endl;  // 2

    // Sequence numbers are monotonic even across level changes.
    const vector<LogRecord> &entries = memoryPtr->entries();

    for (const LogRecord &record : entries) {
        cout << record.sequence << " " << levelName(record.level) << " "
             << record.message << endl;
    }
    // 6 INFO after clear
    // 7 FATAL shutting down

    // A logger with capacity zero keeps no history but still writes to sinks.
    Logger noHistory(0);
    auto secondMemory = make_unique<MemorySink>();
    MemorySink *secondPtr = secondMemory.get();
    noHistory.addSink(std::move(secondMemory));

    noHistory.info("one");
    noHistory.info("two");

    cout << noHistory.recentCount() << " " << secondPtr->size() << endl;
    // 0 2

    // A disabled level never reaches a sink.
    noHistory.setLevel(LogLevel::Error);
    noHistory.info("dropped");
    cout << secondPtr->size() << endl;  // 2

    // Fatal is always enabled at the Error threshold.
    noHistory.fatal("boom");
    cout << secondPtr->size() << endl;  // 3

    // Unused pointer keeps the compiler honest about the sink lifetime.
    (void)consolePtr;

    return 0;
}

/**
 * Time complexity:
 *   log -> O(1) for the ring (amortized deque push/pop) plus O(sinks) writes
 *   addSink -> O(1) amortized
 *   printRecent -> O(capacity)
 * Space complexity -> O(capacity) for the ring plus O(records) for each
 * memory sink
 *
 * Special cases:
 *   - level below the threshold -> dropped before any sink or ring work
 *   - capacity 0 -> no recent history, but sinks still receive records
 *   - capacity smaller than the number of records -> oldest records evicted
 *   - no sinks -> records still enter the ring, so debugging still works
 *   - a null sink is ignored
 *   - clearing a memory sink does not affect the logger ring or other sinks
 *   - sequence numbers keep increasing after a sink clear or a level change
 *   - Fatal is not special in the filter; it is simply the highest level. A
 *     production logger may also abort after Fatal
 *   - exceptions from a sink are not handled here; a real logger must contain
 *     them so logging never crashes the process
 *
 * Why the ring is separate from the sinks: sinks have different retention
 * policies. A console sink keeps nothing, a file sink rotates by size, and a
 * memory sink may be cleared by a test. The logger's own ring gives a stable
 * "last N records" view regardless of which sinks are attached.
 *
 * Why levels are ordered: a threshold check is a single integer comparison.
 * The enum order is part of the contract; inserting a level in the middle
 * changes the meaning of every configured threshold.
 *
 * Extensions:
 *   - async logging with a bounded queue and a background writer
 *   - structured fields (request id, user id, latency)
 *   - JSON formatter and log rotation
 *   - per-module loggers with a shared sink list
 *   - sampling and rate limiting for high-volume paths
 *   - crash-safe logging with a preallocated buffer
 */

#!/usr/bin/env python3
"""Run system-design lessons and isolated regression harnesses in temporary files."""

import argparse
import concurrent.futures
import json
import os
from pathlib import Path
import subprocess
import tempfile


# These harnesses test failure paths that ordinary happy-path demos can miss.
# Each lesson is compiled separately so its standalone class/main names cannot
# collide with names from another lesson.
REGRESSIONS = {
    "DSA-for-System-Design/write_ahead_log.cpp": r'''
        WriteAheadLog wal(2);
        wal.append("a", "1");
        wal.append("b", "2");
        wal.sync();
        auto first = wal.checkpoint();
        wal.compactWith(first);
        assert(wal.recover() == first.state);
        wal.append("c", "3");
        wal.sync();
        auto second = wal.checkpoint();
        assert(second.state.size() == 3 && second.state.at("a") == "1");
        wal.compactWith(second);
        assert(wal.recover() == second.state);
        wal.append("unsynced", "ignored");
        assert(!wal.recover().count("unsynced"));
        wal.appendTorn("broken", "record");
        wal.append("after-broken", "ignored");
        wal.sync();
        auto recovered = wal.recover();
        assert(recovered.count("unsynced") == 1);
        assert(!recovered.count("after-broken"));
        auto forged = wal.checkpoint();
        forged.state["forged"] = "data";
        bool rejected = false;
        try { wal.compactWith(forged); }
        catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
    ''',
    "DSA-for-System-Design/distributed_lock.cpp": r'''
        auto node = std::make_shared<LockServerNode>(1);
        DistributedLockClient client({node});
        auto first = client.acquireLock("key", "same-owner", std::chrono::seconds(30));
        assert(first.acquired);
        auto failed = client.acquireLock("key", "same-owner", std::chrono::seconds(30));
        assert(!failed.acquired);
        // Failed rollback must not erase the original lease.
        assert(!client.acquireLock("key", "other", std::chrono::seconds(30)).acquired);
        assert(client.releaseLock(first));
        auto next = client.acquireLock("key", "same-owner", std::chrono::seconds(30));
        assert(next.acquired && next.fencing_token > first.fencing_token);
        assert(!client.releaseLock(first));
        assert(!node->renew("key", "same-owner", first.fencing_token, std::chrono::seconds(30)));
        assert(client.releaseLock(next));
        SharedStorageService storage;
        assert(!storage.write("invalid", 0, "same-owner"));
        assert(storage.write("one", first.fencing_token, "same-owner"));
        assert(storage.write("two", first.fencing_token, "same-owner"));
        assert(storage.write("new", next.fencing_token, "same-owner"));
        assert(!storage.write("stale", first.fencing_token, "same-owner"));
    ''',
    "DSA-for-System-Design/url_shortener.cpp": r'''
        BruteShortener shortener;
        shortener.shorten("first");
        assert(shortener.expand("!").empty());
        assert(shortener.expand("").empty());
        assert(shortener.expand("0") == "first");
        for (uint64_t value : {uint64_t(0), uint64_t(61), uint64_t(62),
                               std::numeric_limits<uint64_t>::max()}) {
            assert(fromBase62(toBase62(value)) == value);
        }
        assert(!fromBase62(std::string(100, 'z')));
    ''',
    "DSA-for-System-Design/count_min_sketch.cpp": r'''
        CountMinSketch sketch(3, 17);
        sketch.add("a", 4);
        assert(sketch.estimate("a") >= 4);
        bool rejected = false;
        try { sketch.add("a", -1); }
        catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected && sketch.estimate("a") >= 4);
        CountMinSketch single(1, 1);
        single.add("x", std::numeric_limits<long long>::max());
        rejected = false;
        try { single.add("y"); }
        catch (const std::overflow_error &) { rejected = true; }
        assert(rejected);
        rejected = false;
        try { CountMinSketch invalid(0, 1); }
        catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
    ''',
    "Advanced-System-Design/lsm_tree.cpp": r'''
        LSMTree tree(1);
        std::string value;
        tree.put("key", "old");
        tree.erase("key");
        assert(!tree.get("key", value));
        tree.compact();
        assert(!tree.get("key", value) && tree.scan().empty());
        tree.put("key", "new");
        assert(tree.get("key", value) && value == "new");
        bool rejected = false;
        try { LSMTree invalid(0); }
        catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
    ''',
    "LLD-Problems/pub_sub_message_broker.cpp": r'''
        MessageBroker broker;
        assert(!broker.createTopic("bad", 0));
        assert(!broker.createTopic("", 1));
        assert(broker.createTopic("events", 1));
        assert(broker.publish("events", "key", "payload") == 0);
        Message message(0, "", "");
        assert(broker.poll("group", "events", 0, message));
        assert(!broker.poll("group", "events", 0, message));
        assert(broker.poll("independent", "events", 0, message));
    ''',
    "DSA-for-System-Design/bloom_filter.cpp": r'''
        bool rejected = false;
        try { BloomFilter invalid(0, 2); }
        catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
        BloomFilter filter(100, 3);
        for (int i = 0; i < 100; ++i) filter.add(std::to_string(i));
        for (int i = 0; i < 100; ++i) assert(filter.contains(std::to_string(i)));
    ''',
    "DSA-for-System-Design/ring_buffer.cpp": r'''
        RingBuffer<int> empty(0);
        assert(!empty.push(1));
        bool rejected = false;
        try { empty.back(); }
        catch (const std::out_of_range &) { rejected = true; }
        assert(rejected);
        RingBuffer<int> ring(2, true);
        ring.push(1); ring.push(2); ring.push(3);
        int value = 0;
        assert(ring.pop(value) && value == 2);
        assert(ring.pop(value) && value == 3);
        assert(!ring.pop(value));
    ''',
    "DSA-for-System-Design/rate_limiter.cpp": r'''
        TokenBucket bucket(1, 1);
        assert(bucket.allowRequest(10));
        assert(!bucket.allowRequest(9));
        assert(!bucket.allowRequest(10));
        assert(bucket.allowRequest(11));
        assert(!bucket.allowRequest(std::numeric_limits<double>::quiet_NaN()));
        SlidingWindowLog log(1, 10);
        assert(log.allowRequest("client", 0));
        assert(!log.allowRequest("client", 1));
        assert(!log.allowRequest("client", 0));
        assert(log.allowRequest("client", 10));
    ''',
    "DSA-for-System-Design/snowflake_id.cpp": r'''
        Snowflake generator(1, 2);
        const long long now = 1704067200000LL + 1000;
        long long previous = -1;
        for (int i = 0; i < 4096; ++i) {
            long long next = generator.nextId(now);
            assert(next > previous);
            previous = next;
        }
        bool rejected = false;
        try { generator.nextId(now); }
        catch (const std::overflow_error &) { rejected = true; }
        assert(rejected && generator.nextId(now + 1) > previous);
        rejected = false;
        try { generator.nextId(now); }
        catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
        rejected = false;
        try { Snowflake invalid(32, 0); }
        catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
        rejected = false;
        try { generator.nextId(1704067200000LL + (1LL << 41)); }
        catch (const std::out_of_range &) { rejected = true; }
        assert(rejected);
    ''',
    "LLD-Problems/multi_level_cache.cpp": r'''
        bool rejected = false;
        try {
            CacheLevel<int, int> invalid(1, 0, std::make_unique<LRUEvictionPolicy<int>>());
        } catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
        rejected = false;
        try { CacheLevel<int, int> invalid(1, 2, nullptr); }
        catch (const std::invalid_argument &) { rejected = true; }
        assert(rejected);
    ''',
    "LLD-Problems/task_scheduler.cpp": r'''
        Scheduler scheduler;
        long long end = std::numeric_limits<long long>::max();
        scheduler.scheduleRecurring("last", end - 1, 2);
        assert(scheduler.runNext());
        assert(scheduler.pending() == 0 && !scheduler.runNext());
        Scheduler repeated;
        int id = repeated.scheduleRecurring("tick", 1, 2);
        repeated.runUntil(5);
        assert(repeated.executionCount() == 3);
        assert(repeated.cancel(id));
        repeated.runUntil(100);
        assert(repeated.executionCount() == 3);
    ''',
    "DSA-for-System-Design/leaderboard.cpp": r'''
        Leaderboard board;
        board.addScore("alice", 1000);
        board.addScore("alice", std::numeric_limits<int>::max());
        assert(board.rank("alice") == 1);
        assert(board.topK(-1).empty());
        board.addScore("bob", 1000);
        assert(board.rank("bob") == 1);
    ''',
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--no-sanitize", action="store_true")
    parser.add_argument("--jobs", type=int, default=4)
    args = parser.parse_args()
    if not 1 <= args.jobs <= 16:
        parser.error("--jobs must be between 1 and 16")

    root = Path(__file__).resolve().parents[1]
    lessons = sorted((root / "System-Design").rglob("*.cpp"))
    environment = dict(os.environ)
    environment.setdefault("CXX", "clang++")
    flags = "-O1 -g -pthread -UNDEBUG"
    if not args.no_sanitize:
        flags += " -fsanitize=address,undefined -fno-omit-frame-pointer"
        environment["UBSAN_OPTIONS"] = "halt_on_error=1"
    environment["CXXFLAGS"] = flags

    def run(label, source):
        try:
            result = subprocess.run(
                [str(root / "run_cpp.sh"), str(source)], cwd=root,
                input="", text=True, capture_output=True, timeout=60,
                env=environment,
            )
        except subprocess.TimeoutExpired:
            return label, "Timed out after 60 seconds"
        if result.returncode:
            return label, result.stdout + result.stderr
        return label, None

    with tempfile.TemporaryDirectory(prefix="system-design-checks-") as folder:
        tasks = [(str(path.relative_to(root)), path) for path in lessons]
        for index, (relative, body) in enumerate(REGRESSIONS.items()):
            fixture = Path(folder) / f"regression_{index}.cpp"
            # Renaming main lets the fixture exercise a lesson's public logic.
            fixture.write_text(
                "#include <cassert>\n#include <limits>\n#include <stdexcept>\n"
                "#define main lesson_demo_main\n#include "
                + json.dumps(str(root / "System-Design" / relative))
                + "\n#undef main\nint main() {\n" + body + "\n}\n"
            )
            tasks.append(("regression: " + relative, fixture))
        with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
            results = list(pool.map(lambda task: run(*task), tasks))

    failures = [(label, error) for label, error in results if error is not None]
    for label, error in failures:
        print(f"FAIL: {label}\n{error}")
    if failures:
        raise SystemExit(f"{len(failures)} of {len(results)} checks failed")
    mode = "with address/undefined sanitizers" if not args.no_sanitize else "without sanitizers"
    print(f"Passed {len(lessons)} system-design examples and {len(REGRESSIONS)} regression harnesses {mode}.")


if __name__ == "__main__":
    main()

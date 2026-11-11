# Implementation Coverage and Remaining Extensions

This inventory separates the logic actually demonstrated from infrastructure or
features that a lesson does not implement. A passing compiler or sanitizer run
is evidence for the tested cases, not a proof of every possible execution.

The review added ten executable system-design models, repaired concrete failure
paths in existing lessons, added a foundations path, and added eight high-level
architecture walkthroughs. Use [the learning path](README.md) to study them.

## DSA-for-System-Design

| Lesson | Implemented learning scope | Remaining boundary |
| --- | --- | --- |
| [url shortener](DSA-for-System-Design/url_shortener.cpp) | Base62 mappings, repeatable hash-based shortening, collision retry, validated decoding. | In-memory mappings; no redirect server, abuse service, TTL enforcement, or shared ID authority. |
| [snowflake id](DSA-for-System-Design/snowflake_id.cpp) | Bit-packed IDs with worker/epoch validation, rejected clock rollback, and sequence exhaustion. | Single generator instance; restart persistence, worker assignment, and thread synchronization are external. |
| [bloom filter](DSA-for-System-Design/bloom_filter.cpp) | Membership filter, parameter validation, and an approximate false-positive formula. | No deletion or proof that salted std::hash satisfies the theoretical independence assumptions. |
| [count min sketch](DSA-for-System-Design/count_min_sketch.cpp) | Non-negative frequency sketch with dimension, update, and counter-overflow checks. | No negative-update guarantee; salted demonstration hashes do not establish a theoretical probability bound. |
| [hyperloglog](DSA-for-System-Design/hyperloglog.cpp) | Register-based approximate cardinality estimation with small-range correction. | Estimation is not an exact distinct count; cross-process stable configuration/hash compatibility must be maintained. |
| [heavy hitters](DSA-for-System-Design/heavy_hitters.cpp) | Misra-Gries candidate summary and comparison with exact counts. | Candidate counters are lower bounds, not exact frequencies; verification needs another pass/source. |
| [ring buffer](DSA-for-System-Design/ring_buffer.cpp) | Bounded buffer with reject/overwrite policies and checked empty access. | Ordinary single-threaded container, not a lock-free SPSC queue. |
| [ttl cache](DSA-for-System-Design/ttl_cache.cpp) | TTL expiration using a map and expiry heap, with capacity eviction. | Single-process freshness model; lazy heap records and time assumptions need operational bounds. |
| [lru ttl cache](DSA-for-System-Design/lru_ttl_cache.cpp) | LRU ordering, TTL expiration, stale heap-record checks, and explicit timestamps. | Single-threaded; lazy expiry-record accumulation is distinct from live-entry capacity. |
| [rate limiter](DSA-for-System-Design/rate_limiter.cpp) | Token bucket and sliding-window log, monotonic/finite-time validation, deque expiration. | No leaky-bucket or interpolated counter implementation; no shared multi-process atomic store. |
| [distributed rate limiter](DSA-for-System-Design/distributed_rate_limiter.cpp) | Gateway-local buckets plus a shared in-memory authoritative sliding log. | Central object simulates coordination; no Redis deployment, network, partition recovery, or distributed transaction. |
| [consistent hashing](DSA-for-System-Design/consistent_hashing.cpp) | Virtual-node hash ring, server add/remove, and clockwise routing. | Routing model only; no actual data migration, replication, or node-membership protocol. |
| [geohash quadtree](DSA-for-System-Design/geohash_quadtree.cpp) | Owned quadtree nodes and rectangular point queries with pruning. | Quadtree implementation, not a geohash encoder, complete radius search, or road-distance index. |
| [leaderboard](DSA-for-System-Design/leaderboard.cpp) | Score map/buckets, Fenwick ranks, shared tie ranks, and top-K traversal. | Scores are deliberately clamped to 0..1000; not an unrestricted distributed ranking service. |
| [write ahead log](DSA-for-System-Design/write_ahead_log.cpp) | Durable-prefix simulation, checksums, torn tails, repeated checkpoints, and recovery after compaction. | Records/checkpoints live in memory; sync marks state and does not call fsync or survive process loss. |
| [distributed lock](DSA-for-System-Design/distributed_lock.cpp) | Majority lease-grant simulation, token-bound release/renewal, and protected-store fencing. | Process-local token allocator; not consensus/production Redlock, persistent fencing, or a partition-safety proof. |

## Distributed-Patterns

| Lesson | Implemented learning scope | Remaining boundary |
| --- | --- | --- |
| [load balancer](Distributed-Patterns/load_balancer.cpp) | Health-aware least-connections routing, round-robin ties, and ticketed completion. | Single-threaded routing model; no actual proxy, service discovery, or health-check transport. |
| [circuit breaker](Distributed-Patterns/circuit_breaker.cpp) | Closed/open/half-open transitions, one probe, cooldown, and ignored stale-generation results. | Caller reports deadlines and completes each ticket once; no network cancellation or error-rate window. |
| [retry backoff](Distributed-Patterns/retry_backoff.cpp) | Retry eligibility, capped exponential full jitter, attempt limits, and deadline budgets. | Plans delays only; does not make requests, sleep, or cancel remote work. |
| [idempotency keys](Distributed-Patterns/idempotency_keys.cpp) | Customer-scoped payload-bound keys, saved results, and mutex-protected repeatable charges. | In-memory ledger; no durable cross-process transaction or external payment gateway. |
| [reliable work queue](Distributed-Patterns/reliable_work_queue.cpp) | Capacity admission, visibility leases, delivery tokens, ack, redelivery, and dead letters. | Process-lifetime semantics only; DLQ retention is unbounded, persistence and worker cancellation are absent. |
| [transactional outbox](Distributed-Patterns/transactional_outbox.cpp) | Atomic local order+outbox mutation, crash-after-publish replay, and idempotent local projection. | Copy-and-swap transaction simulation; external effects are not made exactly-once. |
| [saga order workflow](Distributed-Patterns/saga_order_workflow.cpp) | Reservation/payment/fulfillment states, terminal retry safety, and pending compensation retry. | Sequential in-memory orchestration; no durable recovery or real service participants. |
| [cache aside versioning](Distributed-Patterns/cache_aside_versioning.cpp) | TTL/negative caching, deletion versions, and version-floor rejection of stale refills. | Synchronous invalidation model; distributed atomic fill, persistent delivery, and stampede protection remain extensions. |

## Advanced-System-Design

| Lesson | Implemented learning scope | Remaining boundary |
| --- | --- | --- |
| [inverted index](Advanced-System-Design/inverted_index.cpp) | Term/phrase posting lists, Boolean AND/OR queries, and local TF-IDF ranking. | No distributed search, NOT-query API, incremental delete/replace pipeline, or production ranking engine. |
| [merkle tree](Advanced-System-Design/merkle_tree.cpp) | FNV-based tree construction and sibling proof verification, including odd leaves. | Non-cryptographic demonstration; verifier does not authenticate leaf index/count or implement secure tree-shape commitments. |
| [lsm tree](Advanced-System-Design/lsm_tree.cpp) | Memtable flushes, newest-version lookup, tombstones, scans, and full compaction. | In-memory tables; no SSTable files, WAL integration, background compaction, or storage durability. |
| [vector clock](Advanced-System-Design/vector_clock.cpp) | Local/send/receive causal counters and comparison of before/after/concurrent states. | Causality detection is not conflict resolution or a membership protocol. |
| [crdt counter](Advanced-System-Design/crdt_counter.cpp) | G/PN state-based counters with max merges and convergence examples. | Replica identity/ownership is a caller contract; no network transport, anti-entropy scheduler, or persistent replica registry. |
| [replication quorums](Advanced-System-Design/replication_quorums.cpp) | Fixed-replica read/write overlap, version selection, read repair, and uncertain partial writes. | No concurrent writers, leader election, membership changes, durable storage, or proof of linearizability. |
| [mvcc snapshot isolation](Advanced-System-Design/mvcc_snapshot_isolation.cpp) | Versioned reads, staged writes, write-conflict rejection, and a write-skew demonstration. | Sequential in-memory snapshot-isolation model; not serializable isolation, vacuum, or a SQL database. |

## LLD-Problems

| Lesson | Implemented learning scope | Remaining boundary |
| --- | --- | --- |
| [parking lot](LLD-Problems/parking_lot.cpp) | Vehicle/spot compatibility, tickets, availability, and integer hourly fees. | Local single-threaded allocation; no distributed gates, external payments, or durable ticket store. |
| [vending machine](LLD-Problems/vending_machine.cpp) | Selection, accepted funds, inventory, change, and transaction-state transitions. | Simulated machine state; physical actuation failures and durable reconciliation are separate. |
| [elevator system](LLD-Problems/elevator_system.cpp) | Car state, stop requests, dispatch cost, and stepwise movement. | Local simulator; hardware safety interlocks and a full building-controller policy are not implemented. |
| [atm](LLD-Problems/atm.cpp) | Card/PIN session states, cash note selection, withdrawal/deposit, and balance checks. | In-memory ledger/cassettes; no secure PIN storage, banking integration, or hardware failure protocol. |
| [library management](LLD-Problems/library_management.cpp) | Titles/copies, members, loans, reservation queues, returns, and fines. | Single-threaded in-memory workflow; real checkout needs storage transactions and identity controls. |
| [logger](LLD-Problems/logger.cpp) | Severity filtering, pluggable console/memory sinks, and bounded recent-log history. | Not a chain-of-responsibility framework; no durable/network sink or concurrent log transport. |
| [snake and ladder](LLD-Problems/snake_and_ladder.cpp) | Board jumps, player turns, dice-driven progression, and win detection. | Game simulation; multiplayer transport, persistence, and adversarial dice security are external. |
| [chess](LLD-Problems/chess.cpp) | Listed piece moves, king safety, check/checkmate/stalemate, and queen promotion. | No castling, en passant, underpromotion, repetition/fifty-move rules, or full chess compliance. |
| [movie ticket booking](LLD-Problems/movie_ticket_booking.cpp) | Seat holds, hold expiry, confirmation, cancellation, and pricing. | Single-threaded model; real concurrent seat holds and payment coordination need atomic storage operations. |
| [hotel booking](LLD-Problems/hotel_booking.cpp) | Date-range availability, reservations, check-in/out, cancellation, and nightly totals. | No concurrent database reservation protocol, external payment, or complete seasonal-pricing engine. |
| [splitwise](LLD-Problems/splitwise.cpp) | Equal/exact/percentage shares, integer balance accounting, and greedy settlement. | Greedy settlement is not guaranteed to minimize transfer count; no real money movement. |
| [food delivery](LLD-Problems/food_delivery.cpp) | Menus/carts, order totals, partner assignment, status transitions, and cancellation. | Local business workflow; mapping, payment, restaurant integrations, and cross-service delivery are external. |
| [ride sharing](LLD-Problems/ride_sharing.cpp) | Driver/rider state, nearby selection, ride lifecycle, and local fare logic. | No routing/traffic engine, real GPS ingestion, payment provider, or distributed exclusive dispatch. |
| [online shopping](LLD-Problems/online_shopping.cpp) | Catalog/cart/coupons, checkout stock reservation, price snapshots, order lifecycle, and cancellation. | Single-threaded local atomicity; no payment capture, distributed stock ledger, shipping, or durable saga. |
| [task scheduler](LLD-Problems/task_scheduler.cpp) | Virtual-time tasks, priority ties, recurrence, lazy cancellation, and timestamp-horizon handling. | Single-threaded scheduler; no cron parser, worker pool, durable retries, distributed ownership, or durable task log. |
| [multi level cache](LLD-Problems/multi_level_cache.cpp) | Strategy-based LRU cache levels, read promotion, writes, hit/miss counts, and constructor validation. | All levels are in memory; no actual SSD/network tiers, coherence protocol, or concurrent access control. |
| [pub sub message broker](LLD-Problems/pub_sub_message_broker.cpp) | Partitioned in-memory append logs and per-group positions advanced on poll. | No processing ack, durable replication, membership/rebalance, or exactly-once effect guarantee. |

## Explicit extension backlog

- **Consensus:** a complete Raft/Paxos implementation needs persistent terms/votes,
  log matching, commit rules, membership changes, leader-read safety, snapshots,
  and testing under a realistic fault model. Majority arithmetic is not enough.
- **Storage:** integrate a real WAL, checksummed/framed files, fsync/group commit,
  atomic manifests/checkpoints, SSTables, compaction scheduling, and recovery
  from process/storage failures. The current WAL and LSM lessons stay in memory.
- **Deployment:** real RPC, service discovery, health probes, authentication,
  secret handling, metrics/tracing, durable migrations, multi-region failover,
  and restore drills are described in the guides, not deployed by these lessons.
- **Messaging:** durable acknowledgements, partition ownership/rebalance,
  replication, bounded retention, schema evolution, and transactional external
  effects remain separate broker/integration work.
- **Concurrency:** most domain lessons are single-threaded. A mutex in one
  component is not a cross-process transaction or a concurrency proof.
- **Domain completeness:** the chess lesson explicitly omits several legal rules;
  payment/refund operations are simulated; physical machines and fulfillment
  services are modeled rather than integrated.

These boundaries are deliberate for a C++/DSA learning repository. They must be
specified and implemented separately before presenting an example as a complete
production system. New subject areas and exercises can always be added; there
is no finite checklist containing every possible system-design problem.

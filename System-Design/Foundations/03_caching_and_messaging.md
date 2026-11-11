# Caching, Messaging, and Cross-Service Workflows

## Cache policies and races

| Policy | Flow | Failure or tradeoff |
| --- | --- | --- |
| Cache-aside | Reader loads database on miss, then fills cache | Stale refill races and concurrent miss storms. |
| Read-through | Cache layer owns loading | Loader availability becomes part of the read path. |
| Write-through | Update authoritative storage through the cache layer | Define behavior if either update fails. |
| Write-behind | Acknowledge before asynchronous storage update | Dirty data can be lost without a durable write queue. |
| Refresh-ahead | Refill before expiry | Spend work refreshing values that may never be read. |

TTL is a freshness policy; LRU/LFU is an eviction policy. A positive TTL does
not bound total memory if keys and stale expiry records are unbounded. Use
capacity limits, stale-record cleanup, and a defined clock.

Negative caching reduces repeated missing-key queries. Bloom filters can avoid
some database misses but false positives still require a lookup. A cache
stampede needs coalesced loading, randomized expiry, stale-while-revalidate,
or another explicit coordination policy. Hot keys may need replication and
per-key load protection; changing the eviction policy does not solve every hot
key. Protect tenant identity and authorization in cache keys.

[The versioned cache model](../Distributed-Patterns/cache_aside_versioning.cpp)
rejects a captured old value after a newer invalidation. Its assumptions include
ordered versions and synchronous invalidation; a deployed implementation needs
an atomic check-and-fill and a reliable invalidation path.

## Queue versus log

A work queue distributes jobs among workers and normally removes acknowledged
work. A partitioned log retains records for independent consumer positions and
replay. Ordering is usually per partition, not global. More partitions improve
parallelism but cannot automatically parallelize one ordering-sensitive hot key.

| Acknowledgement point | Processing consequence after consumer failure |
| --- | --- |
| Before application processing | Work can be skipped: at-most-once processing. |
| After processing | A crash before acknowledgement can cause duplicates. |
| Effect and consumed position committed atomically in one supported system | Can provide an exactly-once effect inside that transaction boundary. |

Do not promise exactly-once email, payment, or arbitrary database effects merely
because the broker offers transactions. [Kafka's delivery-semantics discussion](https://kafka.apache.org/44/design/design/)
distinguishes publication, consumption, and transaction boundaries.

The [existing pub-sub model](../LLD-Problems/pub_sub_message_broker.cpp) advances
its in-memory group offset on poll. It does not implement member assignment,
rebalance protocols, durable offsets, replication, or processing acknowledgement.
The [reliable queue](../Distributed-Patterns/reliable_work_queue.cpp) instead
shows leases, acknowledgement, stale-token rejection, redelivery, a live-job
capacity limit, and dead-letter handling.

## Retrying safely

Timeout means the result is unknown; the remote effect may have occurred.
Retries need error classification, idempotency, backoff, jitter, attempt limits,
and an end-to-end budget. Avoid retrying at every layer: three attempts at each
of five independent layers can amplify load dramatically.

[The retry planner](../Distributed-Patterns/retry_backoff.cpp) does not issue real
requests. It tests eligibility and scheduling without network side effects.
[Amazon's retry guidance](https://aws.amazon.com/builders-library/timeouts-retries-and-backoff-with-jitter/)
explains why unbounded retries and synchronized clients amplify outages.

An idempotency record binds a client-scoped key to the request's meaning and
saved result. Define conflicts, concurrent duplicates, in-progress requests,
retention, and replay after expiry. [The ledger model](../Distributed-Patterns/idempotency_keys.cpp)
checks concurrent duplicates in one process; a persistent implementation must
atomically couple the effect and key record.

## Outbox, inbox, sagas, and two-phase commit

The [transactional outbox](../Distributed-Patterns/transactional_outbox.cpp)
commits an order and event together locally. A relay can publish twice after a
crash, so the consumer deduplicates within its own transactional boundary. An
inbox records received event IDs with the corresponding local effect. Retention
of deduplication IDs must cover the replay horizon.

A [saga](../Distributed-Patterns/saga_order_workflow.cpp) coordinates local steps
and compensating operations. Compensation can fail and must be retried or sent
to reconciliation/manual intervention. A refund may not undo an email, shipment,
or exchange-rate change. Sagas expose intermediate states and need explicit
business semantics. See [Microsoft's saga discussion](https://learn.microsoft.com/azure/architecture/reference-architectures/saga/saga).

Two-phase commit asks participants to prepare and then commit/abort through a
coordinator. Prepared participants may block while the decision is unavailable;
recovery requires durable decisions. A saga does not provide the same atomicity.
Choose based on invariants, supported infrastructure, availability requirements,
and how reversible the business actions are.

CDC, retry queues, DLQs, and replay tools need ownership and operational policy:
who retries, what ordering is preserved, which payload/schema version is used,
and how duplicate effects are prevented. A DLQ without investigation and replay
procedures just moves failures out of the primary queue.

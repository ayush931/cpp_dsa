# System Design Learning Path

This section combines high-level architecture, executable distributed-pattern
models, data-structure building blocks, and local object-oriented designs.
Start here; folder names alone do not indicate implementation completeness.
The [review result](REVIEW_RESULT.md) records changes, verification, and scope.

## Reading sequence

1. [Requirements, capacity, and communication](Foundations/01_requirements_and_capacity.md).
2. [Storage, transactions, replication, and consistency](Foundations/02_storage_and_consistency.md).
3. [Caching, messaging, and cross-service workflows](Foundations/03_caching_and_messaging.md).
4. [Reliability, security, and operations](Foundations/04_reliability_security_and_operations.md).
5. [Low-level design method, SOLID, ownership, and state machines](Foundations/05_low_level_design_method.md).
6. Follow the C++ modules below in the repository's
   [numbered reading order](../READING_ORDER.md).
7. Work through the eight [architecture case studies](Foundations/06_architecture_walkthroughs.md).
8. Review the [implementation coverage and limits](COVERAGE.md), then test the
   failure cases your own design depends on.

The foundations cover networking/interface choices, estimation, indexes,
partitioning, replication, CAP, consistency, MVCC/isolation, cache policies,
queues/logs, idempotency, outbox/inbox, sagas/2PC, overload control, observability,
security boundaries, deployment, backups, and recovery. The case studies cover
URL shortening, feeds, chat, video, checkout/payments, search, ride dispatch,
and file synchronization.

## Executable modules

| Folder | Purpose | What to study |
| --- | --- | --- |
| `DSA-for-System-Design/` | Local building blocks | Caches, approximate summaries, routing, IDs, bounded buffers, WAL, lease/fencing behavior. |
| `Distributed-Patterns/` | Deterministic failure and coordination models | Load balancing, backoff, circuit state, duplicate requests, redelivery, outbox, saga, cache races. |
| `Advanced-System-Design/` | Storage and consistency models | Inverted/Merkle indexes, LSM, clocks, CRDTs, quorum overlap, snapshot isolation and write skew. |
| `LLD-Problems/` | Domain objects and local workflows | Reservations, inventory, payments, scheduling, games, and application state machines. |

Run a lesson from the repository root:

```bash
./run_cpp.sh System-Design/Distributed-Patterns/reliable_work_queue.cpp
python3 scripts/check_system_design.py
```

The verification script runs every system-design example and additional
failure-regression harnesses with address/undefined-behavior sanitizers by
default. It writes temporary binaries outside the repository and requires a
compiler with those sanitizers; `--no-sanitize` runs plain compiler checks and
examples on a toolchain without sanitizer support.

## What completion means here

A lesson is complete for the behavior, assumptions, and edge cases stated in its
header. The models exercise important logic in one process. They are not
production distributed deployments: persistence, network partitions, crash
recovery, concurrency, security, and infrastructure integration are present
only where a file actually implements them.

In particular, the lease simulation is not consensus, the WAL/LSM stores model
storage in memory, the partitioned broker commits on poll, and snapshot isolation
is intentionally shown allowing write skew. The coverage table identifies these
boundaries so that a simplified implementation does not teach a false guarantee.

The curriculum is broad, not an exhaustive list of every specialized system or
algorithm. Full consensus implementations, production storage engines, full
chess rules, real payment integrations, and deployed multi-region services remain
separate projects; the foundations explain how to reason about their boundaries.

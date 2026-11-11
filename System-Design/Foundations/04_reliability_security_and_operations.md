# Reliability, Security, and Operations

## Bound work and failure amplification

Give each request an end-to-end deadline. Bound connections, worker concurrency,
queue length, payload size, retries, and fan-out. A thread pool with an unbounded
queue can run out of memory while its worker count remains fixed.

[Load balancing](../Distributed-Patterns/load_balancer.cpp) distributes admitted
requests across healthy backends. Account for uneven request cost, slow backends,
health-check lag, locality, warm-up, and connection draining. Sticky sessions
trade routing flexibility for affinity; shared state may remove that requirement.

A [circuit breaker](../Distributed-Patterns/circuit_breaker.cpp) temporarily
rejects calls to a failing dependency. A bulkhead isolates resource pools so
one dependency/tenant cannot consume every worker or connection. A rate limiter
bounds admission; backpressure slows producers; load shedding rejects excess
work. These mechanisms address different failure modes. Use bounded probes
and timeouts to keep half-open circuit tests from hanging forever.

[Google SRE's overload chapter](https://sre.google/sre-book/handling-overload/)
discusses cost-aware admission and graceful degradation. Select which requests
can be delayed, rejected, sampled, or served stale; correctness-sensitive writes
usually need different treatment from an optional recommendation panel.

## Observe user outcomes

| Signal | Examples | Operational question |
| --- | --- | --- |
| Request metrics | Eligible success rate, p50/p95/p99 latency | Are users receiving correct responses within the target? |
| Saturation | Queue age/depth, CPU, memory, pool utilization | Is work accumulating faster than it completes? |
| Dependency health | Timeouts, retry rate, breaker state | Is a downstream issue being amplified? |
| Data progress | Replication/consumer/CDC lag, compaction debt | How stale or vulnerable is the stored/derived state? |
| Business reconciliation | Unsettled payments, orphan reservations, unsent outbox rows | Did a partial workflow leave an unresolved obligation? |

Use structured logs, request/correlation IDs, trace context, and consistent
metric names. Bound log volume and label cardinality. Do not put secrets or
payment credentials in logs. An alert should indicate user impact or a risk
requiring action; routine dashboards and alerts serve different purposes.

## Trust and abuse boundaries

Authenticate the caller, then authorize the specific object/action/tenant.
A guessed resource ID must not grant access. Compare RBAC/ABAC policies,
least-privilege service identities, TLS, encryption at rest, key rotation,
secret storage, and audit trails according to the design's trust boundaries.

Apply input and payload limits, replay/idempotency policy, quotas, and abuse
controls. For browser sessions, account for cookie/session security and the
application's CSRF/XSS threat model. Signed object URLs need scope and expiry.
Data retention/deletion must cover primary data, derived indexes, caches,
logs, and backups according to the actual requirements; do not promise instant
physical deletion from immutable backups.

## Deployment and recovery

Rolling, blue/green, and canary releases need backward-compatible schemas and
contracts. Prefer expand-and-contract migrations: add compatible fields/indexes,
backfill, switch readers/writers, verify, then retire the old representation.
A rollback cannot automatically reverse an irreversible data transformation.

Define recovery point objective (acceptable data loss) and recovery time
objective (acceptable restoration time). Test backups with restore drills.
State what a region failover loses under asynchronous replication; failover
and failback need a split-brain prevention and reconciliation plan.

A cache or broker outage can become a database outage if every request falls
back at once. Test dependency timeout, partial availability, packet delay,
process restart, duplicate messages, corrupt/torn log tails, and clock issues.
Use deterministic injected failure paths locally before involving real services.
The repository's models inject failures; they do not simulate a real network.

## Review checklist

- Identify the owner and source of truth for every piece of mutable state.
- Name the atomic transaction boundary and the success acknowledgement point.
- Explain what happens when a client retries after an unknown result.
- Explain stale reads, partial writes, dropped invalidations, and duplicate events.
- Bound work and storage, including history, tombstones, and dead letters.
- Identify the failure domain, recovery procedure, and operator-visible signal.
- State which guarantees depend on infrastructure absent from the C++ model.

# Requirements, Capacity, and Communication

Start with a concrete workload. A design for 100 concurrent editors differs
from one for 100 million readers even when both are described as a document app.

## A repeatable design process

1. Name users and the three main actions. Explicitly exclude secondary features.
2. Define correctness: uniqueness, ordering, freshness, retention, and what a
   successful acknowledgement promises after a crash.
3. Set measurable targets: peak traffic, data size, p99 latency, availability,
   and recovery expectations. State your assumptions.
4. Choose keys and access patterns before choosing databases. Sketch records,
   indexes, request/response shapes, and cursor pagination.
5. Trace one write and one read through the components. Mark transaction and
   trust boundaries, synchronous calls, and asynchronous queues.
6. Find the first bottleneck. Estimate it; add caching, replicas, partitions,
   or batching only where they solve an identified constraint.
7. Walk through timeout, duplicate, partial failure, hot-key, and region-loss
   scenarios. Explain the remaining tradeoffs.

## Capacity worked example

Assume 10 million daily active users, 20 reads and 2 writes per user per day.
These are hypothetical planning inputs, not measurements.

| Quantity | Calculation | Result |
| --- | --- | --- |
| Average reads | 10,000,000 × 20 / 86,400 | about 2,315 reads/s |
| Average writes | 10,000,000 × 2 / 86,400 | about 232 writes/s |
| Peak reads, assumed 5× | 2,315 × 5 | about 11,575 reads/s |
| Raw new data, 1 KB/write | 20,000,000 × 1 KB | about 20 GB/day |
| One year raw retention | 20 GB × 365 | about 7.3 TB |
| Three stored copies | 7.3 TB × 3 | about 21.9 TB before indexes/backups |
| Read bandwidth, 2 KB/response | 11,575 × 2 KB | about 23.15 MB/s before overhead |

Here KB/GB/TB are decimal units. Keep units explicit and account for indexes,
compression, tombstones, logs, headroom, and backups separately. Cache hit rates
reduce database reads, but misses can arrive in bursts after eviction or outage.

Little's law gives **average in-flight work = arrival rate × average time in
system** in a stable system. At 10,000 requests/s and 0.1 seconds average total
latency, expect about 1,000 in-flight requests. This is not a p99 bound and does
not directly determine the number of CPU cores required.

## Latency and availability

Separate client latency from server latency and synchronous critical paths from
background work. Include connection setup, DNS/TLS, queue wait, service work,
and database time in an end-to-end deadline. Per-hop timeouts must fit inside it.
Do not add per-service p99 values and call the result an end-to-end p99.

A 99.9% success SLO allows 0.1% failed eligible requests over its defined window.
That corresponds to 43.2 minutes over 30 days only under a time-based availability
interpretation. Define the denominator and treatment of partial outages.

## Network and interface choices

| Choice | Use when | Design consequence |
| --- | --- | --- |
| HTTP request/response | Short interactions and broad client support | Bound body sizes, paginate, define error/retry behavior. |
| RPC with typed schemas | Internal contracts and streaming calls | Handle deadlines, backward compatibility, and partial streams. |
| WebSocket | Bidirectional long-lived interaction, such as chat | Plan connection ownership, reconnect, authentication renewal, and backpressure. |
| Server-sent events | Server-to-client event streams | Define resume IDs and reconnection behavior. |
| Queue or log | Work can outlive the request or absorb bursts | Specify durability, acknowledgement, replay, ordering, and duplicate handling. |

DNS resolves names; a load balancer routes connections or requests. A reverse
proxy terminates/routs traffic; an API gateway may additionally apply policy.
L4 routing sees transport connections; L7 routing can inspect application fields.
Connection pooling saves setup work but needs bounded size and idle lifetimes.

Horizontal scaling requires explicit handling of sessions, shared state, and
connection affinity. Vertical scaling can be a useful first step but retains
per-machine capacity and failure limits. HTTP cache headers and CDN caching
must respect user identity and content privacy.

Practice: change the read/write ratio, payload size, retention, and peak factor.
Explain which component becomes the bottleneck before adding another service.

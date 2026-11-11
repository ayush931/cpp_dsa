# Storage, Transactions, Replication, and Consistency

Design the access patterns first: key lookup, sorted range, text search, spatial
search, joins, and analytical scans need different indexes and storage layouts.

## Storage and index selection

| Structure | Strength | Cost or limitation |
| --- | --- | --- |
| Relational tables | Constraints, joins, transactions, flexible querying | Schema/index design and transaction contention matter. |
| Key-value store | Access by a known key and partitioning by that key | Secondary queries require additional indexes or materialized views. |
| Document model | Aggregate-oriented reads and flexible nested records | Cross-document invariants still need coordination. |
| Wide-column model | Partition-local sorted access at scale | Poor partition keys create hot or oversized partitions. |
| Object storage | Large immutable blobs and cheap independent storage | Metadata, permissions, and discovery usually live elsewhere. |
| Search index | Token/posting-list retrieval and ranking | Often a derived view with an indexing freshness delay. |

B+ trees support ordered lookups/ranges; LSM trees trade compaction and read
amplification for a write-oriented layout. A hash index does not provide sorted
ranges. Composite index column order must match equality filters, ordering,
and range predicates; extra indexes amplify writes. Measure query plans rather
than assuming an index always improves a query.

Normalize authoritative transactional data when it simplifies invariants.
Denormalize/materialize read views where measured read cost justifies the
additional update and reconciliation work. CQRS separates read/write models;
event sourcing stores domain events as the source of truth. Neither implies
a microservice architecture or follows automatically from adding an event log.

## Transactions and isolation

Atomicity, consistency of application invariants, isolation, and durability are
separate properties. A transaction's durability depends on its acknowledgement
and storage configuration, not just the presence of a transaction API.

| Behavior | Example | Appropriate mechanism to investigate |
| --- | --- | --- |
| Lost update | Two read-modify-write operations overwrite one another | Atomic update, compare-and-set/version check, or locking. |
| Write conflict | Two writers modify the same row | Row locking or optimistic conflict detection with retry. |
| Write skew | Two doctors independently go off call after reading each other on call | Serializable isolation or explicit protection of the shared invariant. |
| Phantom-sensitive invariant | Two transactions both find no overlapping booking and insert | Constraint, serializable predicate protection, or correctly scoped lock. |

MVCC retains versions so readers can use snapshots. Snapshot isolation can still
allow write skew; [the runnable model](../Advanced-System-Design/mvcc_snapshot_isolation.cpp)
shows that failure. Isolation names and implementations differ between products;
consult the actual database's guarantees. [PostgreSQL's isolation documentation](https://www.postgresql.org/docs/current/transaction-iso.html)
explains its snapshot and serialization behavior.

Optimistic concurrency reads a version and commits only if it is unchanged.
Pessimistic concurrency locks the protected state first. Both require transaction
boundaries, contention handling, and deadlock/timeout policies. Use a consistent
lock order when a transaction touches several resources.

## Replication and read guarantees

Leader/follower replication can scale reads, but an asynchronous follower can
return an older value immediately after a successful write. Read-your-writes may
require leader reads, session version tokens, or waiting for replica progress.
Distinguish eventual convergence, causal ordering, linearizable operations, and
serializable transactions; these answer different questions.

For a fixed set of N replicas, R+W>N ensures read/write set overlap under the
stated membership and persistence assumptions. That arithmetic alone does not
establish a correct concurrent protocol. The [quorum model](../Advanced-System-Design/replication_quorums.cpp)
shows overlap, read repair, and the ambiguity of a partially acknowledged write.
Sloppy quorums, concurrent versions, and changing membership need more reasoning.

Consensus protocols coordinate an ordered decision/log among failure-prone nodes.
[Raft's paper](https://raft.github.io/raft.pdf) specifies elections, replication,
and safety rules; a majority counter with TTL leases is not a Raft implementation.
A leader must also establish its current authority before serving linearizable
reads. A fencing token is enforced by the protected resource, not by trusting
that an old client stopped after its lease expired.

CAP concerns the inability to provide both linearizable consistency and the
formal availability property during a network partition. It does not say every
system permanently chooses any two of three independent features, nor does its
availability definition equal an operational uptime SLO. State which operations
reject, wait, or return stale data during the partition.

## Partitioning and lifecycle

Hash partitioning spreads key lookups; range partitioning preserves locality but
can concentrate monotonically increasing writes. Directory-based routing adds
an indirection service. Plan for hot tenants/keys, resharding, ownership epochs,
backfill, duplicate writes during migration, and route changes.

Replication copies data for resilience; partitioning splits data for capacity.
Replicas do not replace backups: a replicated mistaken deletion is still a
mistaken deletion. Specify retention, point-in-time restore, encryption, and a
restore drill. CDC can maintain derived views, but consumers must handle replay,
schema changes, lag, deletion, and reconciliation.

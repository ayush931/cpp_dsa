# Recognizing DSA Patterns

Use [READING_ORDER.md](READING_ORDER.md) for the full curriculum and
[CONCEPTS.md](CONCEPTS.md) for the concept index. This guide helps choose an
approach when a problem statement does not name its technique.

Read the example, explain its invariant, then implement it without looking.
Change the constraints afterward: allow negatives, introduce duplicates,
request a count instead of a length, or require every answer instead of one.
Those changes often determine whether the original technique still works.

| Recognition cue | Pattern and suggested practice order | Common mistake |
| --- | --- | --- |
| Sorted data, pair/triplet/quadruplet target | [Two pointers](Techniques/Two-Pointers/two_pointers_-_use_two_indices.cpp) → [3Sum](Techniques/Two-Pointers/three_sum.cpp) → [4Sum](Techniques/Two-Pointers/four_sum.cpp) | Skipping duplicates at only one level; overflowing a sum before promoting it. |
| A contiguous range with a monotone validity condition | [Window basics](Techniques/Sliding-Window/sliding_window_-_maintain_a_window.cpp) → [minimum size](Techniques/Sliding-Window/minimum_size_subarray_sum.cpp) → [replacement budget](Techniques/Sliding-Window/longest_repeating_character_replacement.cpp) | Assuming a sum window still works after allowing negative numbers. |
| Count ranges with exactly K distinct values | [Exactly K distinct](Techniques/Sliding-Window/subarrays_with_k_distinct.cpp) | Trying to shrink directly on equality instead of subtracting two at-most counts. |
| Subarray sum, divisibility, or XOR with arbitrary signs | [Count sum K](Techniques/Prefix-Sum/subarray_sum_equals_k.cpp) → [longest sum K](Techniques/Prefix-Sum/longest_subarray_sum_k.cpp) → [divisible sums](Techniques/Prefix-Sum/subarray_divisible_by_k.cpp) → [XOR K](Hashing/Hashmaps/subarray_xor_equals_k.cpp) | Confusing earliest indices with frequencies; forgetting the empty prefix or negative-remainder normalization. |
| Best contiguous sum with wraparound | [Kadane](CPP-Basics/Array-Problems/maximum_subarray_kadane_s_algorithm.cpp) → [circular Kadane](CPP-Basics/Array-Problems/maximum_circular_subarray.cpp) | Selecting an empty complement when every value is negative. |
| Values lie in a small indexable range | [First missing positive](CPP-Basics/Array-Problems/first_missing_positive.cpp) | Swapping duplicates forever, or using an out-of-range value as an index. |
| Smallest/largest feasible numeric answer | [Binary search on the answer](Searching/Advanced-Searching/search_answer_questions.cpp) | Failing to prove the feasibility predicate is monotone or choosing invalid bounds. |
| List cycle, midpoint, or fixed-distance relationship | [Cycle detection](Linked-Lists/Singly-Linked-List-Problems/head_the_head_of_linked_list.cpp) → [remove from end](Linked-Lists/Singly-Linked-List-Problems/the_head_of_the_linkedlist_remove.cpp) → [reverse K groups](Linked-Lists/Singly-Linked-List-Problems/reverse_nodes_k_group.cpp) → [sort list](Linked-Lists/Singly-Linked-List-Problems/sort_list.cpp) | Losing the next pointer during reversal; changing values when nodes must be rewired. |
| Nearest smaller/greater boundary or each element's subarray contribution | [Next smaller](Stacks-and-Queues/Stack-Problems/next_smaller_element.cpp) → [histogram](Stacks-and-Queues/Stack-Problems/largest_rectangle_histogram.cpp) → [sum of minima](Stacks-and-Queues/Stack-Problems/sum_of_subarray_minimums.cpp) | Using the same strictness on both boundaries and double-counting equal minima. |
| Expiring range extrema or shortest threshold range with negatives | [Sliding maximum](Stacks-and-Queues/Deque/sliding_window_maximum.cpp) → [shortest subarray](Stacks-and-Queues/Deque/shortest_subarray_at_least_k.cpp) | Keeping values without their indices, or failing to remove dominated starts. |
| Merge multiple sorted streams, or maintain a moving rank | [Merge K lists](Heaps-and-Priority-Queues/Priority-Queue/merge_k_sorted_lists.cpp) → [smallest covering range](Heaps-and-Priority-Queues/Priority-Queue/smallest_range_k_lists.cpp) → [sliding median](Heaps-and-Priority-Queues/Priority-Queue/sliding_window_median.cpp) | Ignoring the outgoing occurrence; two heaps need deletion bookkeeping, while multisets support direct deletion. |
| Intervals can be selected, but weights change the objective | [Activity selection](Greedy/activity_selection.cpp) → [weighted scheduling](Dynamic-Programming/DP-Problems/weighted_job_scheduling.cpp) | Applying earliest-finish greedy to maximum profit. |
| Local decisions need a global invariant | [Jump Game I](Greedy/jump_game.cpp) → [minimum jumps](Greedy/jump_game_ii.cpp); [Candy](Greedy/candy.cpp); [Partition Labels](Greedy/partition_labels.cpp) | Assuming a plausible greedy choice is optimal without a reachability, exchange, or boundary argument. |
| Include/exclude choices with a target | [Knapsack/subset sum](Dynamic-Programming/DP-Problems/knapsack_and_subset_sum.cpp) → [Target Sum](Dynamic-Programming/DP-Problems/target_sum.cpp) | Iterating capacities forward for 0/1 choices, or forgetting that each zero doubles assignment counts. |
| Two strings or pattern prefixes define a state | [LCS](Dynamic-Programming/DP-Problems/longest_common_subsequence.cpp) → [interleaving](Dynamic-Programming/DP-Problems/interleaving_string.cpp) → [wildcards](Dynamic-Programming/DP-Problems/wildcard_matching.cpp) → [regex DP](Dynamic-Programming/DP-Problems/regular_expression_matching.cpp) | Losing the old diagonal during row compression; treating wildcard `*` as regex `*`. |
| Boolean feasibility becomes enumeration | [Word Break I](Dynamic-Programming/Advanced-DP/word_break.cpp) → [Word Break II](Dynamic-Programming/Advanced-DP/word_break_ii.cpp) | Claiming polynomial total runtime while returning exponentially many sentences. |
| Strict nesting or increasing chains in two dimensions | [LIS](Dynamic-Programming/DP-Problems/longest_increasing_subsequence.cpp) → [Russian Doll Envelopes](Dynamic-Programming/DP-Problems/russian_doll_envelopes.cpp) | Sorting both dimensions ascending and accidentally chaining equal widths. |
| Reachability from many sources or from the boundary | [Multi-source BFS](Graphs/Graph-Traversal/multi_source_bfs.cpp) → [surrounded regions](Graphs/Graph-Problems/surrounded_regions.cpp) | Marking visited only on dequeue and adding the same cell repeatedly. |
| Ordering constraints or strictly increasing transitions | [Kahn](Graphs/Topological-Sort/kahn_algo.cpp) → [Alien Dictionary](Graphs/Topological-Sort/alien_dictionary.cpp) → [increasing matrix path](Graphs/Topological-Sort/longest_increasing_path.cpp) | Duplicating indegrees, missing prefix-invalid words, or overlooking cycles. |
| Connectivity changes as undirected relationships arrive | [DSU basics](Graphs/Disjoint-Set/leader_parent_of_the_group_-.cpp) → [redundant edge](Graphs/Disjoint-Set/redundant_connection.cpp) → [Accounts Merge](Graphs/Disjoint-Set/accounts_merge.cpp) | Applying DSU to directed cycles or merging accounts solely by name. |
| Downward tree paths may start anywhere | [Path Sum II](Trees/Binary-Tree-Problems/path_sum_II.cpp) → [Path Sum III](Trees/Binary-Tree-Problems/path_sum_iii.cpp) | Leaving sibling prefixes in a shared map instead of backtracking counts. |
| Need a whole-tree answer for every possible root | [Tree DP](Dynamic-Programming/Advanced-DP/tree_dp.cpp) → [sum of distances](Graphs/Graph-Problems/sum_of_distances_in_tree.cpp) | Recomputing a complete traversal for every root; confusing subtree and whole-tree states. |
| Large sparse coordinates, but only relative order matters | [Coordinate compression](Techniques/Coordinate-Compression/coordinate_compression.cpp) → [Fenwick tree](Trees/Fenwick-Tree/fenwick_tree_binary_indexed_tree_-.cpp) | Assuming adjacent ranks have adjacent original values or equal geometric distances. |
| Combine structures to meet several operation bounds | [LRU cache](Advanced-Data-Structures/lru_cache.cpp) → [Randomized Set](Advanced-Data-Structures/randomized_set.cpp) → [TimeMap](Advanced-Data-Structures/time_based_key_value_store.cpp) | Forgetting to update a moved value's index or silently accepting unsorted timestamp histories. |

For C++ safety while implementing these patterns, start with the
[STL toolkit](CPP-Basics/STL-Essentials/stl_toolkit.cpp),
[initialization rules](CPP-Basics/Fundamentals/initialization_and_narrowing.cpp),
[value categories](Advanced-CPP/References-and-Memory/value_categories.cpp), and
[iterator invalidation](Advanced-CPP/STL-Containers/iterator_invalidation.cpp).

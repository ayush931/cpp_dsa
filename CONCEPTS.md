# C++ and DSA — Complete Concept Index

This file is a complete map of every concept covered in this repository.
It is based on a full scan of all folders and their `.cpp` files.

The folder references below map directly to the corresponding module folders
and files. Use them when navigating or reviewing topics.

- **Section 1** covers C++ language concepts.
- **Section 2** covers Data Structures and Algorithms concepts.
- **Section 3** covers advanced C++, DSA, concurrency, performance,
  and system-design data structures.
- **Appendix** gives a concept checklist so that no topic is missed.

---

## Section 1 — C++ Concepts

## 1.1 Basics, Operators, and Control Flow

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 1 | Program structure | `#include`, `main()`, `cout`, `endl`, `return 0`, `using namespace std` | `CPP-Basics/Fundamentals` |
| 2 | Comments | Single-line `//` comments, multi-line `/* ... */` comments | `CPP-Basics/Fundamentals` |
| 3 | Variables and data types | Declaration, initialization, naming rules, `int`, `float`, `char`, `bool`, `long long` | `CPP-Basics/Fundamentals` |
| 4 | Input and output | `cin`, `cout`, `getline`, ASCII conversion using `int(ch)` | `CPP-Basics/Fundamentals`, `CPP-Basics/Strings` |
| 5 | Swapping values | Swapping two variables using a temporary variable | `CPP-Basics/Fundamentals` |
| 6 | Operators | Arithmetic, relational, assignment, increment and decrement operators | `CPP-Basics/Fundamentals` |
| 7 | Bitwise operators | Bitwise NOT, AND, OR, XOR, left shift, right shift | `CPP-Basics/Fundamentals` |
| 8 | Miscellaneous operators | `sizeof`, ternary operator, comma operator, dot operator, arrow operator | `CPP-Basics/Fundamentals` |
| 9 | Conditional statements | `if`, `else if`, `else`, nested conditions | `CPP-Basics/Fundamentals` |
| 10 | Ternary operator | `condition ? value1 : value2` | `CPP-Basics/Fundamentals` |
| 11 | Logical operators | Logical AND, logical OR, short-circuit evaluation | `CPP-Basics/Fundamentals` |
| 12 | Switch case | `switch`, `case`, `break`, `default` | `CPP-Basics/Fundamentals` |
| 13 | Loops | `while`, `for`, `do-while`, nested loops | `CPP-Basics/Fundamentals` |
| 14 | Loop control | `break` and `continue` | `CPP-Basics/Fundamentals` |
| 15 | Number system conversion | Binary to decimal, decimal to binary | `CPP-Basics/Fundamentals` |
| 16 | Omitting loop parts | Omitting initialization, condition, or update expression in a `for` loop | `CPP-Basics/Fundamentals` |
| 17 | Pattern printing | Rectangle, square, triangle, inverted triangle, hollow patterns, numeric patterns, odd/even patterns | `CPP-Basics/Pattern-Printing` |
| 18 | Loop-based numeric problems | Factorial, count digits, reverse a number, sum of digits, sum of series, `a` to the power `b` | `CPP-Basics/Number-Problems` |
| 19 | Structures | `struct`, member access, custom data grouping, use in sorting comparators | `Greedy`, `Graphs/MST-Kruskal-Prims` |

## 1.2 Functions and Recursion

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 20 | Functions | Declaration, definition, function prototype, return type, parameters | `CPP-Basics/Functions` |
| 21 | Formal and actual parameters | Difference between parameters in the function definition and arguments passed at the call | `CPP-Basics/Functions` |
| 22 | Standard library functions | `pow`, `sqrt`, `<cmath>` | `CPP-Basics/Functions` |
| 23 | Pass by value | A copy of the argument is passed to the function | `CPP-Basics/Functions` |
| 24 | Pass by reference | Reference parameters, changes reflect in the caller | `CPP-Basics/Functions` |
| 25 | Default parameters | Default argument values in function definitions | `CPP-Basics/Functions` |
| 26 | References | Reference variable `int &`, aliasing, shared memory address | `CPP-Basics/Functions`, `CPP-Basics/Pointers` |
| 27 | Recursion | Function calling itself, base case, recursive case, call stack | `Recursion/Basics` |

## 1.3 Arrays, Vectors, 2-D Arrays, and Strings

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 28 | Arrays | Declaration, initialization, contiguous memory, indexing, traversal using `for`, for-each, and `while`, input/output | `CPP-Basics/Arrays` |
| 29 | Linear search | Searching an element in an array and returning its index or `-1` | `CPP-Basics/Arrays` |
| 30 | Vectors | Dynamic arrays, `size()`, `capacity()`, `resize()`, `push_back()`, `insert()`, `pop_back()`, `erase()`, `begin()`, `end()`, iteration | `CPP-Basics/Vectors` |
| 31 | 2-D arrays | Declaration, input, traversal, transpose of a matrix, multiplication of two matrices | `CPP-Basics/Two-D-Arrays` |
| 32 | Strings | `string` vs `char[]`, `getline`, ASCII values, `reverse`, `substr`, concatenation, `strcat`, `push_back`, `size`, `strlen`, `to_string` | `CPP-Basics/Strings` |
| 33 | String iteration and indexing | Accessing characters with `[]`, traversing strings, appending characters | `CPP-Basics/Strings` |

## 1.4 Pointers, References, and Memory

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 34 | Pointers | Address-of operator `&`, pointer declaration, dereference operator `*`, pointer types | `CPP-Basics/Pointers` |
| 35 | Pointer arithmetic | Increment and decrement, array-pointer relationship, `*(arr + i)`, pre-increment and post-increment with pointers | `CPP-Basics/Pointers` |
| 36 | Call by reference using pointers | Swapping values and returning multiple results through pointer parameters | `CPP-Basics/Pointers` |
| 37 | Special pointers | Dangling pointer, null pointer, void pointer, wild pointer | `CPP-Basics/Pointers` |
| 38 | Dynamic memory allocation | `new`, `delete`, pointers to objects, heap allocation | `Linked-Lists/Singly-Linked-List`, `Linked-Lists/Doubly-Linked-List`, `Stacks-and-Queues/Stacks`, `Stacks-and-Queues/Queues`, `Trees/Binary-Tree` |
| 39 | `nullptr` and `NULL` | Null pointer representation and initialization | `CPP-Basics/Pointers`, `Linked-Lists/Singly-Linked-List` |

## 1.5 Object-Oriented Programming (OOP)

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 40 | Classes and objects | Class definition, object creation, data members, member functions, access specifiers (`public`, `private`, `protected`), `this` pointer | `OOP` |
| 41 | Constructors | Default constructor, parameterized constructor, copy constructor | `OOP` |
| 42 | Destructor | `~ClassName()`, object cleanup, `delete` | `OOP` |
| 43 | Encapsulation | Binding data and methods into one unit, data hiding | `OOP` |
| 44 | Abstraction | Showing essential information while hiding implementation details, abstract data types | `OOP` |
| 45 | Inheritance | Single, multilevel, multiple, hierarchical, and hybrid inheritance | `OOP` |
| 46 | Diamond problem | Ambiguity caused by multiple inheritance with a common ancestor, virtual inheritance | `OOP` |
| 47 | Polymorphism | Compile-time polymorphism and runtime polymorphism | `OOP` |
| 48 | Function overloading | Same function name with different parameter lists | `OOP` |
| 49 | Operator overloading | Overloading operators for user-defined classes | `OOP` |
| 50 | Runtime polymorphism | Function overriding using `virtual` functions | `OOP` |
| 51 | Friend function | Non-member function that can access private members of a class | `OOP` |

### 1.5.1 Advanced OOP

| Concept | What is covered | File |
| --- | --- | --- |
| Static members | Shared data, static member functions, static locals, `inline static` | `OOP/static_members.cpp` |
| `this` pointer | Shadowing, method chaining, self-assignment, const member functions | `OOP/this_pointer.cpp` |
| Copy semantics | Shallow vs deep copy, copy constructor, copy assignment, self-assignment | `OOP/copy_semantics.cpp` |
| Virtual destructor | Polymorphic deletion, pure virtual destructor, destruction order | `OOP/virtual_destructor.cpp` |
| Const member functions | Const objects, const overloading, `mutable` | `OOP/const_member_functions.cpp` |
| Rule of 0/3/5 | Destructor, copy, move operations, RAII alternatives | `OOP/rule_of_three_five_zero.cpp` |

## 1.6 Templates, STL, and Other Language Features

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 52 | Templates | Class templates, generic data types, `template <typename T>` | `Linked-Lists/Circular-Linked-List` |
| 53 | STL overview | Standard Template Library, three major components: containers, iterators, algorithms | `Linked-Lists/Circular-Linked-List` |
| 54 | Iterators | `begin()`, `end()`, `rbegin()`, `rend()`, `advance()`, reverse iterators | `Linked-Lists/Circular-Linked-List` |
| 55 | STL containers | `vector`, `list`, `stack`, `queue`, `deque`, `set`, `multiset`, `unordered_set`, `unordered_multiset`, `map`, `multimap`, `unordered_map`, `unordered_multimap`, `priority_queue`, `pair` | See the STL container map below |
| 56 | STL algorithms | `sort`, `min`, `max`, `reverse`, `find`, custom comparator functions | `Linked-Lists/Circular-Linked-List`, `Greedy` |
| 57 | Range-based for loop and `auto` | `for (auto value : container)`, automatic type deduction | `CPP-Basics/Arrays`, `CPP-Basics/Vectors`, `Hashing/Sets` |
| 58 | `const` keyword | Constant variables, `const` references in range-based loops | `CPP-Basics/Array-Problems`, `Heaps-and-Priority-Queues/Heaps`, `Greedy` |
| 59 | Preprocessor directives and macros | `#include`, `#define`, `ll` and `pp` macros, common header files | `Graphs/MST-Kruskal-Prims`, `Graphs/Dijkstra`, `Greedy` |
| 60 | Header files | `iostream`, `bits/stdc++.h`, `cmath`, `vector`, `list`, `queue`, `stack`, `deque`, `set`, `map`, `algorithm`, `climits` (`INT_MAX`) | Used across the project |

### STL Container Map

| STL container | Folder in this project |
| --- | --- |
| `vector` | `CPP-Basics/Vectors` |
| `list` (doubly linked list) | `Linked-Lists/Circular-Linked-List` |
| `stack` | `Stacks-and-Queues/Stacks` |
| `queue` | `Stacks-and-Queues/Queues` |
| `deque` | `Stacks-and-Queues/Deque` |
| `set` | `Hashing/Sets` |
| `multiset` | `Hashing/Unordered-Sets` |
| `unordered_set` | `Hashing/Unordered-Sets` |
| `unordered_multiset` | `Hashing/Unordered-Sets` |
| `map` | `Hashing/Hash-Concepts` |
| `multimap` | `Hashing/Hash-Concepts` |
| `unordered_map` | `Hashing/Hash-Concepts` |
| `unordered_multimap` | `Hashing/Hash-Concepts` |
| `priority_queue` | `Heaps-and-Priority-Queues/Priority-Queue` |
| `pair` and `make_pair` (`.first`, `.second`) | `Hashing/Hash-Concepts`, `Graphs/Graph-Basics` |
| Iterators | `Linked-Lists/Circular-Linked-List` |
| STL algorithms and comparators | `Linked-Lists/Circular-Linked-List`, `Greedy` |

---

## Section 2 — Data Structures and Algorithms

## 2.1 Complexity Analysis

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 1 | Time complexity | Big-O notation, constant time, linear time, nested loops, `O(n + m)`, `O(n²)`, `O(log n)`, `O(n log n)`, `O(n√n)`, best/worst/average case, amortized analysis, common growth rates, optimizing a solution | `Complexity-Analysis/Time-Complexity`, `Complexity-Analysis/Problems` |
| 2 | Space complexity | Extra memory requirement, auxiliary space, `O(1)` space examples, recursion stack space | `Complexity-Analysis/Space-Complexity`, `Complexity-Analysis/Problems` |
| 3 | Complexity comparison | Comparing brute-force and optimized approaches for the same problem | `Complexity-Analysis/Time-Complexity`, `Complexity-Analysis/Problems` |

## 2.2 Recursion

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 4 | Recursion fundamentals | Base case, recursive case, call stack, factorial, Fibonacci | `Recursion/Basics` |
| 5 | Recursion on numbers | Power calculation, optimized power using divide and conquer, sum of digits | `Recursion/Problems` |
| 6 | Recursion on arrays | Print array, sum of array, maximum element of array | `Recursion/Problems` |
| 7 | Recursion on strings and numbers | Palindrome number, remove all occurrences of a character from a string | `Recursion/Problems` |
| 8 | Recursion patterns | Print multiples of a number, increasing sequence without loops, sum with alternating signs | `Recursion/Problems` |
| 9 | Recursion applications | Armstrong number, frog jump problem, greatest common divisor (Euclidean algorithm) | `Recursion/Problems` |
| 10 | Recursion and combinatorics | Count paths in a grid, sum of all subsets, search for an element in an array | `Recursion/Problems` |
| 11 | Recursion and strings | Phone keypad letter combinations, all subsequences of a string | `Recursion/Problems` |

## 2.3 Arrays and 2-D Arrays

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 12 | Array fundamentals | Traversal, sum of elements, maximum element, linear search | `CPP-Basics/Arrays` |
| 13 | Array problem solving 1 | Pair sum, triplet sum, unique element, second largest element, array rotation by `k` steps, frequency queries | `CPP-Basics/Array-Problems` |
| 14 | Array two-pointer and partitioning | Sort an array of 0s and 1s, move even numbers before odd numbers, squares of a sorted array | `CPP-Basics/Array-Problems` |
| 15 | Prefix sums | Running sum, equal-sum partition, range sum queries using prefix sums | `CPP-Basics/Array-Problems` |
| 16 | 2-D array fundamentals | Taking input, printing, transpose, matrix multiplication | `CPP-Basics/Two-D-Arrays` |
| 17 | Pascal's triangle | Building the first `n` rows of Pascal's triangle | `CPP-Basics/Two-D-Array-Problems` |
| 18 | 2-D array searching and rotation | Row with maximum number of 1s (brute force and optimized), rotate a matrix by 90 degrees clockwise in place | `CPP-Basics/Two-D-Array-Problems` |
| 19 | Spiral traversal | Spiral order traversal of a matrix, generating a matrix in spiral order | `CPP-Basics/Two-D-Array-Problems` |
| 20 | 2-D prefix sums | Rectangle sum queries using brute force, row-wise prefix sums, and full 2-D prefix sums | `CPP-Basics/Two-D-Array-Problems` |

## 2.4 Sorting and Searching

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 21 | Bubble sort | Repeatedly swap adjacent elements in the wrong order; optimized bubble sort for nearly sorted arrays; time and space complexity | `Sorting/Bubble-Sort` |
| 22 | Selection sort | Repeatedly find the minimum element and place it at the beginning; stability analysis | `Sorting/Selection-Sort` |
| 23 | Insertion sort | Take elements from the unsorted part and insert them into the sorted part; best-case complexity and stability | `Sorting/Insertion-Sort` |
| 24 | Sorting problems 1 | Move all zeros to the end while keeping the order of non-zero elements; sort fruit names in lexicographical order using selection sort | `Sorting/Problems` |
| 25 | Merge sort | Divide and conquer, merge two sorted halves; master's theorem for complexity; `O(n log n)` time and `O(n)` space | `Sorting/Merge-Sort` |
| 26 | Quick sort | Pivot selection, partitioning, recursive sorting; average and worst-case complexity, stability | `Sorting/Quick-Sort` |
| 27 | Count sort | Frequency array, cumulative frequency, stable counting sort; `O(n)` time and `O(n)` space | `Sorting/Count-Sort` |
| 28 | Bucket sort | Finding the range, distributing elements into buckets, sorting each bucket, concatenating buckets | `Sorting/Bucket-Sort` |
| 29 | Sorting problems 2 | Kth smallest element using quick sort (quickselect), merging two sorted arrays | `Sorting/Problems` |
| 30 | Radix sort | Digit-by-digit counting sort; `O(d * n)` time and `O(n)` space | `Sorting/Radix-Sort` |
| 31 | Binary search | Iterative binary search, recursive binary search, first occurrence, square root using binary search | `Searching/Binary-Search` |
| 32 | Binary search problems 1 | First and last position of a target, minimum element in a rotated sorted array, search in a rotated sorted array, search with duplicates | `Searching/Binary-Search-Problems` |
| 33 | Binary search problems 2 | Peak index in a mountain array, peak element, search in a 2-D sorted matrix | `Searching/Binary-Search-Problems` |
| 34 | Binary search on answer | Chocolate distribution / book allocation, aggressive cows / race track problem | `Searching/Binary-Search-Problems` |

## 2.5 Strings

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 35 | String fundamentals | `string` vs character array, input using `cin` and `getline`, ASCII values, common string functions | `CPP-Basics/Strings` |
| 36 | String algorithms | Sorting a string using frequency counting, anagram check, isomorphic strings, longest common prefix (sorting method and optimized method), decoding an encoded string, maximum consecutive 1s with at most `k` flips using sliding window | `CPP-Basics/Strings` |

## 2.6 Linked Lists

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 37 | Singly linked list fundamentals | Node structure, head pointer, insertion at head, insertion at tail, insertion at kth position, update at kth position, deletion, traversal | `Linked-Lists/Singly-Linked-List` |
| 38 | Singly linked list problems 1 | Delete every alternate node, remove duplicates from a sorted list, print the list in reverse, reverse the list iteratively, reverse recursively, reverse nodes in groups of `k` | `Linked-Lists/Singly-Linked-List` |
| 39 | Singly linked list problems 2 | Compare two lists for equality, find intersection of two lists, remove the kth node from the end, merge two sorted lists, merge `k` sorted lists, find the middle node, detect a cycle, remove a cycle, check palindrome, rotate right by `k`, group odd and even indices, reorder the list, swap adjacent pairs | `Linked-Lists/Singly-Linked-List-Problems` |
| 40 | Doubly linked list | Node structure with `prev` and `next`, insertion at start, insertion at end, insertion at kth position, deletion from start, end, and kth position, reverse, palindrome check, delete nodes whose neighbors have the same value, critical points (local minima and maxima), pair sum in a sorted doubly linked list | `Linked-Lists/Doubly-Linked-List` |
| 41 | Circular linked list | Circular traversal, insertion and deletion at start and end, template-based node, STL `list` operations (`insert`, `erase`, `advance`, iterators) | `Linked-Lists/Circular-Linked-List` |

## 2.7 Stacks, Queues, and Deques

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 42 | Stack implementation | Stack using an array, stack using a linked list, stack using the STL `stack` container (`push`, `pop`, `top`, `empty`) | `Stacks-and-Queues/Stacks` |
| 43 | Stack operations | Copy one stack to another (iterative and recursive), insert at bottom (iterative and recursive), insert at any position, remove from bottom (iterative and recursive), reverse a stack (iterative and recursive) | `Stacks-and-Queues/Stacks` |
| 44 | Stack problems 1 | Balanced bracket sequence, next greater element, stock span problem, largest rectangle in a histogram | `Stacks-and-Queues/Stack-Problems` |
| 45 | Stack problems 2 | Min stack with `getMin()` in constant time, minimum brackets to remove to make a sequence balanced, prefix expression evaluation, infix expression evaluation, prefix to postfix conversion | `Stacks-and-Queues/Stack-Problems` |
| 46 | Queue implementation | Queue using a linked list, queue using an array, queue using the STL `queue` container (`push`, `pop`, `front`, `empty`), reversing a queue | `Stacks-and-Queues/Queues` |
| 47 | Deque and queue problems | STL `deque` (`push_back`, `pop_front`, `front`, `empty`), sliding window maximum, queue using stacks (two approaches), circular queue implementation | `Stacks-and-Queues/Deque` |

## 2.8 Sets, Maps, and Hashing

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 48 | Set | Ordered unique values in ascending or descending order, insertion, deletion, search, `find`, `count`, `lower_bound`, `upper_bound`, `size`, `empty`, `clear`, `max_size`, iterators, duplicate removal | `Hashing/Sets` |
| 49 | Set problems | Birthday invite list with unique names in lexicographical order, sum of common elements between two vectors, check whether a string contains all English alphabets | `Hashing/Sets` |
| 50 | Unordered set and multiset | `unordered_set`, `unordered_multiset`, `multiset`, average `O(1)` operations, duplicate handling, rehashing | `Hashing/Unordered-Sets` |
| 51 | Unordered set problems | Second smallest element, count of all possible marks in an examination | `Hashing/Unordered-Sets` |
| 52 | Hashing theory | Division method, mid-square method, digit folding method, multiplication method, collisions, open hashing, closed hashing, linear probing, quadratic probing, double hashing, load factor, rehashing | `Hashing/Hash-Basics` |
| 53 | Hash table implementation | Hash table using chaining with `vector<list<int>>`, `addKey`, `search`, `deleteKey` | `Hashing/Hash-Basics` |
| 54 | Maps | `map`, ordered key-value pairs, duplicate keys, insertion using `[]` and `insert`, iteration, reverse iteration, `find`, `count`, `erase` | `Hashing/Hash-Concepts` |
| 55 | Unordered maps and multimaps | `unordered_map`, `multimap`, `unordered_multimap`, duplicate keys, average and worst-case complexity | `Hashing/Hash-Concepts` |
| 56 | Hashing problems | Sum of repetitive elements, make all strings equal by moving characters, anagram check, isomorphic strings, pair sum, longest subarray with sum zero | `Hashing/Hash-Concepts`, `Hashing/Hashmaps` |

## 2.9 Backtracking

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 57 | Backtracking fundamentals | Recursive state exploration, marking and unmarking choices, pruning invalid paths | `Backtracking` |
| 58 | Backtracking problems | All permutations of a string in lexicographical order, rat in a maze, N-Queens, Sudoku solver, place `K` knights on an `M x N` board, knight's tour, combination sum with unique combinations | `Backtracking` |

## 2.10 Trees

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 59 | Binary tree fundamentals | Tree terminology (root, parent, child, sibling, leaf, height, level, subtree), types of trees (generic, binary, BST, AVL), node structure | `Trees/Binary-Tree` |
| 60 | Tree traversals | DFS preorder, inorder, postorder; BFS level order traversal | `Trees/Binary-Tree` |
| 61 | Binary tree problems 1 | Maximum depth, number of leaf nodes, right view, top view, bottom view, reverse level order traversal | `Trees/Binary-Tree` |
| 62 | Binary tree problems 2 | Path sum III, maximum path sum, construct a tree from preorder and inorder, construct a tree from preorder and postorder, flatten a tree into a linked list, time for a binary tree to be infected | `Trees/Binary-Tree-Problems` |
| 63 | Binary search tree fundamentals | BST property, insertion, searching, deletion, sorted array to balanced BST, lowest common ancestor, remove leaf nodes, inorder predecessor and successor | `Trees/Binary-Search-Tree` |
| 64 | BST problems | Validate a BST, check whether two BSTs are identical, construct a BST from preorder, largest subtree that is a BST, all unique BSTs using Catalan numbers, least greater element on the right | `Trees/BST-Problems` |
| 65 | Generic / N-ary tree | Node with a vector of children, preorder, inorder, postorder, BFS, next larger element, mirror the tree, serialize and deserialize an N-ary tree | `Trees/Generic-Trees` |

## 2.11 Heaps and Priority Queues

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 66 | Heap fundamentals | Binary heap as a complete binary tree, min heap, max heap, array representation, parent and child indices | `Heaps-and-Priority-Queues/Heaps` |
| 67 | Heap operations | Insertion, deletion, heapify, heap sort | `Heaps-and-Priority-Queues/Heaps` |
| 68 | Heap problems | Merge two binary max heaps, kth smallest element, sum of elements between the k1th and k2th smallest elements | `Heaps-and-Priority-Queues/Heaps` |
| 69 | Priority queue | STL `priority_queue` (`push`, `pop`, `top`), min heap and max heap comparators, kth largest element, k closest points to the origin, k operations on an array, task scheduler with cooldown, median of a stream | `Heaps-and-Priority-Queues/Priority-Queue` |

## 2.12 Greedy Algorithms

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 70 | Greedy fundamentals | Building a solution piece by piece, choosing the locally optimal option | `Greedy` |
| 71 | Greedy problems 1 | Fractional knapsack, meeting room scheduling, activity selection, job sequencing with deadlines, survive on an island, largest palindromic number by permuting digits | `Greedy` |
| 72 | Greedy problems 2 | Minimum cost to cut a board into squares, Huffman coding, smallest number with a given digit sum and number of digits | `Greedy` |

## 2.13 Dynamic Programming

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 73 | DP fundamentals | When to use DP, defining the state, finding the recurrence relation, storing computed results | `Dynamic-Programming/DP-Basics` |
| 74 | DP techniques | Top-down memoization, bottom-up tabulation, space optimization | `Dynamic-Programming/DP-Basics` |
| 75 | DP problems 1 | House robber, decode ways, frog jump with `k` jumps, coin change combinations | `Dynamic-Programming/DP-Basics` |
| 76 | DP problems 2 | Triangle minimum path sum, longest common subsequence, longest increasing subsequence, matrix chain multiplication | `Dynamic-Programming/DP-Problems` |
| 77 | DP problems 3 | Unique paths with obstacles, minimum path sum, unique paths, Catalan numbers and unique BSTs, regular expression matching, house robber III on a tree, maximal rectangle | `Dynamic-Programming/DP-Problems` |

## 2.14 Graphs

| # | Concept | What is covered | Folder in this project |
| --- | --- | --- | --- |
| 78 | Graph fundamentals | Directed and undirected graphs, weighted and unweighted graphs, multigraph, simple graph, complete graph, connected graph, degree, indegree, outdegree, path, cycle, tree, forest, connected components | `Graphs/Graph-Basics` |
| 79 | Graph representations | Adjacency list using `vector<list<int>>`, weighted adjacency list using `vector<list<pair<int, int>>>`, adjacency map using `vector<unordered_map<int, int>>` | `Graphs/Graph-Basics` |
| 80 | Graph traversal | DFS, BFS, path existence between two nodes, printing all paths, shortest path in an unweighted graph | `Graphs/Graph-Problems` |
| 81 | Graph grid problems | Connected components, number of islands, Pacific Atlantic water flow, rotting oranges using multi-source BFS | `Graphs/Graph-Problems` |
| 82 | Disjoint Set Union (DSU) | Union-find, parent array, path compression, union by rank, `log*n` complexity | `Graphs/Disjoint-Set` |
| 83 | Topological sorting | Kahn's algorithm using BFS, cycle detection using DFS, cycle detection using BFS, course schedule I and II, stones removal using DSU, count unreachable pairs, detect a cycle in a grid | `Graphs/Topological-Sort` |
| 84 | Minimum spanning tree | Tree, spanning tree, MST, Kruskal's algorithm with DSU, Prim's algorithm with priority queue, connecting cities, minimum cost to connect points | `Graphs/MST-Kruskal-Prims` |
| 85 | Shortest paths and graph coloring | Dijkstra's algorithm using a priority queue, 0-1 BFS for minimum cost to make a grid valid, bipartite graph check using BFS coloring | `Graphs/Dijkstra` |

---

## 2.15 Problem Patterns and C++ Foundations Added in the Coverage Review

See [PATTERNS.md](PATTERNS.md) for recognition cues, prerequisites, and common
pitfalls. Every example below also has a numbered place in the reading order.

| Example | Concept and purpose |
| --- | --- |
| [initialization and narrowing](CPP-Basics/Fundamentals/initialization_and_narrowing.cpp) | Zero/value initialization, brace initialization, narrowing conversions, and vector constructor overloads. |
| [maximum circular subarray](CPP-Basics/Array-Problems/maximum_circular_subarray.cpp) | Maximum circular subarray: maximum/minimum Kadane passes, wraparound complement, all-negative input. |
| [recurrences and invariants](Complexity-Analysis/Time-Complexity/recurrences_and_invariants.cpp) | Recurrence expansion, Master theorem cases and limits, and a loop-invariant proof. |
| [four sum](Techniques/Two-Pointers/four_sum.cpp) | 4Sum: two fixed indices, opposing pointers, duplicate skipping, and wide sums. |
| [subarray divisible by k](Techniques/Prefix-Sum/subarray_divisible_by_k.cpp) | Counting divisible subarray sums using normalized prefix remainders. |
| [longest subarray sum k](Techniques/Prefix-Sum/longest_subarray_sum_k.cpp) | Longest subarray with sum K using earliest prefix indices. |
| [minimum size subarray sum](Techniques/Sliding-Window/minimum_size_subarray_sum.cpp) | Minimum-size subarray with sum at least target for non-negative numbers. |
| [longest repeating character replacement](Techniques/Sliding-Window/longest_repeating_character_replacement.cpp) | Longest repeated-character substring after at most K replacements. |
| [subarrays with k distinct](Techniques/Sliding-Window/subarrays_with_k_distinct.cpp) | Count exactly K distinct values through atMost(K) minus atMost(K-1). |
| [reverse nodes k group](Linked-Lists/Singly-Linked-List-Problems/reverse_nodes_k_group.cpp) | Reverse complete groups of K nodes by rewiring links around a dummy head. |
| [sort list](Linked-Lists/Singly-Linked-List-Problems/sort_list.cpp) | Stable merge sort on a linked list using fast/slow splitting. |
| [sum of subarray minimums](Stacks-and-Queues/Stack-Problems/sum_of_subarray_minimums.cpp) | Contribution counting with previous strictly smaller and next smaller-or-equal boundaries. |
| [shortest subarray at least k](Stacks-and-Queues/Deque/shortest_subarray_at_least_k.cpp) | Shortest subarray with sum at least K, including negatives, using a monotonic prefix deque. |
| [subarray xor equals k](Hashing/Hashmaps/subarray_xor_equals_k.cpp) | Count subarrays with XOR K using prefix XOR frequencies. |
| [path sum iii](Trees/Binary-Tree-Problems/path_sum_iii.cpp) | Count downward paths from any starting node using prefix frequencies and DFS backtracking. |
| [nodes distance k](Trees/Binary-Tree-Problems/nodes_distance_k.cpp) | Nodes at distance K using parent links, node identity, and level-order BFS. |
| [smallest range k lists](Heaps-and-Priority-Queues/Priority-Queue/smallest_range_k_lists.cpp) | Smallest range covering K sorted lists using a min-heap and a running maximum. |
| [sliding window median](Heaps-and-Priority-Queues/Priority-Queue/sliding_window_median.cpp) | Sliding-window median using two balanced multisets with exact deletion. |
| [jump game ii](Greedy/jump_game_ii.cpp) | Minimum jumps using greedy reachable layers, with unreachable-input detection. |
| [candy](Greedy/candy.cpp) | Minimum candy allocation using left-to-right and right-to-left constraints. |
| [partition labels](Greedy/partition_labels.cpp) | Partition labels using last occurrences and the earliest valid cut. |
| [target sum](Dynamic-Programming/DP-Problems/target_sum.cpp) | Count signed assignments through a 0/1 subset-sum reduction, including zeros. |
| [interleaving string](Dynamic-Programming/DP-Problems/interleaving_string.cpp) | Interleaving strings with two-prefix DP and a rolling row. |
| [maximal square](Dynamic-Programming/DP-Problems/maximal_square.cpp) | Largest all-one square through top, left, and diagonal DP states. |
| [weighted job scheduling](Dynamic-Programming/DP-Problems/weighted_job_scheduling.cpp) | Weighted interval scheduling through finish-time sorting, binary search, and prefix DP. |
| [russian doll envelopes](Dynamic-Programming/DP-Problems/russian_doll_envelopes.cpp) | Two-dimensional strict nesting reduced to LIS using descending equal-width ties. |
| [surrounded regions](Graphs/Graph-Problems/surrounded_regions.cpp) | Capture enclosed regions by marking boundary-connected cells with multi-source BFS. |
| [minimum height trees](Graphs/Graph-Problems/minimum_height_trees.cpp) | Find tree centers by peeling leaf layers; distinguish centers from centroids. |
| [alien dictionary](Graphs/Topological-Sort/alien_dictionary.cpp) | Infer alphabet constraints with topological sorting, edge deduplication, prefix validation, and cycle detection. |
| [longest increasing path](Graphs/Topological-Sort/longest_increasing_path.cpp) | Longest increasing matrix path using DAG indegrees and topological BFS layers. |
| [accounts merge](Graphs/Disjoint-Set/accounts_merge.cpp) | Merge account components through shared emails with union by size and path compression. |
| [redundant connection](Graphs/Disjoint-Set/redundant_connection.cpp) | Detect the redundant undirected edge whose endpoints are already connected. |
| [value categories](Advanced-CPP/References-and-Memory/value_categories.cpp) | Lvalues, prvalues, xvalues, named rvalue references, move casts, and temporary lifetimes. |
| [iterator invalidation](Advanced-CPP/STL-Containers/iterator_invalidation.cpp) | Operation-specific iterator invalidation, erase-remove, and stable references across rehash. |
| [randomized set](Advanced-Data-Structures/randomized_set.cpp) | Expected O(1) insert/delete/random using a dense vector, index map, and uniform sampling. |
| [time based key value store](Advanced-Data-Structures/time_based_key_value_store.cpp) | Versioned key-value lookup with per-key timestamps and upper_bound. |
| [sum of distances in tree](Graphs/Graph-Problems/sum_of_distances_in_tree.cpp) | Sum of distances from every vertex with two-pass rerooting DP. |
| [wildcard matching](Dynamic-Programming/DP-Problems/wildcard_matching.cpp) | Whole-string wildcard matching with independent ? and * tokens and rolling-prefix DP. |
| [word break ii](Dynamic-Programming/Advanced-DP/word_break_ii.cpp) | Word Break II: enumerate all segmentations with memoized suffix sentences. |
| [stl toolkit](CPP-Basics/STL-Essentials/stl_toolkit.cpp) | Early library vocabulary before DSA exercises: containers, iterators, comparators, and algorithms. |
| [coordinate compression](Techniques/Coordinate-Compression/coordinate_compression.cpp) | Order-preserving ranks for sparse coordinates; numeric distances remain in the original coordinates. |

---

## Section 3 — Advanced C++, DSA, and System Design

## 3.1 Advanced C++ Language and Runtime

| Folder | What is covered |
| --- | --- |
| `Advanced-CPP/Structures-Union-Enum` | `struct`, `union`, `enum`, `enum class`, `typedef`, memory layout |
| `Advanced-CPP/References-and-Memory` | References, dynamic memory, pointer to pointer, function pointers |
| `Advanced-CPP/Preprocessor-Namespace` | Macros, conditional compilation, namespaces, `using` |
| `Advanced-CPP/Templates` | Function templates, class templates, specialization, variadic templates, fold expressions |
| `Advanced-CPP/Exception-Handling` | `try`/`catch`/`throw`, custom exceptions, stack unwinding, RAII |
| `Advanced-CPP/File-Handling` | `ifstream`, `ofstream`, text and binary files, error handling |
| `Advanced-CPP/STL-Algorithms` | `sort`, `binary_search`, lambdas, `next_permutation`, algorithm composition |
| `Advanced-CPP/STL-Containers` | `pair`, `tuple`, `list`, `forward_list`, `multiset`, `multimap`, iterators, `bitset` |
| `Advanced-CPP/Modern-CPP-Features` | `auto`, `decltype`/`decltype(auto)`, initializer lists, range-for, `constexpr`, smart pointers, move semantics, `std::function`, `<regex>`, structured bindings |
| `Advanced-CPP/Concurrency` | Threads, mutex, condition variables (`condition_variable`, `condition_variable_any`), atomics, `async`/`future`, thread pool, deadlock, `shared_mutex` reader-writer locks, latch/barrier/semaphore, `jthread`/`stop_token`, `thread_local` |
| `Advanced-CPP/Type-System-and-Casting` | `static_cast`, `dynamic_cast`, RTTI, `typeid`, type traits, slicing |
| `Advanced-CPP/Compilation-and-Linkage` | Storage duration, linkage, ODR, `inline`, `extern "C"`, header guards |
| `Advanced-CPP/Modern-CPP17-20` | `optional`, `variant`, `any`, `string_view`, filesystem, concepts and constraints, ranges, designated initializers, three-way comparison (`<=>`), coroutines, `span`, `<bit>`, `std::format`, `source_location`, `consteval`/`constinit`, template lambdas |
| `Advanced-CPP/Memory-and-Undefined-Behavior` | Alignment, padding, endianness, undefined behavior, strict aliasing, lifetime, RAII, type punning, integer overflow |
| `Advanced-CPP/Design-Patterns` | Singleton, factory, observer, strategy, decorator, adapter, builder, CRTP |
| `Advanced-CPP/Performance` | Cache locality, false sharing, small-string optimization, moves, `reserve`, virtual dispatch, branch prediction, arena allocator |

## 3.2 Core and Advanced DSA

| Folder | What is covered |
| --- | --- |
| `Searching/Linear-Search` | Linear search on sorted and unsorted arrays, best case, time and space complexity |
| `Searching/Advanced-Searching` | Ternary, exponential, interpolation, jump, Fibonacci, and sentinel search; binary search on the answer |
| `Sorting/Advanced-Sorting` | Odd-even transposition, cocktail shaker, comb, gnome, pancake, cycle, tree, bitonic, TimSort, IntroSort, and external merge sort |
| `Sorting/Shell-Sort` | Gap sequences, in-place sorting, complexity |
| `Sorting/Heap-Sort` | Binary heap, heapify, in-place O(n log n) sorting |
| `Divide-and-Conquer` | Divide-and-conquer design, merge sort, count inversions |
| `Trees/Trie` | Prefix tree, insert/search, word search II |
| `Trees/AVL-Tree` | Height-balanced BST, rotations, balance factors |
| `Trees/Red-Black-Tree` | Color rules, rotations, comparison with AVL |
| `Trees/Segment-Tree` | Range queries, point updates, lazy propagation |
| `Trees/Fenwick-Tree` | Binary indexed tree, prefix sums, point updates |
| `Trees/Sparse-Table` | Idempotent range queries, O(1) query after O(n log n) build |
| `String-Algorithms/Basic` | KMP, Z-algorithm, Rabin-Karp, Manacher, palindromic substrings |
| `Math-Algorithms/Basic-Math` | GCD/LCM, sieve, prime factorization, modular arithmetic, combinatorics, matrix exponentiation, happy number |
| `Techniques/Bit-Manipulation` | Bit tricks, single number, counting bits, missing number |
| `Techniques/Two-Pointers` | 3Sum, container with most water, trapping rain water |
| `Techniques/Sliding-Window` | Fixed and variable windows, longest substring without repeating characters |
| `Techniques/Prefix-Sum` | Prefix sums, difference arrays, subarray sum equals k, product except self |
| `Dynamic-Programming/Advanced-DP` | Knapsack, coin change, edit distance, partition equal subset, word break, bitmask DP, digit DP, tree DP |
| `Graphs/Graph-Traversal` | BFS, DFS, grid traversal, clone graph, word ladder |
| `Graphs/Shortest-Path` | Bellman-Ford, Floyd-Warshall, negative cycles |
| `Graphs/Graph-Advanced` | Bipartite check, cycle detection, bridges, articulation points, SCC, Euler path |
| `Graphs/Max-Flow` | Ford-Fulkerson, Edmonds-Karp, Dinic, min cut |
| `Advanced-Data-Structures` | Monotonic stack/queue, LRU, skip list, order-statistic tree, meeting rooms, LFU |
| `Techniques/Binary-Lifting` | Jump pointers, LCA, k-th ancestor |
| `Techniques/Sqrt-Decomposition` | Block decomposition, lazy blocks, range updates |
| `Techniques/Mo-Algorithm` | Offline range queries, block ordering, add/remove operations |
| `Techniques/Meet-in-the-Middle` | Split search space, subset sums, bidirectional search |
| `Complexity-Analysis/Amortized-Analysis` | Aggregate, accounting, and potential methods |
| `Algorithm-Design/Randomized-Algorithms` | Random pivots, reservoir sampling, hashing, expected complexity, Las Vegas vs Monte Carlo |
| `Algorithm-Design/Design-Paradigms` | Greedy, divide and conquer, DP, backtracking, branch and bound |
| `Trees/Advanced-Trees` | B-tree, B+ tree, k-d tree, interval tree, van Emde Boas tree, treap, splay tree, persistent segment tree |
| `String-Algorithms/Advanced` | Aho-Corasick, Boyer-Moore, suffix array, suffix automaton, palindromic tree, string hashing |
| `Math-Algorithms/Number-Theory` | Euler totient, CRT, Miller-Rabin, Pollard's rho |
| `Math-Algorithms/Geometry` | Convex hull, line intersection, closest pair of points |
| `Dynamic-Programming/DP-Optimization` | Convex hull trick, divide-and-conquer DP optimization |
| `Graphs/Advanced-Graph-Algorithms` | A* search, Borůvka MST, Johnson's algorithm, Kosaraju SCC, 2-SAT, Hopcroft-Karp bipartite matching, Hungarian assignment, Stoer-Wagner min cut, Hamiltonian path/TSP, min-cost max-flow, heavy-light decomposition |
| `Dynamic-Programming/Advanced-DP-Techniques` | SOS DP, broken-profile (plug) DP, monotone-queue DP, Knuth optimization |
| `Math-Algorithms/Advanced-Math` | FFT/NTT polynomial multiplication, Gaussian elimination, Nim and Sprague-Grundy game theory, XOR linear basis, Stirling numbers, Lucas theorem, Möbius inversion, big-integer arithmetic |

## 3.3 System-Design Data Structures

| Folder | What is covered |
| --- | --- |
| `System-Design/DSA-for-System-Design` | Consistent hashing, bloom filter, rate limiter, count-min sketch, HyperLogLog, ring buffer, heavy hitters, geohash/quadtree, URL shortener, TTL cache, Snowflake IDs, leaderboard |
| `System-Design/Advanced-System-Design` | Merkle tree, LSM tree with memtable and SSTables, inverted index with posting lists, vector clock, CRDT counter (G-Counter and PN-Counter) |
| `System-Design/Distributed-Patterns` | Routing, retry budgets, circuit state, idempotency, reliable queue, outbox, saga, versioned cache fills | System design / Failure models |

### System-design foundations and architecture

The [system-design learning path](System-Design/README.md) now separates concepts,
runnable local models, high-level designs, and implementation boundaries.

| Area | Concrete coverage | Reading |
| --- | --- | --- |
| Requirements and scale | Workloads, correctness, SLOs, estimates, Little's law, deadlines, interfaces | [Requirements and capacity](System-Design/Foundations/01_requirements_and_capacity.md) |
| Data and consistency | Indexes, SQL/key-value/document models, MVCC, isolation, replication, quorums, CAP, consensus, partitioning, CDC | [Storage and consistency](System-Design/Foundations/02_storage_and_consistency.md) |
| Caches and messaging | Cache policies/races, stampedes, queues/logs, acknowledgements, replay, idempotency, outbox/inbox, saga/2PC | [Caching and messaging](System-Design/Foundations/03_caching_and_messaging.md) |
| Reliable operations | Load balancing, bulkheads, circuit breakers, backpressure, observability, security, migrations, RPO/RTO, recovery | [Reliability and operations](System-Design/Foundations/04_reliability_security_and_operations.md) |
| Low-level design method | SOLID, ownership, domain invariants, interfaces, state machines, exception/concurrency contracts | [LLD method](System-Design/Foundations/05_low_level_design_method.md) |
| High-level design exercises | Shortener, feed, chat, video, checkout/payments, search, ride dispatch, file synchronization | [Eight walkthroughs](System-Design/Foundations/06_architecture_walkthroughs.md) |

### Executable failure and coordination patterns

| Lesson | Implemented scope |
| --- | --- |
| [replication quorums](System-Design/Advanced-System-Design/replication_quorums.cpp) | Fixed-replica read/write overlap, version selection, read repair, and uncertain partial writes. |
| [mvcc snapshot isolation](System-Design/Advanced-System-Design/mvcc_snapshot_isolation.cpp) | Versioned reads, staged writes, write-conflict rejection, and a write-skew demonstration. |
| [load balancer](System-Design/Distributed-Patterns/load_balancer.cpp) | Health-aware least-connections routing, round-robin ties, and ticketed completion. |
| [circuit breaker](System-Design/Distributed-Patterns/circuit_breaker.cpp) | Closed/open/half-open transitions, one probe, cooldown, and ignored stale-generation results. |
| [retry backoff](System-Design/Distributed-Patterns/retry_backoff.cpp) | Retry eligibility, capped exponential full jitter, attempt limits, and deadline budgets. |
| [idempotency keys](System-Design/Distributed-Patterns/idempotency_keys.cpp) | Customer-scoped payload-bound keys, saved results, and mutex-protected repeatable charges. |
| [reliable work queue](System-Design/Distributed-Patterns/reliable_work_queue.cpp) | Capacity admission, visibility leases, delivery tokens, ack, redelivery, and dead letters. |
| [transactional outbox](System-Design/Distributed-Patterns/transactional_outbox.cpp) | Atomic local order+outbox mutation, crash-after-publish replay, and idempotent local projection. |
| [saga order workflow](System-Design/Distributed-Patterns/saga_order_workflow.cpp) | Reservation/payment/fulfillment states, terminal retry safety, and pending compensation retry. |
| [cache aside versioning](System-Design/Distributed-Patterns/cache_aside_versioning.cpp) | TTL/negative caching, deletion versions, and version-floor rejection of stale refills. |

See [COVERAGE.md](System-Design/COVERAGE.md) for all 48 system-design examples
and their unsupported guarantees/features. A lease simulation is not consensus,
and an in-memory log does not by itself provide process-crash durability.

## 3.4 Low-Level Design

| Folder | What is covered |
| --- | --- |
| `System-Design/LLD-Problems` | Parking lot, elevator system, library management, ATM, vending machine, chess, snake and ladder, Splitwise, hotel booking, food delivery, ride sharing, logger, task scheduler, movie ticket booking, online shopping |

---

## Appendix — Folder-by-Folder Coverage Checklist

Every folder in the repository is listed below with the primary concept it
covers. This confirms that no folder or concept was missed.

| Folder | Primary concept | Main section |
| --- | --- | --- |
| `CPP-Basics/Pattern-Printing` | Pattern printing using loops | C++ / Control flow |
| `CPP-Basics/Number-Problems` | Loop-based numeric problems | C++ / Control flow |
| `CPP-Basics/Fundamentals` | C++ basics, operators, conditionals, loops, number systems | C++ |
| `CPP-Basics/Functions` | Functions, pass by value/reference, default parameters, references | C++ |
| `CPP-Basics/Arrays` | Array basics, traversal, linear search | DSA / Arrays |
| `CPP-Basics/Array-Problems` | Pair sum, triplets, unique element, second largest, rotation, frequency queries | DSA / Arrays |
| `CPP-Basics/Array-Problems` | Sort 0s and 1s, even-odd partition, sorted squares | DSA / Arrays |
| `CPP-Basics/Array-Problems` | Prefix sums, equal partition, range sum queries | DSA / Arrays |
| `CPP-Basics/Two-D-Arrays` | 2-D array input, transpose, matrix multiplication | C++ / DSA |
| `CPP-Basics/Two-D-Array-Problems` | Pascal's triangle | DSA / 2-D Arrays |
| `CPP-Basics/Two-D-Array-Problems` | Maximum 1s row, rotate matrix by 90 degrees | DSA / 2-D Arrays |
| `CPP-Basics/Two-D-Array-Problems` | Spiral traversal and spiral matrix generation | DSA / 2-D Arrays |
| `CPP-Basics/Two-D-Array-Problems` | Rectangle sum using prefix sums | DSA / 2-D Arrays |
| `Complexity-Analysis/Time-Complexity` | Time complexity analysis | DSA / Complexity |
| `Complexity-Analysis/Space-Complexity` | Space complexity analysis | DSA / Complexity |
| `Complexity-Analysis/Problems` | Complexity practice problems | DSA / Complexity |
| `CPP-Basics/Pointers` | Pointer basics, address-of, dereference | C++ / Pointers |
| `CPP-Basics/Pointers` | Pointer arithmetic, array-pointer, call by reference | C++ / Pointers |
| `CPP-Basics/Pointers` | Dangling, null, void, and wild pointers | C++ / Pointers |
| `Recursion/Basics` | Recursion fundamentals, factorial, Fibonacci | DSA / Recursion |
| `Recursion/Problems` | Power, optimized power, sum of digits | DSA / Recursion |
| `Recursion/Problems` | Print array, sum of array, maximum in array | DSA / Recursion |
| `Recursion/Problems` | Palindrome number, remove occurrences | DSA / Recursion |
| `Recursion/Problems` | Multiples, sequence without loops, alternate signs | DSA / Recursion |
| `Recursion/Problems` | Armstrong number, frog jump, GCD | DSA / Recursion |
| `Recursion/Problems` | Grid paths, subset sums, search in array | DSA / Recursion |
| `Recursion/Problems` | Phone keypad combinations, subsequences | DSA / Recursion |
| `Sorting/Bubble-Sort` | Bubble sort and optimized bubble sort | DSA / Sorting |
| `Sorting/Selection-Sort` | Selection sort | DSA / Sorting |
| `Sorting/Insertion-Sort` | Insertion sort | DSA / Sorting |
| `Sorting/Problems` | Move zeros, lexicographical sorting | DSA / Sorting |
| `CPP-Basics/Vectors` | Vector STL container | C++ / STL |
| `Sorting/Merge-Sort` | Merge sort | DSA / Sorting |
| `Sorting/Quick-Sort` | Quick sort | DSA / Sorting |
| `Sorting/Count-Sort` | Count sort | DSA / Sorting |
| `Sorting/Bucket-Sort` | Bucket sort | DSA / Sorting |
| `Sorting/Problems` | Quickselect, merge sorted arrays | DSA / Sorting |
| `Sorting/Radix-Sort` | Radix sort | DSA / Sorting |
| `Searching/Binary-Search` | Binary search, recursive search, first occurrence, square root | DSA / Searching |
| `Searching/Binary-Search-Problems` | First/last position, rotated sorted array | DSA / Searching |
| `Searching/Binary-Search-Problems` | Peak element, 2-D sorted matrix search | DSA / Searching |
| `Searching/Binary-Search-Problems` | Binary search on answer | DSA / Searching |
| `CPP-Basics/Strings` | String basics and string algorithms | C++ / DSA |
| `OOP` | Classes, constructors, inheritance, polymorphism, overloading | C++ / OOP |
| `Linked-Lists/Singly-Linked-List` | Singly linked list fundamentals and problems | DSA / Linked List |
| `Linked-Lists/Singly-Linked-List-Problems` | Advanced singly linked list problems | DSA / Linked List |
| `Linked-Lists/Doubly-Linked-List` | Doubly linked list and problems | DSA / Linked List |
| `Linked-Lists/Circular-Linked-List` | Circular linked list, templates, STL `list` | DSA / C++ |
| `Stacks-and-Queues/Stacks` | Stack implementation and stack operations | DSA / Stack |
| `Stacks-and-Queues/Stack-Problems` | Balanced brackets, next greater, stock span, histogram | DSA / Stack |
| `Stacks-and-Queues/Stack-Problems` | Min stack, expression evaluation, prefix/postfix | DSA / Stack |
| `Stacks-and-Queues/Queues` | Queue implementation and reverse queue | DSA / Queue |
| `Stacks-and-Queues/Deque` | Deque, sliding window maximum, queue using stacks, circular queue | DSA / Queue |
| `Hashing/Sets` | Set container and set problems | DSA / Sets |
| `Hashing/Unordered-Sets` | Unordered set, multiset, unordered multiset | DSA / Sets |
| `Hashing/Hash-Basics` | Hashing functions, collisions, chaining implementation | DSA / Hashing |
| `Hashing/Hash-Concepts` | Map, unordered map, multimap, unordered multimap | DSA / Hashing |
| `Hashing/Hashmaps` | Hashmap problem solving | DSA / Hashing |
| `Backtracking` | Permutations, rat in maze, N-Queens, Sudoku, knights, combination sum | DSA / Backtracking |
| `Trees/Binary-Tree` | Binary tree basics, traversals, views | DSA / Trees |
| `Trees/Binary-Tree-Problems` | Binary tree problem solving | DSA / Trees |
| `Trees/Binary-Search-Tree` | BST basics, insertion, deletion, LCA | DSA / Trees |
| `Trees/BST-Problems` | BST validation, construction, Catalan numbers | DSA / Trees |
| `Trees/Generic-Trees` | Generic / N-ary tree and problems | DSA / Trees |
| `Heaps-and-Priority-Queues/Heaps` | Heap fundamentals, heapify, heap sort, heap problems | DSA / Heaps |
| `Heaps-and-Priority-Queues/Priority-Queue` | Priority queue and priority queue problems | DSA / Heaps |
| `Greedy` | Greedy fundamentals, scheduling, job sequencing, and Huffman coding | DSA / Greedy |
| `Greedy` | Greedy problem solving and optimal merge patterns | DSA / Greedy |
| `Dynamic-Programming/DP-Basics` | DP fundamentals, memoization, tabulation, DP problems | DSA / DP |
| `Dynamic-Programming/DP-Problems` | DP problem solving | DSA / DP |
| `Dynamic-Programming/DP-Problems` | Advanced DP problem solving | DSA / DP |
| `Graphs/Graph-Basics` | Graph fundamentals and representations | DSA / Graphs |
| `Graphs/Graph-Problems` | DFS, BFS, all paths, shortest unweighted path | DSA / Graphs |
| `Graphs/Graph-Problems` | Connected components, islands, water flow, rotten oranges | DSA / Graphs |
| `Graphs/Disjoint-Set` | Disjoint Set Union (union-find) | DSA / Graphs |
| `Graphs/Topological-Sort` | Topological sorting, cycle detection, DSU applications | DSA / Graphs |
| `Graphs/MST-Kruskal-Prims` | Kruskal's and Prim's minimum spanning tree algorithms | DSA / Graphs |
| `Graphs/Dijkstra` | Dijkstra, 0-1 BFS, bipartite graph | DSA / Graphs |
| `Advanced-CPP/Structures-Union-Enum` | Structures, unions, enums, typedef | C++ |
| `Advanced-CPP/References-and-Memory` | References, dynamic memory, pointer to pointer, function pointers | C++ / Memory |
| `Advanced-CPP/Preprocessor-Namespace` | Preprocessor directives, macros, namespaces | C++ |
| `Advanced-CPP/Templates` | Function/class templates, specialization, variadic templates | C++ |
| `Advanced-CPP/Exception-Handling` | Exceptions, custom exceptions, RAII | C++ |
| `Advanced-CPP/File-Handling` | File streams, text and binary I/O | C++ |
| `Advanced-CPP/STL-Algorithms` | STL algorithms, lambdas, binary search, permutations | C++ / STL |
| `Advanced-CPP/STL-Containers` | Pair, tuple, list, forward_list, multiset, multimap, bitset | C++ / STL |
| `Advanced-CPP/Modern-CPP-Features` | Auto, decltype, initializer lists, range-for, constexpr, smart pointers, move semantics, std::function, regex | C++ |
| `Searching/Linear-Search` | Linear search and case analysis | DSA / Searching |
| `Searching/Advanced-Searching` | Ternary, exponential, interpolation, jump, Fibonacci, and sentinel search; binary search on the answer | DSA / Searching |
| `Sorting/Advanced-Sorting` | Odd-even transposition, cocktail shaker, comb, gnome, pancake, cycle, tree, bitonic, TimSort, IntroSort, and external merge sort | DSA / Sorting |
| `Sorting/Shell-Sort` | Shell sort and gap sequences | DSA / Sorting |
| `Sorting/Heap-Sort` | Heap sort | DSA / Sorting |
| `Divide-and-Conquer` | Divide and conquer, count inversions | DSA / Paradigms |
| `Trees/Trie` | Trie, prefix search, word search II | DSA / Strings |
| `Trees/AVL-Tree` | AVL tree and rotations | DSA / Trees |
| `Trees/Red-Black-Tree` | Red-black tree properties | DSA / Trees |
| `Trees/Segment-Tree` | Segment tree, lazy propagation | DSA / Trees |
| `Trees/Fenwick-Tree` | Binary indexed tree | DSA / Trees |
| `Trees/Sparse-Table` | Sparse table range queries | DSA / Range queries |
| `String-Algorithms/Basic` | KMP, Z-algorithm, Rabin-Karp, Manacher, palindromic substrings | DSA / Strings |
| `Math-Algorithms/Basic-Math` | GCD, sieve, factorization, modular arithmetic, combinatorics | DSA / Math |
| `Techniques/Bit-Manipulation` | Bit tricks and bitwise problems | DSA / Bits |
| `Techniques/Two-Pointers` | 3Sum, container with most water, trapping rain water | DSA / Two pointers |
| `Techniques/Sliding-Window` | Fixed/variable windows, replacement budgets, exactly-K counting | DSA / Sliding window |
| `Techniques/Prefix-Sum` | Prefix sums, differences, sum-K counting/longest length, prefix remainders | DSA / Prefix sums |
| `Dynamic-Programming/Advanced-DP` | Knapsack, coin change, edit distance, bitmask/digit/tree DP | DSA / DP |
| `Graphs/Graph-Traversal` | BFS, DFS, grids, clone graph, word ladder | DSA / Graphs |
| `Graphs/Shortest-Path` | Bellman-Ford, Floyd-Warshall | DSA / Graphs |
| `Graphs/Graph-Advanced` | Bipartite, bridges, articulation points, SCC, Euler path | DSA / Graphs |
| `Graphs/Max-Flow` | Ford-Fulkerson, Edmonds-Karp, Dinic | DSA / Graphs |
| `Advanced-Data-Structures` | Monotonic stack/queue, LRU, skip list, order-statistic tree, LFU | DSA / Advanced DS |
| `Techniques/Binary-Lifting` | Binary lifting and LCA | DSA / Trees |
| `Techniques/Sqrt-Decomposition` | Sqrt decomposition and lazy blocks | DSA / Range queries |
| `Techniques/Mo-Algorithm` | Mo's algorithm for offline queries | DSA / Range queries |
| `Techniques/Meet-in-the-Middle` | Meet-in-the-middle search | DSA / Paradigms |
| `Complexity-Analysis/Amortized-Analysis` | Amortized complexity analysis | DSA / Complexity |
| `Algorithm-Design/Randomized-Algorithms` | Randomized algorithms, reservoir sampling, and expected complexity | DSA / Paradigms |
| `Algorithm-Design/Design-Paradigms` | Greedy, divide and conquer, DP, backtracking | DSA / Paradigms |
| `Trees/Advanced-Trees` | B-tree, B+ tree, k-d tree, interval tree, van Emde Boas tree, treap, splay tree, persistent segment tree | DSA / Trees |
| `String-Algorithms/Advanced` | Aho-Corasick, Boyer-Moore, suffix array, suffix automaton, palindromic tree, string hashing | DSA / Strings |
| `Math-Algorithms/Number-Theory` | Totient, CRT, Miller-Rabin, Pollard's rho | DSA / Math |
| `Math-Algorithms/Geometry` | Convex hull, line intersection, closest pair | DSA / Geometry |
| `Dynamic-Programming/DP-Optimization` | Convex hull trick, divide-and-conquer DP | DSA / DP |
| `System-Design/DSA-for-System-Design` | Caching, hashing, sketches, rate limiting, IDs, leaderboards | System design |
| `Advanced-CPP/Concurrency` | Threads, mutexes, condition variables, atomics, futures, thread pool, deadlock, shared locks, latch/barrier/semaphore, stop tokens, thread-local storage | C++ / Concurrency |
| `Advanced-CPP/Type-System-and-Casting` | Casts, RTTI, type traits, slicing | C++ / Types |
| `Advanced-CPP/Compilation-and-Linkage` | Storage, linkage, ODR, extern C, header guards | C++ / Build |
| `Advanced-CPP/Modern-CPP17-20` | C++17/20 vocabulary types, `string_view`, filesystem, concepts, ranges, designated initializers, three-way comparison, coroutines, `std::format`, `source_location`, `consteval`/`constinit`, template lambdas | C++ |
| `Advanced-CPP/Memory-and-Undefined-Behavior` | Alignment, endianness, UB, aliasing, lifetime, RAII, type punning, integer overflow | C++ / Memory |
| `Advanced-CPP/Design-Patterns` | Singleton, factory, observer, strategy, decorator, CRTP | C++ / Design |
| `Advanced-CPP/Performance` | Cache locality, false sharing, SSO, moves, `reserve`/capacity, virtual dispatch, branch prediction, arena allocator | C++ / Performance |
| `Graphs/Advanced-Graph-Algorithms` | A*, Borůvka MST, Johnson, Kosaraju SCC, 2-SAT, Hopcroft-Karp, Hungarian assignment, Stoer-Wagner, Hamiltonian path/TSP, min-cost max-flow, heavy-light decomposition | DSA / Graphs |
| `Dynamic-Programming/Advanced-DP-Techniques` | SOS DP, broken-profile DP, monotone-queue DP, Knuth optimization | DSA / DP |
| `Math-Algorithms/Advanced-Math` | FFT/NTT, Gaussian elimination, game theory, XOR linear basis, Stirling numbers, Lucas theorem, Möbius inversion, big-integer arithmetic | DSA / Math |
| `System-Design/Advanced-System-Design` | Merkle tree, LSM tree, inverted index, vector clock, CRDT counter | System design |
| `System-Design/LLD-Problems` | Parking lot, elevator system, library management, ATM, vending machine, chess, snake and ladder, Splitwise, hotel booking, food delivery, ride sharing, logger, task scheduler, movie ticket booking, online shopping | System design / LLD |
| `CPP-Basics/STL-Essentials` | Early container and algorithm toolkit | C++ / STL |
| `Techniques/Coordinate-Compression` | Sparse coordinate ranks, deduplication, inverse mapping | DSA / Range queries |

---

## Notes

- Folder and concept names in this document correspond to the modular
  topics in this repository.
- A few language features are demonstrated in more than one place:
  dynamic memory allocation appears in references, memory, and linked lists;
  `const` appears in modern features and OOP; templates appear in template
  modules and generic containers. The tables point to the primary folder
  for each concept.
- Practice and problem files are organized with their respective topic.
- Coverage is broad and expandable. Source examples are educational; compiler
  checks and selected runtime checks do not prove every implementation correct
  for every input. Specialist topics can be revisited after their prerequisites.

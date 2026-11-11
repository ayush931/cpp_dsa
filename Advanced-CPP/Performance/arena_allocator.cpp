/**
 * Arena (bump) allocator
 *
 * An arena allocates from one large block and moves a pointer forward for each
 * request. It never frees individual objects; reset() releases everything at
 * once.
 *
 * Why it is fast:
 *   - allocation is a pointer bump, O(1)
 *   - no per-object metadata, no free list, no locking
 *   - excellent cache locality because objects are packed together
 *   - destruction is one reset instead of many deallocations
 *
 * Where it is used:
 *   - per-frame game allocations
 *   - parsers and compilers (AST nodes live until the parse ends)
 *   - request-scoped server memory
 *   - temporary scratch buffers in hot loops
 *
 * Tradeoffs:
 *   - individual objects cannot be freed
 *   - memory is wasted if object lifetimes differ a lot
 *   - reset() does not run destructors; use trivially destructible types or
 *     destroy them explicitly
 *   - the arena must be sized up front, or it needs a chain of blocks
 */

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <new>
#include <string>
#include <utility>
using namespace std;

class Arena {
    unique_ptr<byte[]> buffer;
    size_t capacity;
    size_t offset;

   public:
    explicit Arena(size_t bytes) {
        buffer = make_unique<byte[]>(bytes);
        capacity = bytes;
        offset = 0;
    }

    // Returns aligned storage for `bytes` bytes, or throws bad_alloc.
    // `alignment` must be a power of two.
    void *allocate(size_t bytes, size_t alignment) {
        uintptr_t current = reinterpret_cast<uintptr_t>(buffer.get()) + offset;
        uintptr_t aligned = (current + alignment - 1) & ~(alignment - 1);
        size_t padding = aligned - current;

        if (offset + padding + bytes > capacity) {
            throw bad_alloc();
        }

        void *result = buffer.get() + offset + padding;
        offset += padding + bytes;

        return result;
    }

    template <typename T, typename... Args>
    T *create(Args &&...args) {
        void *memory = allocate(sizeof(T), alignof(T));
        return new (memory) T(std::forward<Args>(args)...);
    }

    void reset() {
        offset = 0;  // destructors are not called
    }

    size_t used() const {
        return offset;
    }

    size_t remaining() const {
        return capacity - offset;
    }
};

struct Particle {
    float x;
    float y;
    float velocityX;
    float velocityY;

    Particle(float xValue, float yValue, float velocityXValue,
             float velocityYValue)
        : x(xValue),
          y(yValue),
          velocityX(velocityXValue),
          velocityY(velocityYValue) {}
};

int main() {
    Arena arena(1024);

    int *number = arena.create<int>(42);
    double *value = arena.create<double>(3.5);
    Particle *particle = arena.create<Particle>(1.0f, 2.0f, 0.5f, -0.5f);

    cout << *number << endl;                            // 42
    cout << *value << endl;                             // 3.5
    cout << particle->x << " " << particle->y << endl;  // 1 2

    cout << (arena.used() > 0) << endl;          // 1
    cout << (arena.remaining() < 1024) << endl;  // 1

    // Allocate many small objects without a per-object allocation.
    int *numbers =
        static_cast<int *>(arena.allocate(10 * sizeof(int), alignof(int)));

    for (int i = 0; i < 10; i++) {
        numbers[i] = i * i;
    }

    for (int i = 0; i < 10; i++) {
        cout << numbers[i] << " ";
    }
    cout << endl;
    // 0 1 4 9 16 25 36 49 64 81

    // Reset releases everything at once.
    arena.reset();
    cout << arena.used() << endl;       // 0
    cout << arena.remaining() << endl;  // 1024

    // A non-trivial type needs an explicit destructor before reset.
    string *text = arena.create<string>("arena string");
    cout << *text << endl;  // arena string
    destroy_at(text);       // destroy before reset

    arena.reset();

    return 0;
}

/**
 * Time complexity -> O(1) per allocation, O(1) reset
 * Space complexity -> O(capacity) for the block, plus alignment padding
 *
 * Special cases:
 *   - alignment: a type with alignof(T) > 1 needs the pointer rounded up
 *   - running out of space throws bad_alloc; a production arena may add a new
 *     block instead
 *   - reset() does not call destructors; leaking non-trivial resources is a
 *     real bug
 *   - a moved-from or copied arena must not double-free; this one is
 *     non-copyable because unique_ptr is non-copyable
 *   - thread safety: this arena is single-threaded; use one arena per thread
 *     or add synchronization
 *
 * Variants:
 *   - a chained arena keeps a list of blocks and allocates a new one when full
 *   - a pool allocator recycles fixed-size objects
 *   - a monotonic buffer resource (std::pmr::monotonic_buffer_resource) is the
 *     standard-library version of the same idea
 */

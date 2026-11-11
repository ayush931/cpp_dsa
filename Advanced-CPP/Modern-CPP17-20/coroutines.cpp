/**
 * Coroutines (C++20)
 *
 * A coroutine is a function that can suspend and resume while keeping its
 * local state. It uses three keywords:
 *   - co_await: suspend until another operation is ready
 *   - co_yield: produce a value and suspend
 *   - co_return: finish the coroutine
 *
 * The compiler rewrites the function around a `promise_type`. That promise
 * controls the return object, initial/final suspension, yielded values, and
 * exception handling.
 *
 * This example implements a lazy generator: nothing runs until the caller
 * asks for the first value.
 */

#include <coroutine>
#include <iostream>
using namespace std;

class Generator {
   public:
    struct promise_type {
        int currentValue = 0;

        Generator get_return_object() {
            return Generator{
                coroutine_handle<promise_type>::from_promise(*this)};
        }

        suspend_always initial_suspend() {
            return {};
        }

        suspend_always final_suspend() noexcept {
            return {};
        }

        void return_void() {}

        suspend_always yield_value(int value) {
            currentValue = value;
            return {};
        }

        void unhandled_exception() {
            terminate();
        }
    };

   private:
    coroutine_handle<promise_type> handle;

   public:
    Generator(coroutine_handle<promise_type> h) {
        handle = h;
    }

    Generator(const Generator &) = delete;
    Generator &operator=(const Generator &) = delete;

    ~Generator() {
        if (handle) {
            handle.destroy();
        }
    }

    // Resume until the next yield or until the coroutine finishes.
    bool next() {
        if (!handle || handle.done()) {
            return false;
        }

        handle.resume();

        return !handle.done();
    }

    int value() {
        return handle.promise().currentValue;
    }
};

Generator countTo(int limit) {
    for (int i = 1; i <= limit; i++) {
        co_yield i;
    }
}

Generator fibonacci(int count) {
    int first = 0;
    int second = 1;

    for (int i = 0; i < count; i++) {
        co_yield first;

        int next = first + second;
        first = second;
        second = next;
    }
}

int main() {
    Generator numbers = countTo(4);

    while (numbers.next()) {
        cout << numbers.value() << " ";
    }
    cout << endl;  // 1 2 3 4

    Generator fib = fibonacci(7);

    while (fib.next()) {
        cout << fib.value() << " ";
    }
    cout << endl;  // 0 1 1 2 3 5 8

    return 0;
}

/**
 * Time complexity -> O(1) per resume, O(n) for n yielded values
 * Space complexity -> O(1) per suspended coroutine frame
 *
 * Special cases: a generator that yields nothing, a coroutine destroyed before
 * it finishes (the destructor calls handle.destroy()), calling next() after
 * completion (returns false), move-only return objects, exceptions inside the
 * coroutine (route them through unhandled_exception()).
 */

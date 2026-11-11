// Smart-pointer preview
//
// Raw new/delete are error-prone: an early return or an exception can skip the
// delete. Smart pointers apply RAII, so the object is released automatically
// when the owner goes out of scope.
//
//   std::unique_ptr<T>  one owner, cannot be copied, can be moved
//   std::shared_ptr<T>  shared ownership with a reference count
//   std::weak_ptr<T>    observes a shared object without keeping it alive
//
// Example:
//   auto value = std::make_unique<int>(42);
//   auto first = std::make_shared<int>(7);
//   auto second = first; // use_count() becomes 2

#include <iostream>
#include <memory>
#include <string>
#include <utility>

struct Node {
    std::string name;
    int value;
};

int main() {
    std::unique_ptr<int> unique = std::make_unique<int>(42);
    std::cout << *unique << "\n";  // 42

    // std::unique_ptr<int> copy = unique; // error: unique ownership
    std::unique_ptr<int> moved = std::move(unique);
    std::cout << (unique == nullptr) << " " << *moved << "\n";  // 1 42

    std::shared_ptr<Node> first = std::make_shared<Node>(Node{"root", 7});
    std::cout << first->name << " " << first->value << " " << first.use_count()
              << "\n";  // root 7 1

    {
        std::shared_ptr<Node> second = first;
        std::cout << first.use_count() << "\n";  // 2
    }

    std::cout << first.use_count() << "\n";  // 1

    // weak_ptr does not increase the reference count.
    std::weak_ptr<Node> observer = first;
    std::cout << observer.expired() << "\n";  // 0

    if (auto locked = observer.lock()) {
        std::cout << locked->name << "\n";  // root
    }

    first.reset();
    std::cout << observer.expired() << "\n";  // 1

    return 0;
}

// Output:
// 42
// 1 42
// root 7 1
// 2
// 1
// 0
// root
// 1
//
// Time complexity: O(1) per ownership operation.
// Space complexity: O(1) per pointer, plus the managed object.
//
// Edge cases: a shared_ptr cycle keeps both objects alive forever; break the
// cycle with weak_ptr. Do not create two independent shared_ptrs from the same
// raw pointer. Prefer make_unique and make_shared over explicit new.

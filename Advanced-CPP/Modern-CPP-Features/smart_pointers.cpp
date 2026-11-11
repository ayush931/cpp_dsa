/**
 * Smart pointer -> automatically manages dynamically allocated memory
 * unique_ptr -> exclusive ownership, cannot be copied, can be moved
 * shared_ptr -> shared ownership with a reference count
 * weak_ptr   -> non-owning reference to a shared_ptr, breaks cycles
 * Defined in <memory>
 */

#include <iostream>
#include <memory>
using namespace std;

class Node {
   public:
    int value;

    Node(int v) {
        value = v;
        cout << "Node " << value << " created" << endl;
    }

    ~Node() {
        cout << "Node " << value << " destroyed" << endl;
    }
};

int main() {
    unique_ptr<int> u1 = make_unique<int>(10);
    cout << *u1 << endl;

    // unique_ptr<int> u2 = u1; -> error, unique_ptr cannot be copied
    unique_ptr<int> u2 = move(u1);
    cout << *u2 << endl;

    shared_ptr<int> s1 = make_shared<int>(20);
    shared_ptr<int> s2 = s1;
    cout << s1.use_count() << endl;  // 2

    s2.reset();
    cout << s1.use_count() << endl;  // 1

    weak_ptr<int> w = s1;
    cout << w.expired() << endl;  // 0
    if (auto sp = w.lock()) {
        cout << *sp << endl;
    }

    {
        unique_ptr<Node> n = make_unique<Node>(5);
    }  // destroyed automatically here

    return 0;
}

// Prefer make_unique and make_shared over raw new
// Smart pointers call delete automatically, so they prevent memory leaks

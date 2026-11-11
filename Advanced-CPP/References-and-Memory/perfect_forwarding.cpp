/**
 * Perfect Forwarding and Universal (Forwarding) References in Modern C++
 *
 * Concepts Covered:
 *   1. Forwarding References (Universal References):
 *      - Occurs in template parameter deduction: `template <typename T> void
 * func(T&& arg)`.
 *      - When an lvalue of type X is passed, T is deduced as X& -> arg becomes
 * X& (lvalue ref).
 *      - When an rvalue of type X is passed, T is deduced as X  -> arg becomes
 * X&& (rvalue ref).
 *
 *   2. Reference Collapsing Rules:
 *      In C++, references to references are not allowed in syntax, but occur
 * during type deduction and typedef expansion. They collapse according to: &  +
 * &  -> &   (lvalue ref to lvalue ref is lvalue ref) &  + && -> &   (rvalue ref
 * to lvalue ref is lvalue ref)
 *        && + &  -> &   (lvalue ref to rvalue ref is lvalue ref)
 *        && + && -> &&  (rvalue ref to rvalue ref is rvalue ref)
 *      Rule of thumb: "An lvalue reference always wins."
 *
 *   3. std::move vs std::forward:
 *      - std::move<T>(x) unconditionally casts 'x' to an rvalue reference
 * (X&&).
 *      - std::forward<T>(x) conditionally casts 'x' to an rvalue reference IF
 * AND ONLY IF T is deduced as a non-reference (i.e. was originally an rvalue).
 *
 *   4. Applications:
 *      - std::make_unique / std::make_shared
 *      - std::vector::emplace_back
 *      - Transparent wrapper functions / decorator patterns
 */

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// A tracker class that logs constructor and copy/move calls
class HeavyResource {
   private:
    string name;
    vector<int> data;

   public:
    // Default constructor
    HeavyResource() : name("Default"), data(100, 1) {
        cout << "  [HeavyResource] Default constructor" << endl;
    }

    // Parameterized constructor
    explicit HeavyResource(string n) : name(std::move(n)), data(100, 1) {
        cout << "  [HeavyResource] Parameterized constructor for: " << name
             << endl;
    }

    // Copy constructor
    HeavyResource(const HeavyResource &other)
        : name(other.name), data(other.data) {
        cout << "  [HeavyResource] COPY constructor called for: " << name
             << endl;
    }

    // Move constructor
    HeavyResource(HeavyResource &&other) noexcept
        : name(std::move(other.name)), data(std::move(other.data)) {
        cout << "  [HeavyResource] MOVE constructor called for: " << name
             << endl;
    }

    const string &getName() const {
        return name;
    }
};

// Target receiver functions with lvalue and rvalue overloads
void process(const HeavyResource &res) {
    cout << "  ==> process(const HeavyResource&) [LVALUE handler] for: "
         << res.getName() << endl;
}

void process(HeavyResource &&res) {
    cout << "  ==> process(HeavyResource&&) [RVALUE handler] for: "
         << res.getName() << endl;
}

// -------------------------------------------------------------
// BAD WRAPPER: Passes by value (unnecessary copy/move)
// -------------------------------------------------------------
template <typename T>
void badWrapperByValue(T arg) {
    cout << "Entering badWrapperByValue..." << endl;
    process(arg);  // Inside the function, 'arg' is always an lvalue!
}

// -------------------------------------------------------------
// BAD WRAPPER: Takes by universal reference, but passes without std::forward
// -------------------------------------------------------------
template <typename T>
void badWrapperNoForward(T &&arg) {
    cout << "Entering badWrapperNoForward..." << endl;
    // Bug: Even if arg bound to an rvalue, the named variable 'arg' has an
    // identity and is thus an LVALUE expression inside the function body!
    process(arg);
}

// -------------------------------------------------------------
// PERFECT WRAPPER: Takes forwarding reference and uses std::forward<T>
// -------------------------------------------------------------
template <typename T>
void perfectWrapper(T &&arg) {
    cout << "Entering perfectWrapper..." << endl;
    // std::forward preserves the exact value category:
    // - If T is HeavyResource&, std::forward yields an lvalue reference
    // - If T is HeavyResource,  std::forward yields an rvalue reference
    process(std::forward<T>(arg));
}

// -------------------------------------------------------------
// Variadic Factory Function demonstrating emplace / make_unique pattern
// -------------------------------------------------------------
template <typename T, typename... Args>
unique_ptr<T> makeCustom(Args &&...args) {
    cout << "Creating object via variadic perfect forwarding..." << endl;
    return make_unique<T>(std::forward<Args>(args)...);
}

int main() {
    cout << "=== 1. Value Category Deduction & Forwarding ===" << endl;

    cout << "\nCreating an lvalue instance:" << endl;
    HeavyResource item("ResourceA");

    cout << "\n--- Testing with LVALUE ---" << endl;
    cout << "Direct call:" << endl;
    process(item);  // Calls lvalue overload

    cout << "\nBad wrapper without forward (with lvalue):" << endl;
    badWrapperNoForward(item);  // Correct by coincidence: calls lvalue overload

    cout << "\nPerfect wrapper (with lvalue):" << endl;
    perfectWrapper(
        item);  // Correctly calls lvalue overload without extra copies!

    cout << "\n--- Testing with RVALUE (Temporary Object) ---" << endl;
    cout << "Direct call with temporary:" << endl;
    process(HeavyResource("Temp1"));  // Calls rvalue overload

    cout << "\nBad wrapper without forward (with temporary):" << endl;
    badWrapperNoForward(HeavyResource("Temp2"));
    // Fails! Named 'arg' treats temporary as lvalue, calling lvalue handler!

    cout << "\nPerfect wrapper with std::forward (with temporary):" << endl;
    perfectWrapper(HeavyResource("Temp3"));
    // Success! Correctly forwards temporary to rvalue handler!

    cout << "\n=== 2. Variadic Factory Construction (Emplace Pattern) ==="
         << endl;
    string label = "DynamicEntity";
    auto ptr = makeCustom<HeavyResource>(label);
    cout << "Created: " << ptr->getName() << endl;

    return 0;
}

/**
 * Summary of Rules:
 *   - Function signature: `T&&` with deduced `T` is a Forwarding Reference.
 *   - Signature: `std::vector<T>&&` or `const T&&` is NOT a forwarding
 * reference; it is strictly an rvalue reference.
 *   - Always use `std::forward<T>(param)` when forwarding parameters to another
 * function.
 *   - Use `std::move(param)` when you want to unconditionally steal ownership
 * from a local object.
 */

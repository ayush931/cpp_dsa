/**
 * std::source_location (C++20)
 *
 * A small value type that describes a point in the source code:
 *   - file_name()
 *   - function_name()
 *   - line()
 *   - column()
 *
 * The usual pattern is a default argument:
 *
 *   void log(const string &message,
 *            source_location location = source_location::current());
 *
 * The default argument is evaluated at the call site, so the location points
 * to the caller, not to the log function. This replaces the __FILE__ and
 * __LINE__ macros and works with templates and default arguments.
 *
 * The values are implementation-defined but are stable for a given build.
 * They are useful for logging, assertions, test diagnostics, and error objects.
 */

#include <iostream>
#include <source_location>
#include <string>
using namespace std;

void logMessage(const string &message,
                source_location location = source_location::current()) {
    cout << location.file_name() << ":" << location.line() << " in "
         << location.function_name() << " -> " << message << endl;
}

void assertTrue(bool condition, const string &expression,
                source_location location = source_location::current()) {
    if (condition) {
        return;
    }

    cout << "assertion failed: " << expression << " at " << location.file_name()
         << ":" << location.line() << endl;
}

// A function can also capture its own location explicitly.
source_location whereAmI() {
    return source_location::current();
}

template <typename T>
void printType(const T &value,
               source_location location = source_location::current()) {
    cout << "value=" << value << " at line " << location.line() << endl;
}

int main() {
    logMessage("starting");  // points to this line
    logMessage("another");   // points to this line

    assertTrue(1 + 1 == 2, "1 + 1 == 2");
    assertTrue(1 + 1 == 3, "1 + 1 == 3");  // prints the failure location

    source_location here = whereAmI();
    cout << here.line() << endl;

    printType(42);
    printType(string("text"));

    // The location can be stored and passed around.
    source_location saved = source_location::current();
    cout << saved.line() << endl;

    return 0;
}

/**
 * Time complexity -> O(1); the compiler embeds the location as constants
 * Space complexity -> O(1), usually a few pointers and integers
 *
 * Special cases:
 *   - function_name() may be an empty string or an implementation-specific
 *     signature, depending on the compiler
 *   - the file name is the path as the compiler saw it, not necessarily an
 *     absolute path
 *   - a source_location captured in a default argument belongs to the caller
 *   - storing a source_location is safe; it does not point into a temporary
 *   - some compilers only support it in C++20 mode
 *
 * Before C++20, the same idea needed macros such as __FILE__ and __LINE__, or
 * a library like Boost.Stacktrace. std::source_location is cheaper and type
 * safe, and it composes with default arguments and templates.
 */

/**
 * std::format (C++20)
 *
 * A type-safe, extensible replacement for printf and stream formatting.
 *
 *   std::format("{} {}", a, b)       -> returns a std::string
 *   std::format_to(out, "...", ...)  -> writes to an output iterator
 *   std::format_to_n(out, n, ...)    -> writes at most n characters
 *   std::formatted_size(...)         -> size of the result without building it
 *
 * Format spec: [[fill]align][sign][#][0][width][.precision][type]
 *   alignment -> < (left), > (right), ^ (center)
 *   types -> d, x, X, o, b, f, e, g, s, p, ...
 *
 * Custom types specialize std::formatter<T>. The parse() member reads the
 * format spec, and format() writes the value.
 */

#include <format>
#include <iostream>
#include <iterator>
#include <string>
using namespace std;

struct Point {
    int x;
    int y;
};

// Custom formatter: allow std::format("{}", point).
template <>
struct std::formatter<Point> {
    constexpr auto parse(format_parse_context &context) {
        return context.begin();  // no custom options; accept the closing brace
    }

    auto format(const Point &point, format_context &context) const {
        return std::format_to(context.out(), "({}, {})", point.x, point.y);
    }
};

int main() {
    cout << format("{} + {} = {}\n", 2, 3, 5);
    cout << format("{} {}\n", "hello", "world");

    // Alignment and width.
    cout << format("|{:>10}|{:<10}|{:^10}|\n", "right", "left", "center");

    // Numbers.
    cout << format("{:.2f}\n", 3.14159);  // 3.14
    cout << format("{:x} {:X} {:b} {:o}\n", 255, 255, 255, 255);
    // ff FF 11111111 377
    cout << format("{:+d} {:+d}\n", 42, -42);  // +42 -42
    cout << format("{:08.2f}\n", 3.14159);     // 00003.14

    // Custom type.
    cout << format("{}\n", Point{3, 4});  // (3, 4)

    // Format into a string.
    string message = format("user={} score={}", "ayush", 100);
    cout << message << endl;  // user=ayush score=100

    // Format into an existing buffer.
    string buffer;
    format_to(back_inserter(buffer), "[{}:{}]", 7, 9);
    cout << buffer << endl;  // [7:9]

    // Size without allocating the result.
    cout << formatted_size("{} {}", "abc", 12345) << endl;  // 9

    return 0;
}

/**
 * Time complexity -> O(length of the formatted output)
 * Space complexity -> O(length of the formatted output) for std::format
 *
 * Special cases:
 *   - a missing argument throws std::format_error
 *   - an invalid format spec throws std::format_error
 *   - `{}` uses the default format for the type
 *   - precision on an integer is an error; use width instead
 *   - `std::format` is not a drop-in replacement for printf's `%` syntax
 *   - compile-time checking is available through std::format_string; use
 *     std::vformat for runtime format strings
 *
 * Compared with iostreams:
 *   - no manipulator state leaks between calls
 *   - much faster in typical implementations
 *   - the format string is checked at compile time when it is a literal
 *   - custom types opt in with a formatter specialization
 */

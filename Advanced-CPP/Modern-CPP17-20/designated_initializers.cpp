/**
 * Designated initializers (C++20) -> initialize members by name
 *
 * Point p{.x = 1, .y = 2};
 *
 * Rules:-
 * - Designators must follow the declaration order
 * - You cannot mix designated and positional initialization
 * - Every member without a designator is value-initialized
 * - Works for aggregates (no user-declared constructors)
 *
 * Benefit: adding a new member does not silently shift positional arguments
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;
    int z;
};

struct Config {
    string host = "localhost";
    int port = 8080;
    bool secure = false;
    int timeoutSeconds = 30;
};

struct Employee {
    string name;
    int age;
    string department;
};

int main() {
    Point origin{};
    Point point{.x = 3, .y = 4};  // z is value-initialized to 0

    cout << point.x << " " << point.y << " " << point.z << endl;  // 3 4 0

    // defaults can be overridden by name
    Config defaultConfig;
    Config production{.host = "api.example.com", .port = 443, .secure = true};

    cout << defaultConfig.host << ":" << defaultConfig.port
         << endl;  // localhost:8080
    cout << production.host << ":" << production.port
         << endl;  // api.example.com:443

    vector<Employee> employees = {
        {.name = "alice", .age = 30, .department = "engineering"},
        {.name = "bob", .age = 25, .department = "design"}};

    for (Employee &employee : employees) {
        cout << employee.name << " " << employee.department << endl;
    }

    // arrays of aggregates
    Point triangle[] = {{.x = 0, .y = 0}, {.x = 4, .y = 0}, {.x = 0, .y = 3}};

    cout << triangle[1].x << " " << triangle[2].y << endl;  // 4 3

    return 0;
}

// Special cases: out-of-order designators are an error;
// mixing {.x = 1, 2} is an error; a union can have only one designator;
// a non-aggregate (user-declared constructor) cannot use designators;
// nested designators like {.inner = {.value = 1}} are allowed

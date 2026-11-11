/**
 * Header guards and include hygiene
 *
 * A header can be included more than once through different paths, which
 * redefines classes and functions. Two common guards:
 *
 *   #ifndef MY_HEADER_H
 *   #define MY_HEADER_H
 *   ...
 *   #endif
 *
 *   #pragma once   // non-standard but supported by all major compilers
 *
 * Include order:-
 * 1. The matching header for this .cpp
 * 2. Other project headers
 * 3. Standard library headers
 *
 * Forward declaration -> declare a class without including its header,
 * enough for pointers and references, reduces compile time
 *
 * Include what you use -> include the header for every type you name,
 * do not rely on transitive includes
 */

#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

// A forward declaration is enough here because only a pointer is stored
class Database;

class Service {
    Database *database;  // pointer, no complete type needed

   public:
    Service() {
        database = nullptr;
    }

    void setDatabase(Database *db) {
        database = db;
    }

    bool hasDatabase() {
        return database != nullptr;
    }
};

// The complete definition is needed when the object is used by value
class Database {
    string name;

   public:
    Database(string n) {
        name = n;
    }

    string getName() {
        return name;
    }
};

int main() {
    Service service;
    cout << service.hasDatabase() << endl;  // 0

    Database database("users");
    service.setDatabase(&database);

    cout << service.hasDatabase() << endl;  // 1
    cout << database.getName() << endl;     // users

    // unique_ptr to an incomplete type needs a complete type at destruction,
    // so the destructor must be defined where the type is complete (pimpl
    // idiom)
    vector<unique_ptr<Database>> databases;
    databases.push_back(make_unique<Database>("orders"));

    cout << databases[0]->getName() << endl;  // orders

    return 0;
}

// Special cases: the same header included twice without a guard ->
// redefinition; two headers with the same guard macro -> one is silently
// skipped; cyclic includes -> one class ends up incomplete, use forward
// declarations; #pragma once relies on file identity, which can fail with
// symlinks or generated copies of the same header

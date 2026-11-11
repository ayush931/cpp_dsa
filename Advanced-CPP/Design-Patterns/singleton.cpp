/**
 * Singleton -> exactly one instance of a class with a global access point
 *
 * Meyers singleton (C++11 and later):-
 * - Function-local static -> initialized on first use
 * - Initialization is thread-safe by the standard
 * - Destroyed at program exit
 *
 * The copy constructor and assignment are deleted so nobody can clone it.
 *
 * Problems: global state, hidden dependencies, hard to test, lifetime issues
 * at shutdown. Prefer passing a dependency explicitly unless a single shared
 * resource is truly required (logger, config, thread pool).
 */

#include <iostream>
#include <mutex>
#include <string>
using namespace std;

class Logger {
    string prefix;
    int messages;

    Logger() {
        prefix = "[app]";
        messages = 0;
    }

   public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    static Logger &getInstance() {
        static Logger instance;  // thread-safe lazy initialization
        return instance;
    }

    void log(const string &message) {
        messages++;
        cout << prefix << " " << message << " (#" << messages << ")" << endl;
    }

    int count() const {
        return messages;
    }
};

// Eager variant: created before main, used when startup cost is acceptable
class Configuration {
    static Configuration instance;

    Configuration() {}

   public:
    Configuration(const Configuration &) = delete;
    Configuration &operator=(const Configuration &) = delete;

    static Configuration &getInstance() {
        return instance;
    }

    string environment = "production";
};

Configuration Configuration::instance;

int main() {
    Logger &first = Logger::getInstance();
    Logger &second = Logger::getInstance();

    cout << (&first == &second) << endl;  // 1

    first.log("starting");
    second.log("ready");

    cout << first.count() << endl;  // 2

    cout << Configuration::getInstance().environment << endl;  // production

    // Logger copy;   // error: copy constructor is deleted
    // Logger another = Logger::getInstance();   // error

    return 0;
}

// Special cases: static initialization order across translation units is
// unspecified (the function-local static avoids it); destruction order at
// exit can matter if other globals use the singleton; a singleton holding
// a mutex must not be used from a signal handler; in tests, replace the
// singleton with dependency injection; double-checked locking without atomics
// is broken

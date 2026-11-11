/**
 * Storage duration and linkage
 *
 * Storage duration -> how long an object lives
 *   automatic -> block scope, destroyed at the end of the block
 *   static -> lives for the whole program
 *   thread_local -> one instance per thread
 *   dynamic -> new / delete, lifetime controlled manually
 *
 * Linkage -> whether a name can be used from another translation unit
 *   external -> visible to other files (global non-static, extern)
 *   internal -> visible only in this translation unit (static global,
 *               anonymous namespace, const at namespace scope)
 *   none -> local variables
 *
 * static local -> initialized once, on first execution (thread-safe since
 * C++11) inline variable (C++17) -> one definition shared across translation
 * units
 */

#include <iostream>
#include <string>
using namespace std;

int globalCounter = 0;        // external linkage
static int fileCounter = 0;   // internal linkage
extern int definedElsewhere;  // declaration only

namespace {
int hiddenCounter = 0;  // internal linkage, preferred over static
}

class Logger {
   public:
    Logger(string message) {
        cout << "created: " << message << endl;
    }

    ~Logger() {
        cout << "destroyed" << endl;
    }
};

int &getSharedCounter() {
    static int counter = 0;  // created once, thread-safe initialization
    return counter;
}

void incrementFileCounter() {
    fileCounter++;
    hiddenCounter++;
}

thread_local int perThreadValue = 0;

int main() {
    globalCounter = 10;
    incrementFileCounter();
    incrementFileCounter();

    cout << globalCounter << " " << fileCounter << " " << hiddenCounter
         << endl;  // 10 2 2

    getSharedCounter()++;
    getSharedCounter()++;
    cout << getSharedCounter() << endl;  // 2

    // order of construction across translation units is unspecified;
    // a function-local static avoids the static initialization order fiasco
    {
        Logger logger("block");
        cout << "inside block" << endl;
    }

    perThreadValue = 5;
    cout << perThreadValue << endl;  // 5

    // const at namespace scope has internal linkage by default
    const int localConstant = 3;
    cout << localConstant << endl;

    return 0;
}

// Special cases: static local initialization is thread-safe, but destruction
// order at exit is still unspecified; extern without a definition fails at
// link time; a static member of a class needs a definition before C++17
// (use inline static since C++17); thread_local is per thread, not per task

// A dangling pointer points to memory whose lifetime has ended.
// Reading or writing through it is undefined behavior: the memory may be
// reused, so the old address can appear to "work" and still be wrong.

#include <iostream>
using namespace std;

int main() {
    int *ptr = nullptr;

    {
        int x = 10;
        ptr = &x;
        cout << *ptr << "\n";  // 10: x is still alive here
    }

    // x is destroyed at the end of the block, so ptr is now dangling.
    // Never dereference it. The safe options are to stop using it or to
    // reset it to nullptr.
    ptr = nullptr;

    if (ptr == nullptr) {
        cout << "ptr was reset, so it is safe to check and reuse\n";
    }

    int y = 20;
    ptr = &y;
    cout << *ptr << "\n";  // 20: valid again

    return 0;
}

// Output:
// 10
// ptr was reset, so it is safe to check and reuse
// 20

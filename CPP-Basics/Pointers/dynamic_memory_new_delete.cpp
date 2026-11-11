// Stack variables are freed automatically at the end of their scope.
// Heap memory lives until you free it yourself with delete or delete[].

#include <iostream>
using namespace std;

int main() {
    // ! Single value on the heap
    int *p = new int(25);
    cout << "*p = " << *p << endl;
    *p = 30;
    cout << "after update *p = " << *p << endl;
    delete p;     // release the memory
    p = nullptr;  // avoid keeping a dangling pointer around

    // ! Array on the heap, sized at run time
    int n = 5;
    int *arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = i * i;
    }

    cout << "\nheap array:";
    for (int i = 0; i < n; i++) {
        cout << " " << arr[i];
    }
    cout << endl;

    delete[] arr;  // arrays must be freed with delete[]
    arr = nullptr;

    // ! Every new needs exactly one matching delete.
    // ! Mixing them up (delete on an array, or deleting twice) is undefined
    // behavior.

    // ! Deleting nullptr is safe, so a null check is not needed before delete.
    int *none = nullptr;
    delete none;
    cout << "\ndeleting nullptr is safe" << endl;

    return 0;
}

// ! Rule of thumb:
// ! new  -> delete
// ! new[] -> delete[]
// ! Prefer std::vector or smart pointers (std::unique_ptr) over raw new/delete
// ! in real code. Raw new/delete is shown here because linked lists and trees
// ! create nodes this way.

// Stack memory -> automatic, freed when the scope ends
// Heap memory -> manual, allocated with new and freed with delete
// new returns a pointer; delete frees a single object; delete[] frees an array

#include <iostream>
using namespace std;

int main() {
    int *p = new int(10);
    cout << *p << endl;
    delete p;
    p = nullptr;

    int n = 5;
    int *arr = new int[n];
    for (int i = 0; i < n; i++) {
        arr[i] = i * i;
    }
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    delete[] arr;

    int rows = 3, cols = 4;
    int **matrix = new int *[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i + j;
        }
    }
    cout << matrix[2][3] << endl;

    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;

    // Memory leak -> allocated memory is never freed
    // Dangling pointer -> pointer still points to memory that has been freed
    // Double free -> deleting the same memory twice

    return 0;
}

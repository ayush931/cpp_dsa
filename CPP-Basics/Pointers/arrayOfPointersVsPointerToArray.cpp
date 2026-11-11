// Array of pointers vs pointer to an array
//
//   int *arrOfPtrs[3];    // array of 3 pointers to int
//   int (*ptrToArr)[3];   // one pointer to an array of 3 ints
//
// The parentheses in the second declaration are essential. Without them,
// int *ptrToArr[3] would again be an array of three pointers.
//
// Example:
//   int a = 1, b = 2, c = 3;
//   int *arrOfPtrs[3] = {&a, &b, &c};
//   int row[3] = {10, 20, 30};
//   int (*ptrToArr)[3] = &row;
//
// arrOfPtrs + 1 moves by one pointer; ptrToArr + 1 moves by one whole row.

#include <iostream>

int main() {
    int a = 1;
    int b = 2;
    int c = 3;

    int *arrOfPtrs[3] = {&a, &b, &c};

    std::cout << *arrOfPtrs[0] << " " << *arrOfPtrs[1] << " " << *arrOfPtrs[2]
              << "\n";  // 1 2 3

    std::cout << sizeof(arrOfPtrs) / sizeof(arrOfPtrs[0]) << "\n";  // 3

    int grid[2][3] = {{10, 20, 30}, {40, 50, 60}};
    int (*ptrToArr)[3] = grid;  // points to the first row

    std::cout << (*ptrToArr)[0] << " " << (*ptrToArr)[1] << " "
              << (*ptrToArr)[2] << "\n";  // 10 20 30

    // Pointer arithmetic on a pointer to an array skips an entire row.
    std::cout << ptrToArr[1][0] << " " << ptrToArr[1][1] << " "
              << ptrToArr[1][2] << "\n";  // 40 50 60

    // An array of pointers can point at unrelated objects.
    arrOfPtrs[1] = &c;
    std::cout << *arrOfPtrs[1] << "\n";  // 3

    return 0;
}

// Output:
// 1 2 3
// 3
// 10 20 30
// 40 50 60
// 3
//
// Time complexity: O(1) per access.
// Space complexity: O(1) extra.
//
// Edge cases: a pointer to an array has the type int (*)[N]; do not confuse it
// with int **, which is a pointer to a pointer. Use std::array or std::vector
// when the row length is not known at compile time.

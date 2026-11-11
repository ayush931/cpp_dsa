#include <iostream>
using namespace std;

int main() {
    int apples;
    cin >> apples;
    cout << "Number of apples: " << apples << endl;

    return 0;
}

// ? TypeVariable_name[=value]

/*

? 1 byte = 8 bits (smallest unit of storage)
? 1 byte = 2^8 combinations.

*/

// ! Rules for naming Variables in C++

/*

* Rule 1:- Variable names should not begain with a number

* Rule 2:- Whitespace is not permitted in variable names.

* Rule 3:- A C++ keyword (reserved word) cannot be used as a variable name.

* Rule 4:- It is preferred to use variable names with more than one word with
all lowercase letters for the first word and capitalization of the first letter
of the each subsequent word.

* Rule 5:- When creating variables, it is preferred to give them meaningful
names.

* Rule 6:- All lowercase letters should be used when creating one-word variable
names.

*/

// ! Identifiers:- Identifiers are name of any class, variables, methods, etc.

/*

* Rule 1:- All identifiers should begain with a letter (A to Z or a to z),
currency character($) or an underscore(_).

* Rule 2:- After the first character, identifiers can have any combination of
characters.

* Rule 3:- A keyword cannot be used as an identifiers.

* Rule 4:- The identifiers are case sensitive.

* Rule 5:- Whitespace is not permitted.

*/

// ! C++ Data Types

//? 1. Primary

/*
 * Integer:-  //? int a = 3; (4 bytes)
 * Character:- //? char name = 'a'; It should be write in single quote. (1 byte)
 * Boolean:- //? bool flag = true || false; (1 byte)
 * Floating Point:- //? For decimals, exponentials, float rate = 5.34 (upto 7
 * decimal places) and (4 bytes) Double Floating Point //? For decimals,
 * exponentials, (upto 15 decimal places) and (8 bytes) Void:- //? Null or
 * Nothing, It cannot be defined. Wide Character:- //? (2 bytes || 4 bytes)
 * String:- //? string name = "Ayush", It should be in double quote.
 */

//? 2. Derived

/*
 * Function
 * Array
 * Pointer
 * Reference
 */

//? 3. User Defined

/*
 * Class
 * Structure
 * Union
 * Enum
 * TypeDef
 */

//! Taking Input from user

/*
 * int apples;
 * cin >> apples;
 */
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str = "Ayush";
    string str1("Kumar");

    cout << str << str1 << endl;

    string str2;
    // cin >> str2; // take input until get space, tab or new line
    getline(cin, str2);  // take space, tab or new line

    cout << str2 << "\n";

    char ch = 'a';
    cout << int(ch) << endl;  // give ASCII value of character

    // string -> dynamic memory allocation, not able to waste memory
    // char -> static memory allocation, faster

    return 0;
}
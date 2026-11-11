#include <bits/stdc++.h>
using namespace std;

int main() {
    // reverse -> Time Complexity -> 0(length of string)
    string str = "Hello";
    reverse(str.begin(), str.end());
    cout << str << endl;

    // substr(start, end) -> substring -> Time Complexity -> 0(length of string)
    string str1 = "survive";
    cout << str1.substr(1, 4) << endl;
    cout << str1.substr(1) << endl;

    // + -> concatinate string
    string str2 = str1 + str;
    cout << str2 << endl;

    // strcat() -> concatinate char arrays
    char s1[20] = "Ayush";
    char s2[20] = "Kumar";
    strcat(s1, s2);
    cout << s1 << endl;

    // push_back() => append character at the end of string
    string st1 = "abcd";
    char e = 'e';
    st1.push_back(e);
    cout << st1 << endl;

    // size(), strlen() -> determine size of string 0(1) and character array
    // 0(n)
    string st2 = "abcde";
    cout << st2.size() << endl;
    char ch1[20] = "abcde";
    cout << strlen(ch1) << endl;

    // to_string() -> convert into string
    int num = 432;
    string numStr = to_string(num);
    numStr += "1";
    cout << numStr << endl;
    cout << numStr[1] << endl;

    return 0;
}
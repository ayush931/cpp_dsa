#include <iostream>
using namespace std;

int main() {
    char alphabet;
    cout << "Enter the alphabet: ";
    cin >> alphabet;

    switch (alphabet) {
        case 'a':
            cout << "Vowel" << endl;
            break;

        case 'e':
            cout << "Vowel" << endl;
            break;

        case 'i':
            cout << "Vowel" << endl;
            break;

        case 'o':
            cout << "Vowel" << endl;
            break;

        case 'u':
            cout << "Vowel" << endl;
            break;

        default:
            cout << "Consonants" << endl;
            break;
    }

    return 0;
}
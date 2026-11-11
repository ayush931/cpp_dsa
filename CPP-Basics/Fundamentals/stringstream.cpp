// stringstream -> treat a string like a stream
// istringstream -> read from a string
// ostringstream -> write to a string
// stringstream  -> both

// Used for: parsing words, splitting by delimiter, converting between types,
// formatting output, reading numbers mixed with text

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main() {
    // split a sentence into words
    string sentence = "the quick brown fox";
    istringstream input(sentence);

    string word;
    vector<string> words;

    while (input >> word) {
        words.push_back(word);
    }

    for (string &current : words) {
        cout << current << " ";
    }
    cout << endl;

    cout << words.size() << endl;  // 4

    // parse numbers separated by commas
    string data = "10,20,30,40";
    stringstream numbers(data);
    string token;
    int sum = 0;

    while (getline(numbers, token, ',')) {
        sum += stoi(token);
    }

    cout << sum << endl;  // 100

    // convert a string to a number and back
    string numberText = "12345";
    int number = stoi(numberText);
    cout << number + 1 << endl;  // 12346

    ostringstream output;
    output << "value=" << number << ", doubled=" << number * 2;
    cout << output.str() << endl;

    // mix text and numbers while reading
    string record = "alice 30 95.5";
    istringstream recordStream(record);

    string name;
    int age;
    double score;

    recordStream >> name >> age >> score;
    cout << name << " " << age << " " << score << endl;

    // formatted output
    ostringstream formatted;
    formatted << fixed << setprecision(2) << 3.14159;
    cout << formatted.str() << endl;  // 3.14

    formatted.str("");  // clear the content
    formatted.clear();  // clear error flags

    formatted << setw(5) << 42 << setw(5) << 7;
    cout << formatted.str() << endl;  // "   42    7"

    // detect a failed conversion
    istringstream badInput("abc");
    int value;

    if (!(badInput >> value)) {
        cout << "not a number" << endl;
    }

    return 0;
}

// Special cases: stoi throws std::invalid_argument on bad input and
// std::out_of_range on overflow; after a failed read the stream sets failbit,
// call clear() before reusing it; getline with a delimiter keeps empty tokens;
// >> skips whitespace, getline does not; stringstream is slower than manual
// parsing, use it when clarity matters more than speed

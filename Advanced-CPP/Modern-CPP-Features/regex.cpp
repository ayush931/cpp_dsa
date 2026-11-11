/**
 * Regular Expressions with <regex>
 *
 * C++11 provides regular expressions through <regex>:
 *   - std::regex_match:    the entire input must match
 *   - std::regex_search:   find a match anywhere in the input
 *   - std::regex_replace:  replace matches with a formatted string
 *   - std::sregex_iterator: iterate over every non-overlapping match
 *
 * The default grammar is ECMAScript. Capture groups are numbered from 1 and
 * are available through std::smatch. Replacement strings use `$1`, `$2`, ...
 *
 * Regular expressions are powerful for validation, extraction, tokenization,
 * and rewriting. They are not a parser for nested structures such as HTML or
 * balanced parentheses; use a dedicated parser for those.
 */

#include <iostream>
#include <regex>
#include <string>
#include <vector>
using namespace std;

bool isValidEmail(const string &email) {
    static const regex pattern(
        R"([A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,})");
    return regex_match(email, pattern);
}

bool isValidDate(const string &date) {
    static const regex pattern(R"((\d{4})-(\d{2})-(\d{2}))");
    return regex_match(date, pattern);
}

vector<string> tokenize(const string &text) {
    static const regex separator(R"([,;\s]+)");
    vector<string> tokens;

    sregex_token_iterator iterator(text.begin(), text.end(), separator, -1);
    sregex_token_iterator end;

    for (; iterator != end; ++iterator) {
        if (!iterator->str().empty()) {
            tokens.push_back(iterator->str());
        }
    }

    return tokens;
}

void printTokens(const vector<string> &tokens) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(tokens.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << "\"" << tokens[i] << "\"";
    }

    cout << "]" << endl;
}

int main() {
    string text =
        "Contact alice@example.com or bob.smith+news@mail.co.uk today.";

    regex emailPattern(R"([A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,})");
    smatch match;

    cout << "emails found:" << endl;
    string remaining = text;

    while (regex_search(remaining, match, emailPattern)) {
        cout << "  " << match.str() << " at offset "
             << (match.position() + (text.size() - remaining.size())) << endl;
        remaining = match.suffix().str();
    }

    cout << "valid emails:" << endl;
    vector<string> emails = {"alice@example.com", "bob.smith+news@mail.co.uk",
                             "not-an-email", "a@b.c", "user@localhost"};

    for (const string &email : emails) {
        cout << "  " << email << ": " << (isValidEmail(email) ? "yes" : "no")
             << endl;
    }

    string date = "2026-11-11";
    smatch dateMatch;

    if (regex_match(date, dateMatch, regex(R"((\d{4})-(\d{2})-(\d{2}))"))) {
        cout << "date parts: year=" << dateMatch[1]
             << ", month=" << dateMatch[2] << ", day=" << dateMatch[3] << endl;
    }

    cout << "valid date 2026-11-11: "
         << (isValidDate("2026-11-11") ? "yes" : "no") << endl;
    cout << "valid date 11-11-2026: "
         << (isValidDate("11-11-2026") ? "yes" : "no") << endl;

    string formatted =
        regex_replace(date, regex(R"((\d{4})-(\d{2})-(\d{2}))"), "$3/$2/$1");
    cout << "reformatted date: " << formatted << endl;

    string sentence = "one  two,three;four   five";
    cout << "tokens: ";
    printTokens(tokenize(sentence));

    string mixedCase = "Hello HELLO hello";
    regex hello(R"(hello)", regex::icase);
    int count = 0;

    for (sregex_iterator it(mixedCase.begin(), mixedCase.end(), hello), end;
         it != end; ++it) {
        count++;
    }

    cout << "case-insensitive hello count: " << count << endl;

    string numbers = "Order 42 has 7 items and costs 199 dollars.";
    regex numberPattern(R"(\d+)");
    vector<int> values;

    for (sregex_iterator it(numbers.begin(), numbers.end(), numberPattern), end;
         it != end; ++it) {
        values.push_back(stoi(it->str()));
    }

    cout << "numbers: ";

    for (int value : values) {
        cout << value << " ";
    }
    cout << endl;

    string phone = "Call 555-1234 or 555-9876.";
    string masked = regex_replace(phone, regex(R"(\d{3}-\d{4})"), "XXX-XXXX");
    cout << "masked phones: " << masked << endl;

    try {
        regex invalid("[");
        (void)invalid;
    } catch (const regex_error &error) {
        cout << "invalid regex error: " << error.what() << endl;
    }

    return 0;
}

// Expected output (abridged):
// emails found:
//   alice@example.com at offset 8
//   bob.smith+news@mail.co.uk at offset 29
// valid emails:
//   alice@example.com: yes
//   bob.smith+news@mail.co.uk: yes
//   not-an-email: no
//   a@b.c: no
//   user@localhost: no
// date parts: year=2026, month=11, day=11
// valid date 2026-11-11: yes
// valid date 11-11-2026: no
// reformatted date: 11/11/2026
// tokens: ["one", "two", "three", "four", "five"]
// case-insensitive hello count: 3
// numbers: 42 7 199
// masked phones: Call XXX-XXXX or XXX-XXXX.
// invalid regex error: ...
//
// Special cases: full match versus search, capture groups, case-insensitive
// matching, tokenization with empty fields, invalid patterns, and replacement
// with backreferences.

/**
 * Write a program to count the number of lines, words and characters in a file.
 */

#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <utility>
using namespace std;
namespace fs = std::filesystem;

struct TempDir {
    fs::path path;

    explicit TempDir(fs::path p) : path(std::move(p)) {
        fs::create_directories(path);
    }

    ~TempDir() {
        error_code ignored;
        fs::remove_all(path, ignored);
    }
};

int main() {
    TempDir temp(fs::temp_directory_path() / "cpp_line_count_demo");
    fs::path path = temp.path / "sample.txt";

    {
        ofstream out(path);
        out << "one two three\n";
        out << "four five\n";
    }

    ifstream fin(path);

    if (!fin) {
        cout << "Unable to open file" << endl;
        return 1;
    }

    int lines = 0, words = 0, characters = 0;
    bool inWord = false;
    char ch;

    while (fin.get(ch)) {
        characters++;

        if (ch == '\n') {
            lines++;
        }

        // isspace expects an unsigned char value (or EOF). A plain char may be
        // negative for non-ASCII bytes, which is undefined behavior.
        if (isspace(static_cast<unsigned char>(ch))) {
            inWord = false;
        } else if (!inWord) {
            words++;
            inWord = true;
        }
    }

    // Count the last line if it does not end with a newline
    if (characters > 0 && ch != '\n') {
        lines++;
    }

    cout << "Lines: " << lines << endl;            // 2
    cout << "Words: " << words << endl;            // 5
    cout << "Characters: " << characters << endl;  // 24

    return 0;
}

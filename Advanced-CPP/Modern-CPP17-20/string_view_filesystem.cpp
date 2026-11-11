/**
 * std::string_view (C++17) -> a non-owning view of a character sequence
 * Stores a pointer and a length, no allocation, no null terminator required
 * Cheap to copy and pass by value
 *
 * Danger: the view does not own the data. If the underlying string is
 * destroyed or reallocated, the view dangles.
 * Do not return a string_view to a local string.
 *
 * std::filesystem (C++17) -> path, directory iteration, file status,
 * create/remove directories, copy files, file size
 * Some operations throw std::filesystem_error; the *_noexcept overloads
 * take an error_code instead
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
using namespace std;
namespace fs = std::filesystem;

size_t countVowels(string_view text) {
    size_t count = 0;

    for (char ch : text) {
        char lower = static_cast<char>(tolower(static_cast<unsigned char>(ch)));

        if (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' ||
            lower == 'u') {
            count++;
        }
    }

    return count;
}

string_view firstWord(string_view text) {
    size_t position = text.find(' ');

    if (position == string_view::npos) {
        return text;
    }
    return text.substr(0, position);
}

int main() {
    string sentence = "the quick brown fox";
    string_view view = sentence;

    cout << view.size() << endl;        // 19
    cout << countVowels(view) << endl;  // 5
    cout << firstWord(view) << endl;    // the

    view.remove_prefix(4);  // skip "the "
    cout << view << endl;   // quick brown fox

    view.remove_suffix(4);  // skip " fox"
    cout << view << endl;   // quick brown

    // string_view does not allocate, so it is cheap in hot paths
    // but it must not outlive the source
    string temporary = "temporary";
    string_view danglingRisk = temporary;
    temporary += " data";  // may reallocate
    // danglingRisk may now point to freed memory

    // filesystem
    fs::path directory = fs::temp_directory_path() / "cpp_demo";
    fs::create_directories(directory);

    fs::path file = directory / "sample.txt";

    {
        ofstream output(file);
        output << "hello filesystem" << endl;
    }

    cout << fs::exists(file) << endl;     // 1
    cout << fs::file_size(file) << endl;  // 17
    cout << file.extension() << endl;     // .txt
    cout << file.filename() << endl;      // sample.txt
    cout << file.parent_path() << endl;   // .../cpp_demo

    for (const fs::directory_entry &entry : fs::directory_iterator(directory)) {
        cout << entry.path().filename() << endl;
    }

    fs::remove(file);
    fs::remove(directory);

    cout << fs::exists(file) << endl;  // 0

    return 0;
}

// Special cases: string_view of a temporary, view into a vector<char> that
// reallocates, embedded null characters (size() is authoritative),
// data() is not null-terminated, filesystem path encoding on Windows,
// permission errors, symlink loops during recursive iteration

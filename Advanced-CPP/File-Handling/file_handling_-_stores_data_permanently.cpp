/**
 * File handling -> stores data permanently on disk
 * ofstream -> writes to a file
 * ifstream -> reads from a file
 * fstream  -> reads and writes
 *
 * File modes:-
 * ios::in     -> open for reading
 * ios::out    -> open for writing (truncates by default)
 * ios::app    -> append at the end
 * ios::ate    -> move to the end after opening
 * ios::trunc  -> discard previous contents
 * ios::binary -> binary mode
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <utility>
using namespace std;
namespace fs = std::filesystem;

// Temporary directory whose destructor removes every file created by the demo.
// Keeping test files out of the repository makes the example self-contained.
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
    TempDir temp(fs::temp_directory_path() / "cpp_file_handling_demo");
    fs::path textPath = temp.path / "sample.txt";
    fs::path binaryPath = temp.path / "data.bin";

    ofstream fout(textPath);
    if (!fout) {
        cout << "Unable to open file" << endl;
        return 1;
    }
    fout << "Hello file" << endl;
    fout << 42 << endl;
    fout.close();

    ifstream fin(textPath);
    string line;
    while (getline(fin, line)) {
        cout << line << endl;
    }
    fin.close();

    fout.open(textPath, ios::app);
    fout << "Appended line" << endl;
    fout.close();

    int num = 10;
    ofstream bout(binaryPath, ios::binary);
    bout.write(reinterpret_cast<const char *>(&num), sizeof(num));
    bout.close();

    int readNum = 0;
    ifstream bin(binaryPath, ios::binary);
    bin.read(reinterpret_cast<char *>(&readNum), sizeof(readNum));
    bin.close();
    cout << readNum << endl;

    return 0;
}

// eof() -> true after an attempt to read past the end
// fail() -> true if an operation failed
// bad() -> true on a serious stream error
// good() -> true if no error has occurred

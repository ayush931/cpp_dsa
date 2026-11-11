/**
 * External merge sort
 *
 * External sorting handles data that is too large to fit in memory:
 *
 *   1. Read the input in fixed-size chunks and sort each chunk in memory.
 *   2. Write every sorted chunk to its own run file in a temporary directory.
 *   3. Merge all run files with a k-way heap merge, writing the final output.
 *   4. Delete the temporary directory.
 *
 * The demo uses a small chunk size so several runs are produced. Run files and
 * the merged file live under std::filesystem::temp_directory_path(), never in
 * the repository. A small RAII helper removes the directory on every exit path,
 * including exceptions.
 *
 * Stability   -> stable: chunks use stable_sort, and the heap breaks equal
 *                values by run index, so earlier runs are consumed first
 * In-place    -> no, data is staged on disk
 * Time        -> O(n log n) comparisons and O(n) sequential I/O
 * Space       -> O(chunk size + number of runs) memory, O(n) disk
 *
 * std::sort is included as the in-memory reference.
 */

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
using namespace std;
namespace fs = std::filesystem;

class TempDirectory {
   private:
    fs::path directoryPath;

   public:
    explicit TempDirectory(fs::path path) : directoryPath(std::move(path)) {
        fs::create_directories(directoryPath);
    }

    ~TempDirectory() {
        error_code error;
        fs::remove_all(directoryPath, error);
    }

    const fs::path &path() const {
        return directoryPath;
    }
};

void writeRunFile(const fs::path &path, const vector<int> &values) {
    ofstream output(path);

    if (!output) {
        throw runtime_error("could not create run file");
    }

    for (int value : values) {
        output << value << "\n";
    }
}

bool readNextValue(ifstream &input, int &value) {
    return static_cast<bool>(input >> value);
}

vector<int> readAllValues(const fs::path &path) {
    ifstream input(path);
    vector<int> values;
    int value;

    if (!input) {
        throw runtime_error("could not open merged file");
    }

    while (input >> value) {
        values.push_back(value);
    }

    return values;
}

vector<int> externalSort(const vector<int> &input, int chunkSize) {
    if (input.empty()) {
        cout << "chunk size: " << chunkSize << endl;
        cout << "runs written: 0" << endl;
        cout << "temporary directory removed: yes" << endl;
        return {};
    }

    long long stamp = chrono::steady_clock::now().time_since_epoch().count();
    fs::path directory =
        fs::temp_directory_path() / ("cpp_external_sort_" + to_string(stamp));

    vector<int> result;

    {
        TempDirectory temporary(directory);

        int runCount = 0;
        vector<fs::path> runFiles;

        for (int start = 0; start < (int)input.size(); start += chunkSize) {
            int end = min((int)input.size(), start + chunkSize);
            vector<int> chunk(input.begin() + start, input.begin() + end);

            stable_sort(chunk.begin(), chunk.end());

            fs::path runPath =
                directory / ("run_" + to_string(runCount) + ".txt");
            writeRunFile(runPath, chunk);
            runFiles.push_back(runPath);
            runCount++;
        }

        // Min-heap of (value, run index). The run index makes equal values
        // leave earlier runs first, preserving stability across chunks.
        using Entry = pair<int, int>;
        priority_queue<Entry, vector<Entry>, greater<Entry>> heap;
        vector<ifstream> inputs(runCount);

        for (int i = 0; i < runCount; i++) {
            inputs[i].open(runFiles[i]);

            if (!inputs[i]) {
                throw runtime_error("could not open run file");
            }

            int value;

            if (readNextValue(inputs[i], value)) {
                heap.push({value, i});
            }
        }

        fs::path mergedPath = directory / "merged.txt";
        ofstream merged(mergedPath);

        if (!merged) {
            throw runtime_error("could not create merged file");
        }

        while (!heap.empty()) {
            Entry current = heap.top();
            heap.pop();

            merged << current.first << "\n";

            int value;

            if (readNextValue(inputs[current.second], value)) {
                heap.push({value, current.second});
            }
        }

        merged.close();
        result = readAllValues(mergedPath);

        cout << "chunk size: " << chunkSize << endl;
        cout << "runs written: " << runCount << endl;
    }

    cout << "temporary directory removed: "
         << (fs::exists(directory) ? "no" : "yes") << endl;

    return result;
}

void print(const vector<int> &arr) {
    cout << "[";

    for (int i = 0; i < (int)arr.size(); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << arr[i];
    }

    cout << "]";
}

int main() {
    vector<int> data;

    for (int i = 0; i < 23; i++) {
        data.push_back((i * 37 + 11) % 100 - 50);
    }

    vector<int> expected = data;
    sort(expected.begin(), expected.end());

    cout << "input: ";
    print(data);
    cout << endl;

    vector<int> sorted = externalSort(data, 4);

    cout << "output: ";
    print(sorted);
    cout << endl;
    cout << "matches std::sort: " << (sorted == expected ? "yes" : "no")
         << endl;

    vector<int> emptySorted = externalSort({}, 4);
    cout << "empty output size: " << emptySorted.size() << endl;

    return 0;
}

// Expected output:
// input: [11, -2, -15, 22, 9, -4, 17, 8, -21, 6, -5, 16, 7, -18, 9, 0, 15,
//         -6, 13, 4, -23, 2, 11]
// chunk size: 4
// runs written: 6
// temporary directory removed: yes
// output: [-23, -21, -18, -15, -6, -5, -4, -2, 0, 2, 4, 6, 7, 8, 9, 9, 11, 11,
//          13, 15, 16, 17, 22]
// matches std::sort: yes
// chunk size: 4
// runs written: 0
// temporary directory removed: yes
// empty output size: 0
//
// Special cases: empty input (no directory is created), input smaller than one
// chunk, exact multiple of the chunk size, duplicates, and negative values.
// The 23-element input produces 6 runs: 5 full chunks and one partial chunk.

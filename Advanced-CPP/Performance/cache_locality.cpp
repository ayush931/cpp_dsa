/**
 * Cache locality -> access memory in the order it is laid out
 *
 * A cache line is usually 64 bytes. Loading one byte pulls the whole line.
 * Sequential access uses every byte of each line; strided access wastes it.
 *
 * Row-major vs column-major traversal of a 2D array can differ by 5-10x
 * because of cache misses, even though both are O(n^2).
 *
 * Array of structs (AoS) -> good when all fields of one object are used
 * Struct of arrays (SoA) -> good when one field of many objects is used
 *
 * Data-oriented design: arrange data by how it is accessed, not by how it
 * is conceptualized.
 */

#include <chrono>
#include <iostream>
#include <vector>
using namespace std;
using namespace chrono;

const int SIZE = 2000;

void rowMajor(vector<vector<int>> &matrix, long long &sum) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            sum += matrix[row][col];
        }
    }
};

void columnMajor(vector<vector<int>> &matrix, long long &sum) {
    for (int col = 0; col < SIZE; col++) {
        for (int row = 0; row < SIZE; row++) {
            sum += matrix[row][col];
        }
    }
};

struct ParticleAoS {
    float x;
    float y;
    float z;
    float mass;
};

struct ParticlesSoA {
    vector<float> x;
    vector<float> y;
    vector<float> z;
    vector<float> mass;
};

int main() {
    vector<vector<int>> matrix(SIZE, vector<int>(SIZE, 1));

    long long sum = 0;

    auto start = high_resolution_clock::now();
    rowMajor(matrix, sum);
    auto end = high_resolution_clock::now();

    cout << "row major: " << duration_cast<milliseconds>(end - start).count()
         << " ms" << endl;

    sum = 0;
    start = high_resolution_clock::now();
    columnMajor(matrix, sum);
    end = high_resolution_clock::now();

    cout << "column major: " << duration_cast<milliseconds>(end - start).count()
         << " ms" << endl;

    // SoA: update only x, the cache lines contain no unused fields
    ParticlesSoA soa;
    soa.x.assign(1000000, 0.0f);
    soa.y.assign(1000000, 0.0f);
    soa.z.assign(1000000, 0.0f);
    soa.mass.assign(1000000, 1.0f);

    start = high_resolution_clock::now();

    for (int i = 0; i < soa.x.size(); i++) {
        soa.x[i] += 1.0f;
    }

    end = high_resolution_clock::now();

    cout << "SoA update: " << duration_cast<microseconds>(end - start).count()
         << " us" << endl;

    // AoS: the same update pulls y, z and mass into cache too
    vector<ParticleAoS> aos(1000000, {0, 0, 0, 1});

    start = high_resolution_clock::now();

    for (int i = 0; i < aos.size(); i++) {
        aos[i].x += 1.0f;
    }

    end = high_resolution_clock::now();

    cout << "AoS update: " << duration_cast<microseconds>(end - start).count()
         << " us" << endl;

    return 0;
}

// Special cases: a vector of vectors is not contiguous (each row is a separate
// allocation) -> use one flat vector for the best locality;
// prefetchers handle sequential access well but not random access;
// a linked list has poor locality because every node is elsewhere in memory;
// measure with a profiler, not by guessing; -O2 changes the picture

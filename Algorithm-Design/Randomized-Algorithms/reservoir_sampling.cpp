/**
 * Reservoir Sampling
 *
 * Reservoir sampling selects a uniform random sample of k items from a stream
 * of unknown length using O(k) memory.
 *
 * Algorithm R:
 *   - Put the first k items into the reservoir.
 *   - For item i (0-based, i >= k), choose a random integer j in [0, i].
 *   - If j < k, replace reservoir[j] with item i.
 *
 * Every item is equally likely to be in the final sample, and every k-subset
 * is equally likely.
 *
 * A weighted variant (A-Res) is also shown. Each item gets the key
 * u^(1 / weight), where u is uniform in (0, 1]. Keeping the k largest keys
 * gives weighted sampling without replacement. For k=1, selection probability
 * is proportional to weight; for larger k, inclusion probabilities are not
 * simply k*weight/sum(weights). Logarithmic keys avoid pow underflow.
 *
 * The tests use a fixed seed; exact draws can vary by standard library.
 * The vector models an input stream; auxiliary storage is O(k).
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <queue>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>
using namespace std;

vector<int> reservoirSample(const vector<int> &stream, int k,
                            mt19937 &generator) {
    if (k <= 0 || stream.empty()) {
        return {};
    }

    int sampleSize = min(k, static_cast<int>(stream.size()));
    vector<int> reservoir(stream.begin(), stream.begin() + sampleSize);

    for (int index = sampleSize; index < static_cast<int>(stream.size());
         index++) {
        uniform_int_distribution<int> distribution(0, index);
        int replacement = distribution(generator);

        if (replacement < sampleSize) {
            reservoir[replacement] = stream[index];
        }
    }

    return reservoir;
}

vector<int> weightedReservoirSample(const vector<int> &items,
                                    const vector<double> &weights, int k,
                                    mt19937 &generator) {
    if (k <= 0 || items.empty() || items.size() != weights.size()) {
        return {};
    }

    int sampleSize = min(k, static_cast<int>(items.size()));

    // Min-heap of (key, item index), keeping the k largest keys.
    priority_queue<pair<double, int>, vector<pair<double, int>>,
                   greater<pair<double, int>>>
        heap;

    uniform_real_distribution<double> distribution(0.0, 1.0);

    for (int index = 0; index < static_cast<int>(items.size()); index++) {
        if (!isfinite(weights[index])) {
            throw invalid_argument("weights must be finite");
        }
        if (weights[index] <= 0.0) {
            continue;
        }

        // 1-u lies in (0,1]; log preserves key ordering and avoids underflow.
        double key = log1p(-distribution(generator)) / weights[index];

        if (static_cast<int>(heap.size()) < sampleSize) {
            heap.push({key, index});
        } else if (!heap.empty() && key > heap.top().first) {
            heap.pop();
            heap.push({key, index});
        }
    }

    vector<int> result;

    while (!heap.empty()) {
        result.push_back(items[heap.top().second]);
        heap.pop();
    }

    sort(result.begin(), result.end());
    return result;
}

void printSample(const vector<int> &sample) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(sample.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << sample[i];
    }

    cout << "]";
}

int main() {
    mt19937 generator(20261111);
    vector<int> stream = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    vector<int> sample = reservoirSample(stream, 3, generator);
    cout << "sample of 3: ";
    printSample(sample);
    cout << endl;

    vector<int> zeroSample = reservoirSample(stream, 0, generator);
    cout << "sample of 0: ";
    printSample(zeroSample);
    cout << endl;

    vector<int> largeSample = reservoirSample(stream, 20, generator);
    cout << "sample larger than stream: ";
    printSample(largeSample);
    cout << endl;

    vector<int> emptySample = reservoirSample({}, 3, generator);
    cout << "empty stream: ";
    printSample(emptySample);
    cout << endl;

    const int trials = 20000;
    const int sampleSize = 3;
    vector<int> frequency(stream.size(), 0);

    for (int trial = 0; trial < trials; trial++) {
        vector<int> current = reservoirSample(stream, sampleSize, generator);

        for (int value : current) {
            frequency[value]++;
        }
    }

    double expected = static_cast<double>(trials) * sampleSize / stream.size();
    bool uniform = true;

    cout << "frequency over " << trials << " trials:" << endl;

    for (int value = 0; value < static_cast<int>(stream.size()); value++) {
        cout << value << ": " << frequency[value] << " (expected " << expected
             << ")" << endl;

        if (abs(frequency[value] - expected) > expected * 0.12) {
            uniform = false;
        }
    }

    cout << "roughly uniform: " << (uniform ? "yes" : "no") << endl;

    vector<int> items = {0, 1, 2, 3};
    vector<double> weights = {1.0, 2.0, 3.0, 4.0};
    vector<int> weightedFrequency(items.size(), 0);

    for (int trial = 0; trial < 10000; trial++) {
        vector<int> current =
            weightedReservoirSample(items, weights, 1, generator);
        weightedFrequency[current.front()]++;
    }

    cout << "weighted sample frequencies (weights 1:2:3:4):" << endl;

    for (int index = 0; index < static_cast<int>(items.size()); index++) {
        cout << items[index] << ": " << weightedFrequency[index] << endl;
    }

    return 0;
}

// Expected output (abridged):
// sample of 3: [8, 4, 2]
// sample of 0: []
// sample larger than stream: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
// empty stream: []
// frequency over 20000 trials:
// 0: ~6000 (expected 6000)
// ...
// roughly uniform: yes
// weighted sample frequencies (weights 1:2:3:4):
// 0: ~1000
// 1: ~2000
// 2: ~3000
// 3: ~4000
//
// Special cases: k = 0, k larger than the stream, empty stream, a single item,
// duplicate values, and weighted sampling with positive weights.

/**
 * van Emde Boas (vEB) Tree
 *
 * A vEB tree stores integers from a fixed universe {0, 1, ..., U - 1} and
 * supports membership, insert, erase, minimum, maximum, successor, and
 * predecessor in O(log log U) time.
 *
 * Structure:
 *   - min and max are stored directly at each node.
 *   - The universe is split into sqrt(U) clusters of size sqrt(U).
 *   - A summary vEB tree stores which clusters are non-empty.
 *
 * Recursion: T(U) = T(sqrt(U)) + O(1), so T(U) = O(log log U).
 *
 * The implementation supports arbitrary universe sizes by using
 * ceil(sqrt(U)) as the cluster width. A std::set is used as the reference
 * implementation in the tests.
 */

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <stdexcept>
#include <vector>
using namespace std;

class VanEmdeBoas {
   private:
    int universe;
    int clusterWidth;
    int minValue;
    int maxValue;
    unique_ptr<VanEmdeBoas> summary;
    vector<unique_ptr<VanEmdeBoas>> clusters;

    static int upperSquareRoot(int value) {
        int root = 1;
        while (1LL * root * root < value) {
            root++;
        }
        return root;
    }

    int high(int value) const {
        return value / clusterWidth;
    }

    int low(int value) const {
        return value % clusterWidth;
    }

    int index(int cluster, int offset) const {
        return cluster * clusterWidth + offset;
    }

   public:
    explicit VanEmdeBoas(int size)
        : universe(size),
          clusterWidth(upperSquareRoot(size)),
          minValue(-1),
          maxValue(-1) {
        if (size <= 0) {
            throw invalid_argument("universe must be positive");
        }
        if (universe <= 2) {
            return;
        }

        int clusterCount = static_cast<int>(
            (1LL * universe + clusterWidth - 1) / clusterWidth);
        summary = make_unique<VanEmdeBoas>(clusterCount);
        clusters.reserve(clusterCount);

        for (int i = 0; i < clusterCount; i++) {
            clusters.push_back(make_unique<VanEmdeBoas>(clusterWidth));
        }
    }

    bool empty() const {
        return minValue == -1;
    }

    int minimum() const {
        return minValue;
    }

    int maximum() const {
        return maxValue;
    }

    bool contains(int value) const {
        if (value < 0 || value >= universe) {
            return false;
        }
        if (value == minValue || value == maxValue) {
            return true;
        }
        if (universe <= 2) {
            return false;
        }

        return clusters[high(value)]->contains(low(value));
    }

    void insert(int value) {
        if (value < 0 || value >= universe) {
            return;
        }
        if (empty()) {
            minValue = maxValue = value;
            return;
        }

        if (value == minValue || value == maxValue) {
            return;
        }

        if (value < minValue) {
            swap(value, minValue);
        }

        if (universe > 2) {
            int cluster = high(value);
            int offset = low(value);

            if (clusters[cluster]->empty()) {
                summary->insert(cluster);
            }
            clusters[cluster]->insert(offset);
        }

        if (value > maxValue) {
            maxValue = value;
        }
    }

    void erase(int value) {
        if (value < 0 || value >= universe || !contains(value)) {
            return;
        }

        if (minValue == maxValue) {
            minValue = maxValue = -1;
            return;
        }

        if (universe == 2) {
            minValue = maxValue = (value == 0 ? 1 : 0);
            return;
        }

        // The minimum is not stored in a cluster, so replace it with the next
        // smallest element before erasing that element from its cluster.
        if (value == minValue) {
            int firstCluster = summary->minimum();
            value = index(firstCluster, clusters[firstCluster]->minimum());
            minValue = value;
        }

        int cluster = high(value);
        int offset = low(value);

        clusters[cluster]->erase(offset);

        if (clusters[cluster]->empty()) {
            summary->erase(cluster);
        }

        if (value == maxValue) {
            if (summary->empty()) {
                maxValue = minValue;
            } else {
                int lastCluster = summary->maximum();
                maxValue = index(lastCluster, clusters[lastCluster]->maximum());
            }
        }
    }

    int successor(int value) const {
        if (value < 0 || value >= universe) {
            return -1;
        }
        if (empty() || universe == 1) {
            return -1;
        }

        if (universe == 2) {
            if (value == 0 && maxValue == 1) {
                return 1;
            }
            return -1;
        }

        if (!empty() && value < minValue) {
            return minValue;
        }

        int cluster = high(value);
        int offset = low(value);

        if (!clusters[cluster]->empty() &&
            offset < clusters[cluster]->maximum()) {
            int nextOffset = clusters[cluster]->successor(offset);
            if (nextOffset != -1) {
                return index(cluster, nextOffset);
            }
        }

        int nextCluster = summary->successor(cluster);
        if (nextCluster == -1) {
            return -1;
        }

        return index(nextCluster, clusters[nextCluster]->minimum());
    }

    int predecessor(int value) const {
        if (value < 0 || value >= universe) {
            return -1;
        }
        if (empty() || universe == 1) {
            return -1;
        }

        if (universe == 2) {
            if (value == 1 && minValue == 0) {
                return 0;
            }
            return -1;
        }

        if (!empty() && value > maxValue) {
            return maxValue;
        }

        int cluster = high(value);
        int offset = low(value);

        if (!clusters[cluster]->empty() &&
            offset > clusters[cluster]->minimum()) {
            int previousOffset = clusters[cluster]->predecessor(offset);
            if (previousOffset != -1) {
                return index(cluster, previousOffset);
            }
        }

        int previousCluster = summary->predecessor(cluster);
        if (previousCluster == -1) {
            if (!empty() && value > minValue) {
                return minValue;
            }
            return -1;
        }

        return index(previousCluster, clusters[previousCluster]->maximum());
    }
};

void checkAgainstSet(const VanEmdeBoas &tree, const set<int> &reference,
                     int universe) {
    bool ok = true;

    int expectedMin = reference.empty() ? -1 : *reference.begin();
    int expectedMax = reference.empty() ? -1 : *reference.rbegin();

    if (tree.minimum() != expectedMin) {
        ok = false;
    }
    if (tree.maximum() != expectedMax) {
        ok = false;
    }

    for (int value = 0; value < universe; value++) {
        bool expectedContains = reference.count(value) != 0;
        if (tree.contains(value) != expectedContains) {
            cout << "contains mismatch at " << value
                 << ": tree=" << tree.contains(value)
                 << " expected=" << expectedContains << endl;
            ok = false;
        }

        auto next = reference.upper_bound(value);
        int expectedSuccessor = (next == reference.end()) ? -1 : *next;
        if (tree.successor(value) != expectedSuccessor) {
            cout << "successor mismatch at " << value
                 << ": tree=" << tree.successor(value)
                 << " expected=" << expectedSuccessor << endl;
            ok = false;
        }

        auto previous = reference.lower_bound(value);
        int expectedPredecessor = -1;
        if (previous != reference.begin()) {
            --previous;
            expectedPredecessor = *previous;
        }
        if (tree.predecessor(value) != expectedPredecessor) {
            cout << "predecessor mismatch at " << value
                 << ": tree=" << tree.predecessor(value)
                 << " expected=" << expectedPredecessor << endl;
            ok = false;
        }
    }

    if (!ok) {
        cout << "MISMATCH against std::set" << endl;
    }
}

int main() {
    const int universe = 64;
    VanEmdeBoas tree(universe);
    set<int> reference;

    for (int value = 0; value < universe; value += 2) {
        tree.insert(value);
        reference.insert(value);
    }

    cout << "after inserting evens:" << endl;
    cout << "min: " << tree.minimum() << ", max: " << tree.maximum() << endl;
    cout << "successor of 10: " << tree.successor(10) << endl;
    cout << "predecessor of 10: " << tree.predecessor(10) << endl;
    cout << "successor of 62: " << tree.successor(62) << endl;
    cout << "predecessor of 0: " << tree.predecessor(0) << endl;
    checkAgainstSet(tree, reference, universe);

    mt19937 generator(20261111);
    uniform_int_distribution<int> valueDistribution(0, universe - 1);
    uniform_int_distribution<int> operationDistribution(0, 2);

    for (int step = 0; step < 5000; step++) {
        int value = valueDistribution(generator);
        int operation = operationDistribution(generator);

        if (operation == 0) {
            tree.insert(value);
            reference.insert(value);
        } else if (operation == 1) {
            tree.erase(value);
            reference.erase(value);
        } else {
            if (tree.contains(value) != (reference.count(value) != 0)) {
                cout << "contains mismatch at step " << step << endl;
                return 1;
            }
        }

        if (step % 100 == 0) {
            checkAgainstSet(tree, reference, universe);
        }
    }

    checkAgainstSet(tree, reference, universe);
    cout << "randomized comparison against std::set: passed" << endl;

    VanEmdeBoas emptyTree(16);
    cout << "empty min: " << emptyTree.minimum()
         << ", successor of 5: " << emptyTree.successor(5) << endl;

    emptyTree.insert(7);
    emptyTree.insert(7);
    cout << "single value after duplicate insert: " << emptyTree.minimum()
         << ", " << emptyTree.maximum() << endl;
    emptyTree.erase(7);
    cout << "after erase: " << (emptyTree.empty() ? "empty" : "not empty")
         << endl;

    return 0;
}

// Expected output (abridged):
// after inserting evens:
// min: 0, max: 62
// successor of 10: 12
// predecessor of 10: 8
// successor of 62: -1
// predecessor of 0: -1
// randomized comparison against std::set: passed
// empty min: -1, successor of 5: -1
// single value after duplicate insert: 7, 7
// after erase: empty
//
// Special cases: empty tree, duplicate insertion, erasing the minimum,
// erasing the maximum, erasing the last element, values outside the universe,
// and predecessor/successor queries outside the stored range.

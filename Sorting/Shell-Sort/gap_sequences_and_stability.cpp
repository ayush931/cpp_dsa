/**
 * Shell Sort: Knuth and Ciura Gap Sequences, Stability, Complexity
 *
 * Shell sort is insertion sort applied to interleaved sublists. A gap of h
 * sorts the sublists arr[i], arr[i + h], arr[i + 2h], ... The final gap is
 * always 1, which is ordinary insertion sort on an almost-sorted array.
 *
 * The gap sequence matters:
 *   n/2 halving -> simple, O(n^2) worst case
 *   Knuth 3h+1  -> 1, 4, 13, 40, ... gives O(n^1.5) worst case
 *   Ciura       -> 1, 4, 10, 23, 57, 132, ... very fast in practice
 *
 * Example:
 *   {9, 8, 3, 7, 5, 6, 4, 1, 2, 0}
 *   Knuth gaps for n = 10 -> 4, 1
 *   Ciura gaps for n = 10 -> 4, 1
 *   sorted -> 0 1 2 3 4 5 6 7 8 9
 *
 * Shell sort is in-place and unstable. With gap sequence {2, 1} the input
 * {2a, 2b, 1} becomes {1, 2b, 2a}: the two equal 2s change order.
 *
 * Time complexity  -> depends on the gap sequence (see table in main)
 * Space complexity -> O(1)
 */

#include <iostream>
#include <vector>
using namespace std;

void shellSortWithGaps(vector<int> &arr, const vector<int> &gaps) {
    int n = arr.size();

    for (int gap : gaps) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }

            arr[j] = temp;
        }
    }
}

// Knuth sequence: 1, 4, 13, 40, ... generated backwards from the largest
// value below n.
vector<int> knuthGaps(int n) {
    vector<int> gaps;
    int h = 1;

    while (h < n / 3) {
        h = 3 * h + 1;
    }

    while (h >= 1) {
        gaps.push_back(h);
        h = (h - 1) / 3;
    }

    return gaps;
}

// Ciura's empirically good sequence. The published values are used for
// moderate n; 1 is always included.
vector<int> ciuraGaps(int n) {
    int published[] = {701, 301, 132, 57, 23, 10, 4, 1};
    vector<int> gaps;

    for (int gap : published) {
        if (gap < n) {
            gaps.push_back(gap);
        }
    }

    if (gaps.empty() || gaps.back() != 1) {
        gaps.push_back(1);
    }

    return gaps;
}

void print(vector<int> &arr) {
    for (int value : arr) {
        cout << value << " ";
    }
    cout << endl;
}

struct Item {
    int value;
    int originalIndex;
};

// Insertion comparisons use only the value, so equal values are never
// swapped inside one gap pass. Different gap passes can still reorder them.
void shellSortItems(vector<Item> &items, const vector<int> &gaps) {
    int n = items.size();

    for (int gap : gaps) {
        for (int i = gap; i < n; i++) {
            Item temp = items[i];
            int j = i;

            while (j >= gap && items[j - gap].value > temp.value) {
                items[j] = items[j - gap];
                j -= gap;
            }

            items[j] = temp;
        }
    }
}

int main() {
    vector<int> base = {9, 8, 3, 7, 5, 6, 4, 1, 2, 0};

    vector<int> halving = base;
    vector<int> halvingGaps;
    for (int gap = (int)halving.size() / 2; gap > 0; gap /= 2) {
        halvingGaps.push_back(gap);
    }
    shellSortWithGaps(halving, halvingGaps);
    print(halving);  // 0 1 2 3 4 5 6 7 8 9

    vector<int> knuth = base;
    vector<int> knuthSequence = knuthGaps(knuth.size());
    shellSortWithGaps(knuth, knuthSequence);
    print(knuth);  // 0 1 2 3 4 5 6 7 8 9

    vector<int> ciura = base;
    vector<int> ciuraSequence = ciuraGaps(ciura.size());
    shellSortWithGaps(ciura, ciuraSequence);
    print(ciura);  // 0 1 2 3 4 5 6 7 8 9

    cout << "Knuth gaps: ";
    for (int gap : knuthSequence) {
        cout << gap << " ";
    }
    cout << endl;  // 4 1

    cout << "Ciura gaps: ";
    for (int gap : ciuraSequence) {
        cout << gap << " ";
    }
    cout << endl;  // 4 1

    vector<Item> items = {{2, 0}, {2, 1}, {1, 2}};
    vector<int> unstableGaps = {2, 1};
    shellSortItems(items, unstableGaps);

    for (Item &item : items) {
        cout << item.value << "@" << item.originalIndex << " ";
    }
    cout << endl;  // 1@2 2@1 2@0  (equal 2s reversed -> unstable)

    cout << "Sequence      Worst case     Notes" << endl;
    cout << "n/2 halving   O(n^2)         simple but slow on some inputs"
         << endl;
    cout << "Knuth 3h+1    O(n^1.5)       good general purpose choice" << endl;
    cout << "Ciura         no tight bound  fastest in practice for moderate n"
         << endl;

    return 0;
}

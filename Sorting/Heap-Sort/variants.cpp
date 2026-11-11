/**
 * Heap Sort Variants
 *
 * 1. Build-heap vs repeated insertion
 *    - Floyd build-heap: heapify internal nodes from the bottom -> O(n)
 *    - Repeated insertion: sift each new element up -> O(n log n)
 *    Both then extract the root n times, which costs O(n log n).
 *
 * 2. Descending order
 *    A min-heap places the smallest element at the root, so swapping the root
 *    to the end builds the array in descending order.
 *
 * 3. Custom comparator
 *    The same template works for any ordering, such as absolute value or a
 *    struct sorted by one field and then another.
 *
 * 4. k largest elements
 *    A min-heap of size k keeps the k largest values in O(n log k) time.
 *
 * Example:
 *   {9, 4, 7, 1, 8, 2, 6, 3, 5, 0}
 *   ascending  -> 0 1 2 3 4 5 6 7 8 9
 *   descending -> 9 8 7 6 5 4 3 2 1 0
 *   4 largest  -> 9 8 7 6
 *
 * Time complexity  -> O(n log n) for all heap sort variants
 * Space complexity -> O(1) for the sort, O(k) for the k-largest heap
 */

#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

void siftDown(vector<int> &arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(arr[i], arr[largest]);
        siftDown(arr, n, largest);
    }
}

// Floyd build-heap: O(n) comparisons, then n extractions.
void heapSortBuild(vector<int> &arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        siftDown(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        siftDown(arr, i, 0);
    }
}

void siftUp(vector<int> &arr, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;

        if (arr[parent] >= arr[i]) {
            break;
        }

        swap(arr[parent], arr[i]);
        i = parent;
    }
}

// Repeated insertion: each of the n inserts can climb O(log n) levels.
void heapSortInsert(vector<int> &arr) {
    int n = arr.size();

    for (int i = 1; i < n; i++) {
        siftUp(arr, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        siftDown(arr, i, 0);
    }
}

void siftDownMin(vector<int> &arr, int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] < arr[smallest]) {
        smallest = left;
    }
    if (right < n && arr[right] < arr[smallest]) {
        smallest = right;
    }

    if (smallest != i) {
        swap(arr[i], arr[smallest]);
        siftDownMin(arr, n, smallest);
    }
}

void heapSortDescending(vector<int> &arr) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        siftDownMin(arr, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        siftDownMin(arr, i, 0);
    }
}

template <typename T, typename Compare>
void siftDownWith(vector<T> &arr, int n, int i, Compare comp) {
    int best = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // comp(a, b) is true when a should come before b in the final order.
    if (left < n && comp(arr[best], arr[left])) {
        best = left;
    }
    if (right < n && comp(arr[best], arr[right])) {
        best = right;
    }

    if (best != i) {
        swap(arr[i], arr[best]);
        siftDownWith(arr, n, best, comp);
    }
}

template <typename T, typename Compare>
void heapSortWith(vector<T> &arr, Compare comp) {
    int n = arr.size();

    for (int i = n / 2 - 1; i >= 0; i--) {
        siftDownWith(arr, n, i, comp);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        siftDownWith(arr, i, 0, comp);
    }
}

vector<int> kLargest(vector<int> &arr, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (int value : arr) {
        minHeap.push(value);

        if ((int)minHeap.size() > k) {
            minHeap.pop();
        }
    }

    vector<int> result;
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }

    reverse(result.begin(), result.end());
    return result;
}

struct Person {
    string name;
    int age;
};

void print(vector<int> &arr) {
    for (int value : arr) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    vector<int> base = {9, 4, 7, 1, 8, 2, 6, 3, 5, 0};

    vector<int> build = base;
    heapSortBuild(build);
    print(build);  // 0 1 2 3 4 5 6 7 8 9

    vector<int> insert = base;
    heapSortInsert(insert);
    print(insert);  // 0 1 2 3 4 5 6 7 8 9

    vector<int> descending = base;
    heapSortDescending(descending);
    print(descending);  // 9 8 7 6 5 4 3 2 1 0

    vector<int> values = {-5, 3, -1, 4, -2};
    heapSortWith(values, [](int a, int b) { return abs(a) < abs(b); });
    print(values);  // -1 -2 3 4 -5

    vector<Person> people = {
        {"Aarav", 30}, {"Bina", 25}, {"Chetan", 30}, {"Divya", 25}};

    heapSortWith(people, [](const Person &a, const Person &b) {
        if (a.age != b.age) {
            return a.age > b.age;  // older first
        }
        return a.name < b.name;  // ties by name
    });

    for (Person &person : people) {
        cout << person.name << ":" << person.age << " ";
    }
    cout << endl;  // Aarav:30 Chetan:30 Bina:25 Divya:25

    vector<int> largest = kLargest(base, 4);
    print(largest);  // 9 8 7 6

    return 0;
}

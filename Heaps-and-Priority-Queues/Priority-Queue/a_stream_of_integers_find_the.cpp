/**
 * Given a stream of integers, find the median of the stream.
 */

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class MedianFinder {
    priority_queue<int> maxpq;                             // lower half
    priority_queue<int, vector<int>, greater<int>> minpq;  // upper half

   public:
    MedianFinder() {}

    void addNum(int num) {
        if (maxpq.empty() || num <= maxpq.top()) {
            maxpq.push(num);
        } else {
            minpq.push(num);
        }

        // rebalance: maxpq may have at most one extra element
        if (maxpq.size() > minpq.size() + 1) {
            minpq.push(maxpq.top());
            maxpq.pop();
        } else if (minpq.size() > maxpq.size()) {
            maxpq.push(minpq.top());
            minpq.pop();
        }
    }

    double findMedian() {
        if (maxpq.empty()) {
            return 0.0;
        }

        if (minpq.size() == maxpq.size()) {
            return ((double)minpq.top() + maxpq.top()) / 2.0;
        }

        return maxpq.top();
    }
};

int main() {
    MedianFinder finder;

    finder.addNum(1);
    cout << finder.findMedian() << endl;  // 1

    finder.addNum(2);
    cout << finder.findMedian() << endl;  // 1.5

    finder.addNum(3);
    cout << finder.findMedian() << endl;  // 2

    finder.addNum(4);
    cout << finder.findMedian() << endl;  // 2.5

    finder.addNum(5);
    cout << finder.findMedian() << endl;  // 3

    MedianFinder empty;
    cout << empty.findMedian() << endl;  // 0

    return 0;
}

// Time complexity -> O(log n) per addNum, O(1) per findMedian
// Space complexity -> O(n)
// Special cases: empty stream, one element, duplicate values, negative
// values, very large values (cast to double before adding to avoid overflow),
// an even vs odd number of elements

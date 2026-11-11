/**
 * Closest pair of points
 * Brute force -> check every pair, O(n^2)
 * Divide and conquer -> sort by x, split, compare across the strip, O(n log n)
 */

#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    long long x;
    long long y;
};

long long distanceSquared(Point a, Point b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
};

long long closestPairBrute(vector<Point> &points) {
    long long minimum = LLONG_MAX;

    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            minimum = min(minimum, distanceSquared(points[i], points[j]));
        }
    }

    return minimum;
};

long long closestPair(vector<Point> &points, int left, int right) {
    if (right - left <= 3) {
        long long minimum = LLONG_MAX;

        for (int i = left; i <= right; i++) {
            for (int j = i + 1; j <= right; j++) {
                minimum = min(minimum, distanceSquared(points[i], points[j]));
            }
        }

        return minimum;
    }

    int middle = (left + right) / 2;
    long long middleX = points[middle].x;

    long long leftMinimum = closestPair(points, left, middle);
    long long rightMinimum = closestPair(points, middle + 1, right);
    long long minimum = min(leftMinimum, rightMinimum);

    vector<Point> strip;

    for (int i = left; i <= right; i++) {
        long long dx = points[i].x - middleX;
        if (dx * dx < minimum) {
            strip.push_back(points[i]);
        }
    }

    sort(strip.begin(), strip.end(),
         [](Point a, Point b) { return a.y < b.y; });

    for (int i = 0; i < strip.size(); i++) {
        for (int j = i + 1; j < strip.size(); j++) {
            long long dy = strip[j].y - strip[i].y;
            if (dy * dy >= minimum) {
                break;
            }

            minimum = min(minimum, distanceSquared(strip[i], strip[j]));
        }
    }

    return minimum;
};

int main() {
    vector<Point> points = {{2, 3}, {12, 30}, {40, 50},
                            {5, 1}, {12, 10}, {3, 4}};

    sort(points.begin(), points.end(),
         [](Point a, Point b) { return a.x < b.x; });

    cout << closestPairBrute(points) << endl;  // 2 (distance squared)
    cout << closestPair(points, 0, points.size() - 1) << endl;

    return 0;
}

// Special cases: fewer than 2 points, duplicate points, all points collinear,
// large coordinates (use long long), comparing squared distances avoids
// floating point

/**
 * Divide and Conquer: Fast Exponentiation and Closest Pair of Points
 *
 * Fast exponentiation:
 *   base^exp = (base^(exp/2))^2          when exp is even
 *   base^exp = (base^(exp/2))^2 * base   when exp is odd
 * This removes half of the remaining multiplications at every step.
 *
 * Closest pair:
 *   Sort points by x, split into left and right halves, solve both halves,
 *   then check only the points inside a narrow strip around the split line.
 *   Squared distances are used so the comparison stays in integers.
 *
 * Example:
 *   2^10 = 1024
 *   closest pair among
 *   {(2,3), (12,30), (40,50), (5,1), (12,10), (3,4)}
 *   is (2,3)-(3,4), squared distance 2.
 *
 * Time complexity:
 *   exponentiation -> O(log exp)
 *   closest pair   -> O(n log^2 n) here (the strip is sorted at every level);
 *                     O(n log n) with a merged y-sorted array
 * Space complexity -> O(log exp) recursion, O(n) auxiliary for closest pair
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

long long powerNaive(long long base, long long exponent) {
    long long result = 1;

    for (long long i = 0; i < exponent; i++) {
        result *= base;
    }

    return result;
}

long long powerRecursive(long long base, long long exponent) {
    if (exponent == 0) {
        return 1;
    }

    long long half = powerRecursive(base, exponent / 2);
    long long result = half * half;

    if (exponent % 2 == 1) {
        result *= base;
    }

    return result;
}

long long powerIterative(long long base, long long exponent) {
    long long result = 1;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }

    return result;
}

struct Point {
    long long x;
    long long y;
};

long long distanceSquared(const Point &a, const Point &b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

long long bruteClosest(vector<Point> &points, int left, int right) {
    long long best = LLONG_MAX;

    for (int i = left; i <= right; i++) {
        for (int j = i + 1; j <= right; j++) {
            best = min(best, distanceSquared(points[i], points[j]));
        }
    }

    return best;
}

long long closestRecursive(vector<Point> &points, int left, int right) {
    if (right - left <= 3) {
        return bruteClosest(points, left, right);
    }

    int mid = left + (right - left) / 2;
    long long midX = points[mid].x;

    long long leftBest = closestRecursive(points, left, mid);
    long long rightBest = closestRecursive(points, mid + 1, right);
    long long best = min(leftBest, rightBest);

    vector<Point> strip;

    for (int i = left; i <= right; i++) {
        long long dx = points[i].x - midX;

        if (dx * dx < best) {
            strip.push_back(points[i]);
        }
    }

    sort(strip.begin(), strip.end(),
         [](const Point &a, const Point &b) { return a.y < b.y; });

    for (int i = 0; i < (int)strip.size(); i++) {
        for (int j = i + 1; j < (int)strip.size(); j++) {
            long long dy = strip[j].y - strip[i].y;

            if (dy * dy >= best) {
                break;
            }

            best = min(best, distanceSquared(strip[i], strip[j]));
        }
    }

    return best;
}

long long closestPair(vector<Point> points) {
    if (points.size() < 2) {
        return -1;
    }

    sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
        if (a.x != b.x) {
            return a.x < b.x;
        }
        return a.y < b.y;
    });

    return closestRecursive(points, 0, (int)points.size() - 1);
}

int main() {
    cout << powerNaive(2, 10) << endl;      // 1024
    cout << powerRecursive(2, 10) << endl;  // 1024
    cout << powerIterative(2, 10) << endl;  // 1024

    cout << powerRecursive(3, 5) << endl;  // 243
    cout << powerIterative(5, 0) << endl;  // 1

    vector<Point> points = {{2, 3}, {12, 30}, {40, 50},
                            {5, 1}, {12, 10}, {3, 4}};

    vector<Point> brutePoints = points;
    sort(brutePoints.begin(), brutePoints.end(),
         [](const Point &a, const Point &b) {
             if (a.x != b.x) {
                 return a.x < b.x;
             }
             return a.y < b.y;
         });

    cout << bruteClosest(brutePoints, 0, (int)brutePoints.size() - 1)
         << endl;                         // 2
    cout << closestPair(points) << endl;  // 2

    vector<Point> duplicates = {{0, 0}, {0, 0}, {5, 5}};
    cout << closestPair(duplicates) << endl;  // 0

    vector<Point> single = {{7, 7}};
    cout << closestPair(single) << endl;  // -1

    return 0;
}

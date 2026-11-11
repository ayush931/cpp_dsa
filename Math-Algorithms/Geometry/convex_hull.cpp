/**
 * Convex hull -> smallest convex polygon containing all given points
 * Andrew's monotonic chain -> sort points, build lower and upper hulls
 * Cross product decides whether three points make a left turn
 * Time -> O(n log n)
 *
 * Brute force -> gift wrapping (Jarvis march), O(n * h)
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    long long x;
    long long y;

    bool operator<(const Point &other) const {
        if (x != other.x) {
            return x < other.x;
        }
        return y < other.y;
    }

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
};

long long cross(Point o, Point a, Point b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
};

long long distanceSquared(Point a, Point b) {
    long long dx = a.x - b.x;
    long long dy = a.y - b.y;
    return dx * dx + dy * dy;
};

// Gift wrapping (Jarvis march): from the current hull vertex, pick the next
// vertex that makes every other point lie to the left. Collinear points are
// skipped in favour of the farthest one, so only hull corners remain.
vector<Point> convexHullBrute(vector<Point> points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    int n = points.size();
    if (n < 3) {
        return points;
    }

    int start = 0;  // leftmost, then lowest
    vector<Point> hull;
    int current = start;

    do {
        hull.push_back(points[current]);
        int next = (current + 1) % n;

        for (int i = 0; i < n; i++) {
            long long turn = cross(points[current], points[next], points[i]);

            if (turn < 0 ||
                (turn == 0 &&
                 distanceSquared(points[current], points[i]) >
                     distanceSquared(points[current], points[next]))) {
                next = i;
            }
        }

        current = next;
    } while (current != start);

    return hull;
};

vector<Point> convexHull(vector<Point> points) {
    int n = points.size();
    if (n < 3) {
        return points;
    }

    sort(points.begin(), points.end());

    vector<Point> lower;

    for (Point &p : points) {
        while (lower.size() >= 2 &&
               cross(lower[lower.size() - 2], lower.back(), p) <= 0) {
            lower.pop_back();
        }
        lower.push_back(p);
    }

    vector<Point> upper;

    for (int i = n - 1; i >= 0; i--) {
        Point p = points[i];

        while (upper.size() >= 2 &&
               cross(upper[upper.size() - 2], upper.back(), p) <= 0) {
            upper.pop_back();
        }
        upper.push_back(p);
    }

    lower.pop_back();
    upper.pop_back();

    lower.insert(lower.end(), upper.begin(), upper.end());

    return lower;
};

int main() {
    vector<Point> points = {{0, 0}, {1, 1}, {2, 2}, {3, 0},
                            {0, 3}, {3, 3}, {1, 2}};

    vector<Point> brute = convexHullBrute(points);
    vector<Point> optimized = convexHull(points);

    for (Point &p : brute) {
        cout << "(" << p.x << ", " << p.y << ") ";
    }
    cout << endl;

    for (Point &p : optimized) {
        cout << "(" << p.x << ", " << p.y << ") ";
    }
    cout << endl;

    return 0;
}

// Special cases: duplicate points, collinear points, fewer than 3 points,
// all points collinear, integer overflow in cross product (use long long)

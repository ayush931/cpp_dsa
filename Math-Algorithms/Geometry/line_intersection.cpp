/**
 * Line segment intersection
 * Orientation of three points -> clockwise, counterclockwise or collinear
 * Two segments intersect if:-
 * 1. They straddle each other (different orientations)
 * 2. A point lies on the other segment (collinear case)
 *
 * Time -> O(1) per pair
 */

#include <iostream>
using namespace std;

struct Point {
    long long x;
    long long y;
};

long long cross(Point o, Point a, Point b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
};

int orientation(Point a, Point b, Point c) {
    long long value = cross(a, b, c);

    if (value > 0) {
        return 1;  // counterclockwise
    }
    if (value < 0) {
        return -1;  // clockwise
    }
    return 0;  // collinear
};

bool onSegment(Point a, Point b, Point p) {
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
};

bool doIntersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4) {
        return true;
    }

    if (o1 == 0 && onSegment(p1, q1, p2)) {
        return true;
    }
    if (o2 == 0 && onSegment(p1, q1, q2)) {
        return true;
    }
    if (o3 == 0 && onSegment(p2, q2, p1)) {
        return true;
    }
    if (o4 == 0 && onSegment(p2, q2, q1)) {
        return true;
    }

    return false;
};

int main() {
    Point p1 = {0, 0}, q1 = {10, 10};
    Point p2 = {0, 10}, q2 = {10, 0};

    cout << doIntersect(p1, q1, p2, q2) << endl;  // 1

    Point p3 = {0, 0}, q3 = {5, 5};
    Point p4 = {6, 6}, q4 = {10, 10};

    cout << doIntersect(p3, q3, p4, q4) << endl;  // 0

    return 0;
}

// Special cases: parallel segments, collinear overlapping segments,
// segments touching at an endpoint, zero length segments,
// integer overflow in cross product

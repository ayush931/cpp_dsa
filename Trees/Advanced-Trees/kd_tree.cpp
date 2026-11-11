/**
 * k-d Tree (2-dimensional)
 *
 * A k-d tree recursively partitions points by alternating coordinate axes.
 * Every node stores one point, a splitting axis, and two subtrees. For 2-D
 * data the axes alternate x, y, x, y, ...
 *
 * Operations:
 *   - build:          O(n log n) expected using nth_element
 *   - nearest point:  O(log n) average, O(n) worst case
 *   - rectangle query: O(sqrt(n) + k) for a balanced 2-D tree, O(n) worst case
 *     before sorting the returned k IDs in O(k log k) time
 *
 * Coordinates in this long long squared-distance demo must be within
 * [-10^9, 10^9], including query coordinates, to keep sums of squares in range.
 *
 * The nearest-neighbour search first visits the side of the splitting plane
 * that contains the query. It visits the other side only when the distance to
 * the splitting plane is smaller than the best distance found so far.
 *
 * A brute-force scanner is included so every query can be verified.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;
    int id;
};

struct Node {
    Point point;
    int left;
    int right;
    int axis;
};

long long squaredDistance(const Point &a, const Point &b) {
    long long dx = static_cast<long long>(a.x) - b.x;
    long long dy = static_cast<long long>(a.y) - b.y;
    return dx * dx + dy * dy;
}

class KDTree {
   private:
    vector<Point> points;
    vector<Node> nodes;
    int root;

    int build(int low, int high, int depth) {
        if (low >= high) {
            return -1;
        }

        int axis = depth % 2;
        int middle = low + (high - low) / 2;

        nth_element(points.begin() + low, points.begin() + middle,
                    points.begin() + high,
                    [axis](const Point &a, const Point &b) {
                        if (axis == 0) {
                            if (a.x != b.x) {
                                return a.x < b.x;
                            }
                            if (a.y != b.y) {
                                return a.y < b.y;
                            }
                            return a.id < b.id;
                        }

                        if (a.y != b.y) {
                            return a.y < b.y;
                        }
                        if (a.x != b.x) {
                            return a.x < b.x;
                        }
                        return a.id < b.id;
                    });

        int index = static_cast<int>(nodes.size());
        nodes.push_back(Node{points[middle], -1, -1, axis});

        nodes[index].left = build(low, middle, depth + 1);
        nodes[index].right = build(middle + 1, high, depth + 1);

        return index;
    }

    void nearest(int index, const Point &query, long long &bestDistance,
                 int &bestId) const {
        if (index == -1) {
            return;
        }

        const Node &node = nodes[index];
        long long distance = squaredDistance(node.point, query);

        if (distance < bestDistance ||
            (distance == bestDistance &&
             (bestId == -1 || node.point.id < bestId))) {
            bestDistance = distance;
            bestId = node.point.id;
        }

        int axis = node.axis;
        bool goLeft =
            (axis == 0) ? query.x < node.point.x : query.y < node.point.y;

        int nearChild = goLeft ? node.left : node.right;
        int farChild = goLeft ? node.right : node.left;

        nearest(nearChild, query, bestDistance, bestId);

        long long delta = (axis == 0)
                              ? static_cast<long long>(query.x) - node.point.x
                              : static_cast<long long>(query.y) - node.point.y;

        // Visit the far side when the splitting plane could contain a closer or
        // equally close point (the latter keeps the smallest id on ties).
        if (delta * delta <= bestDistance) {
            nearest(farChild, query, bestDistance, bestId);
        }
    }

    void collectRange(int index, int xLow, int xHigh, int yLow, int yHigh,
                      vector<int> &result) const {
        if (index == -1) {
            return;
        }

        const Node &node = nodes[index];

        if (node.point.x >= xLow && node.point.x <= xHigh &&
            node.point.y >= yLow && node.point.y <= yHigh) {
            result.push_back(node.point.id);
        }

        if (node.axis == 0) {
            if (xLow <= node.point.x) {
                collectRange(node.left, xLow, xHigh, yLow, yHigh, result);
            }
            if (xHigh >= node.point.x) {
                collectRange(node.right, xLow, xHigh, yLow, yHigh, result);
            }
        } else {
            if (yLow <= node.point.y) {
                collectRange(node.left, xLow, xHigh, yLow, yHigh, result);
            }
            if (yHigh >= node.point.y) {
                collectRange(node.right, xLow, xHigh, yLow, yHigh, result);
            }
        }
    }

   public:
    KDTree() : root(-1) {}

    void build(const vector<Point> &input) {
        points = input;
        nodes.clear();
        root = build(0, static_cast<int>(points.size()), 0);
    }

    int nearestId(const Point &query) const {
        long long bestDistance = LLONG_MAX;
        int bestId = -1;
        nearest(root, query, bestDistance, bestId);
        return bestId;
    }

    vector<int> rangeQuery(int xLow, int xHigh, int yLow, int yHigh) const {
        vector<int> result;
        collectRange(root, xLow, xHigh, yLow, yHigh, result);
        sort(result.begin(), result.end());
        return result;
    }
};

int bruteForceNearest(const vector<Point> &points, const Point &query) {
    long long bestDistance = LLONG_MAX;
    int bestId = -1;

    for (const Point &point : points) {
        long long distance = squaredDistance(point, query);
        if (distance < bestDistance ||
            (distance == bestDistance && (bestId == -1 || point.id < bestId))) {
            bestDistance = distance;
            bestId = point.id;
        }
    }

    return bestId;
}

vector<int> bruteForceRange(const vector<Point> &points, int xLow, int xHigh,
                            int yLow, int yHigh) {
    vector<int> result;

    for (const Point &point : points) {
        if (point.x >= xLow && point.x <= xHigh && point.y >= yLow &&
            point.y <= yHigh) {
            result.push_back(point.id);
        }
    }

    sort(result.begin(), result.end());
    return result;
}

void printIds(const vector<int> &ids) {
    cout << "[";

    for (int i = 0; i < static_cast<int>(ids.size()); i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << ids[i];
    }

    cout << "]";
}

int main() {
    vector<Point> points = {{2, 3, 0}, {5, 4, 1}, {9, 6, 2}, {4, 7, 3},
                            {8, 1, 4}, {7, 2, 5}, {1, 8, 6}, {3, 5, 7},
                            {6, 9, 8}, {0, 0, 9}};

    KDTree tree;
    tree.build(points);

    vector<Point> queries = {
        {0, 0, -1}, {9, 9, -1}, {4, 5, -1}, {7, 3, -1}, {-5, 12, -1}};

    for (const Point &query : queries) {
        int expected = bruteForceNearest(points, query);
        int actual = tree.nearestId(query);
        cout << "nearest to (" << query.x << ", " << query.y << "): " << actual
             << " (brute force: " << expected << ")"
             << "  matches: " << (actual == expected ? "yes" : "no") << endl;
    }

    vector<vector<int>> rectangles = {
        {2, 6, 2, 7}, {0, 9, 0, 9}, {20, 30, 20, 30}, {3, 3, 5, 5}};

    for (const vector<int> &rectangle : rectangles) {
        vector<int> actual = tree.rangeQuery(rectangle[0], rectangle[1],
                                             rectangle[2], rectangle[3]);
        vector<int> expected = bruteForceRange(
            points, rectangle[0], rectangle[1], rectangle[2], rectangle[3]);

        cout << "range [" << rectangle[0] << ", " << rectangle[1] << "] x ["
             << rectangle[2] << ", " << rectangle[3] << "]: ";
        printIds(actual);
        cout << "  matches: " << (actual == expected ? "yes" : "no") << endl;
    }

    KDTree emptyTree;
    emptyTree.build({});
    cout << "empty tree nearest: " << emptyTree.nearestId({0, 0, -1}) << endl;

    KDTree singleTree;
    singleTree.build({{4, 4, 42}});
    cout << "single tree nearest: " << singleTree.nearestId({100, 100, -1})
         << endl;

    return 0;
}

// Expected output (abridged):
// nearest to (0, 0): 9 (brute force: 9)  matches: yes
// nearest to (9, 9): 2 (brute force: 2)  matches: yes
// nearest to (4, 5): 7 (brute force: 7)  matches: yes
// range [2, 6] x [2, 7]: [0, 1, 3, 7]  matches: yes
// empty tree nearest: -1
// single tree nearest: 42
//
// Special cases: empty tree, single point, duplicate coordinates (ties are
// broken by the smallest id), queries outside every rectangle, and queries
// exactly on a splitting plane.

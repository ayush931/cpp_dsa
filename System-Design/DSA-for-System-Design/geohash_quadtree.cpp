/**
 * Geohash / Quadtree for Nearby Search
 *
 * Location-based services need to answer "which points are inside this
 * rectangle?" or "what is near me?" quickly.
 *
 * Brute force -> scan every point -> O(n) per query
 * Quadtree -> recursively split the plane into four quadrants, then visit only
 * the quadrants that intersect the query -> O(log n + k) on average
 *
 * A geohash is the string-based alternative: interleave latitude and longitude
 * bits, then encode them in base32. Nearby points share a prefix, so a range
 * query becomes a prefix scan. A quadtree stores the same idea as a tree.
 */

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;

struct Point {
    int x;
    int y;
};

class QuadTree {
    struct Node {
        int x;
        int y;
        int width;
        int height;
        vector<Point> points;
        Node *children[4];

        Node(int startX, int startY, int w, int h) {
            x = startX;
            y = startY;
            width = w;
            height = h;

            for (int i = 0; i < 4; i++) {
                children[i] = NULL;
            }
        }
    };

    static const int BUCKET_SIZE = 4;
    Node *root;

    void destroy(Node *node) {
        if (node == nullptr) {
            return;
        }
        for (Node *child : node->children) {
            destroy(child);
        }
        delete node;
    }

    bool contains(Node *node, Point point) {
        return point.x >= node->x && point.x < node->x + node->width &&
               point.y >= node->y && point.y < node->y + node->height;
    }

    bool intersects(Node *node, int left, int bottom, int right, int top) {
        return !(right < node->x || left >= node->x + node->width ||
                 top < node->y || bottom >= node->y + node->height);
    }

    void subdivide(Node *node) {
        if (node->children[0] != NULL) {
            return;
        }

        int halfWidth = node->width / 2;
        int halfHeight = node->height / 2;

        if (halfWidth == 0 || halfHeight == 0) {
            return;  // cannot split further
        }

        node->children[0] = new Node(node->x, node->y, halfWidth, halfHeight);
        node->children[1] = new Node(node->x + halfWidth, node->y,
                                     node->width - halfWidth, halfHeight);
        node->children[2] = new Node(node->x, node->y + halfHeight, halfWidth,
                                     node->height - halfHeight);
        node->children[3] =
            new Node(node->x + halfWidth, node->y + halfHeight,
                     node->width - halfWidth, node->height - halfHeight);

        vector<Point> oldPoints = node->points;
        node->points.clear();

        for (Point point : oldPoints) {
            insertInto(node, point);
        }
    }

    void insertInto(Node *node, Point point) {
        if (!contains(node, point)) {
            return;
        }

        if (node->children[0] == NULL && node->points.size() < BUCKET_SIZE) {
            node->points.push_back(point);
            return;
        }

        if (node->children[0] == NULL) {
            subdivide(node);
        }

        if (node->children[0] == NULL) {
            node->points.push_back(point);  // leaf is too small to split
            return;
        }

        for (int i = 0; i < 4; i++) {
            if (contains(node->children[i], point)) {
                insertInto(node->children[i], point);
                return;
            }
        }
    }

    void queryNode(Node *node, int left, int bottom, int right, int top,
                   vector<Point> &result) {
        if (node == NULL || !intersects(node, left, bottom, right, top)) {
            return;
        }

        for (Point point : node->points) {
            if (point.x >= left && point.x <= right && point.y >= bottom &&
                point.y <= top) {
                result.push_back(point);
            }
        }

        for (int i = 0; i < 4; i++) {
            queryNode(node->children[i], left, bottom, right, top, result);
        }
    }

   public:
    QuadTree(int width, int height) {
        if (width <= 0 || height <= 0) {
            throw invalid_argument("positive quadtree dimensions required");
        }
        root = new Node(0, 0, width, height);
    }

    ~QuadTree() {
        destroy(root);
    }

    QuadTree(const QuadTree &) = delete;
    QuadTree &operator=(const QuadTree &) = delete;

    void insert(Point point) {
        insertInto(root, point);
    }

    vector<Point> query(int left, int bottom, int right, int top) {
        vector<Point> result;
        queryNode(root, left, bottom, right, top, result);
        return result;
    }
};

bool comparePoints(Point a, Point b) {
    if (a.x != b.x) {
        return a.x < b.x;
    }
    return a.y < b.y;
}

int main() {
    QuadTree tree(100, 100);
    vector<Point> points;

    for (int i = 0; i < 30; i++) {
        Point point;
        point.x = (i * 37) % 100;
        point.y = (i * 53) % 100;

        points.push_back(point);
        tree.insert(point);
    }

    vector<Point> brute;

    for (Point point : points) {
        if (point.x >= 20 && point.x <= 60 && point.y >= 20 && point.y <= 60) {
            brute.push_back(point);
        }
    }

    vector<Point> treeResult = tree.query(20, 20, 60, 60);

    sort(brute.begin(), brute.end(), comparePoints);
    sort(treeResult.begin(), treeResult.end(), comparePoints);

    cout << "brute: " << brute.size() << endl;
    cout << "quadtree: " << treeResult.size() << endl;

    for (Point point : treeResult) {
        cout << "(" << point.x << "," << point.y << ") ";
    }
    cout << endl;

    cout << (brute.size() == treeResult.size()) << endl;  // 1

    return 0;
}

/**
 * Time complexity:
 *   build -> O(n log n) on average
 *   query -> O(log n + k) on average, O(n) in the worst case
 *   brute query -> O(n)
 * Space complexity -> O(n)
 *
 * Special cases: empty tree, all points in one quadrant, duplicate points,
 * points on a quadrant boundary, a query rectangle that covers everything,
 * a query that matches nothing, coordinates outside the root bounds, very
 * deep trees (use a maximum depth).
 */

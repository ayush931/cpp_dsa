/**
 * Asteroid collision
 *
 * Positive values move right and negative values move left. When two
 * asteroids meet, the smaller one explodes; equal sizes destroy both. Two
 * asteroids moving in the same direction, or away from each other, never
 * collide.
 *
 * Example:
 * Input:  [5, 10, -5]
 * Output: [5, 10]
 *
 * Input:  [8, -8]
 * Output: []
 *
 * Input:  [10, 2, -5]
 * Output: [10]
 *
 * Brute force: repeatedly resolve adjacent colliding pairs -> O(n^2)
 * Optimized: stack of surviving asteroids -> O(n) time, O(n) space
 */

#include <iostream>
#include <vector>
using namespace std;

vector<int> asteroidCollisionBrute(vector<int> asteroids) {
    bool changed = true;

    while (changed) {
        changed = false;

        for (int i = 0; i + 1 < (int)asteroids.size(); i++) {
            if (asteroids[i] > 0 && asteroids[i + 1] < 0) {
                if (asteroids[i] < -asteroids[i + 1]) {
                    asteroids.erase(asteroids.begin() + i);
                } else if (asteroids[i] > -asteroids[i + 1]) {
                    asteroids.erase(asteroids.begin() + i + 1);
                } else {
                    asteroids.erase(asteroids.begin() + i,
                                    asteroids.begin() + i + 2);
                }

                changed = true;
                break;  // restart after changing the array
            }
        }
    }

    return asteroids;
}

vector<int> asteroidCollision(const vector<int> &asteroids) {
    vector<int> survivors;

    for (int asteroid : asteroids) {
        bool alive = true;

        // Only a right-moving top and a left-moving newcomer can collide.
        while (alive && !survivors.empty() && survivors.back() > 0 &&
               asteroid < 0) {
            if (survivors.back() < -asteroid) {
                survivors.pop_back();  // top explodes, newcomer continues
            } else if (survivors.back() == -asteroid) {
                survivors.pop_back();  // both explode
                alive = false;
            } else {
                alive = false;  // newcomer explodes
            }
        }

        if (alive) {
            survivors.push_back(asteroid);
        }
    }

    return survivors;
}

void print(const vector<int> &values) {
    cout << "[";
    for (int i = 0; i < (int)values.size(); i++) {
        cout << values[i];
        if (i + 1 < (int)values.size()) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
}

int main() {
    vector<vector<int>> tests = {{5, 10, -5},    {8, -8},     {10, 2, -5},
                                 {-2, -1, 1, 2}, {1, -2, -3}, {-2, 2, -1, -2}};

    print(asteroidCollisionBrute(tests[0]));  // [5, 10]
    print(asteroidCollision(tests[0]));       // [5, 10]

    print(asteroidCollision(tests[1]));  // []
    print(asteroidCollision(tests[2]));  // [10]
    print(asteroidCollision(tests[3]));  // [-2, -1, 1, 2]
    print(asteroidCollision(tests[4]));  // [-2, -3]
    print(asteroidCollision(tests[5]));  // [-2]

    return 0;
}

/**
 * Each asteroid is pushed at most once and popped at most once, so the stack
 * loop is linear even though it contains a nested while loop.
 *
 * Special cases: all moving right, all moving left, equal sizes, a newcomer
 * that destroys several survivors, and a newcomer that is destroyed by the
 * first survivor it meets.
 */

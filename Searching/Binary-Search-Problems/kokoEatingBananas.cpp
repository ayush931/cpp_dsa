// Koko Eating Bananas
//
// There are n piles of bananas. Koko chooses an integer eating speed k
// (bananas per hour). Each hour she picks one pile and eats up to k bananas
// from it; if the pile has fewer than k bananas left, she finishes it and does
// not start another pile that hour. Return the smallest k that lets her finish
// all piles within h hours.
//
// Example:
//   piles = [3, 6, 7, 11], h = 8 -> 4
//   piles = [30, 11, 23, 4, 20], h = 5 -> 30
//   piles = [30, 11, 23, 4, 20], h = 6 -> 23
//
// A pile of p bananas takes ceil(p / k) hours. The total is monotone in k, so
// binary search the answer.
//
// Brute force: try every speed from 1 to max(piles) -> O(n * maxPile).
// Optimized: binary search the speed -> O(n * log maxPile).

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

long long hoursNeeded(const vector<int> &piles, int speed) {
    long long hours = 0;

    for (int pile : piles) {
        hours += (pile + (long long)speed - 1) / speed;  // ceil(pile / speed)
    }

    return hours;
}

int minEatingSpeedBrute(const vector<int> &piles, int h) {
    if (piles.empty()) {
        return 0;
    }

    int maxPile = *max_element(piles.begin(), piles.end());

    for (int speed = 1; speed <= maxPile; speed++) {
        if (hoursNeeded(piles, speed) <= h) {
            return speed;
        }
    }

    return -1;
}

int minEatingSpeed(const vector<int> &piles, int h) {
    if (piles.empty()) {
        return 0;
    }
    if (h < (int)piles.size()) {
        return -1;  // one pile needs at least one hour
    }

    int lo = 1;
    int hi = *max_element(piles.begin(), piles.end());

    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;

        if (hoursNeeded(piles, mid) <= h) {
            hi = mid;  // mid works, try a smaller speed
        } else {
            lo = mid + 1;  // mid is too slow
        }
    }

    return lo;
}

int main() {
    vector<int> first = {3, 6, 7, 11};
    vector<int> second = {30, 11, 23, 4, 20};

    cout << minEatingSpeed(first, 8) << endl;       // 4
    cout << minEatingSpeedBrute(first, 8) << endl;  // 4

    cout << minEatingSpeed(second, 5) << endl;  // 30
    cout << minEatingSpeed(second, 6) << endl;  // 23

    cout << minEatingSpeed(second, 4) << endl;  // -1 (fewer hours than piles)

    vector<int> one = {1};
    cout << minEatingSpeed(one, 1) << endl;  // 1

    vector<int> empty;
    cout << minEatingSpeed(empty, 5) << endl;  // 0

    return 0;
}

// Time complexity -> O(n log maxPile) time, O(1) extra space
// Brute force -> O(n * maxPile) time, O(1) extra space

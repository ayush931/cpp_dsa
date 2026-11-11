/**
 * Sort Colors (Dutch National Flag)
 *
 * Given an array nums with n objects colored red, white, or blue, sort them
 * in-place so that objects of the same color are adjacent, with the colors in
 * the order red (0), white (1), and blue (2).
 *
 * Example:
 * Input:  [2, 0, 2, 1, 1, 0]
 * Output: [0, 0, 1, 1, 2, 2]
 *
 * Constraints: 1 <= n <= 300, nums[i] is 0, 1, or 2.
 */

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: use the library sort -> O(n log n)
void sortColorsBrute(vector<int> &nums) {
    sort(nums.begin(), nums.end());
}

// Counting sort: count each color, then overwrite -> O(n), two passes
void sortColorsCounting(vector<int> &nums) {
    int count[3] = {0, 0, 0};

    for (int value : nums) {
        count[value]++;
    }

    int index = 0;

    for (int color = 0; color < 3; color++) {
        while (count[color] > 0) {
            nums[index] = color;
            index++;
            count[color]--;
        }
    }
}

// Dutch national flag: three pointers, one pass -> O(n) time, O(1) space
void sortColors(vector<int> &nums) {
    int low = 0;                 // next position for 0
    int mid = 0;                 // current element
    int high = nums.size() - 1;  // next position for 2

    while (mid <= high) {
        if (nums[mid] == 0) {
            swap(nums[mid], nums[low]);
            low++;
            mid++;
        } else if (nums[mid] == 1) {
            mid++;
        } else {
            swap(nums[mid], nums[high]);
            high--;
            // do not advance mid: the swapped-in value is not examined yet
        }
    }
}

void print(vector<int> &nums) {
    for (int value : nums) {
        cout << value << " ";
    }
    cout << endl;
}

int main() {
    vector<int> first = {2, 0, 2, 1, 1, 0};
    vector<int> second = {2, 0, 1};
    vector<int> third = {0};
    vector<int> fourth = {2, 2, 1, 1, 0, 0};

    sortColorsBrute(first);
    print(first);  // 0 0 1 1 2 2

    sortColorsCounting(second);
    print(second);  // 0 1 2

    sortColors(third);
    print(third);  // 0

    sortColors(fourth);
    print(fourth);  // 0 0 1 1 2 2

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n log n)
 *   counting -> O(n) time, O(1) space
 *   Dutch flag -> O(n) time, O(1) space
 *
 * Special cases: empty array, one element, all elements the same color,
 * already sorted input, reverse sorted input, many duplicates.
 */

/**
 * 3Sum
 * Given an integer array nums, return all unique triplets that sum to zero.
 *
 * Input: nums = [-1, 0, 1, 2, -1, -4]
 * Output: [-1, -1, 2], [-1, 0, 1]
 *
 * Brute force -> three nested loops, O(n^3)
 * Optimized -> sort, fix one element and use two pointers, O(n^2)
 */

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

vector<vector<int>> threeSumBrute(vector<int> &nums) {
    int n = nums.size();
    set<vector<int>> uniqueTriplets;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                if (nums[i] + nums[j] + nums[k] == 0) {
                    vector<int> triplet = {nums[i], nums[j], nums[k]};
                    sort(triplet.begin(), triplet.end());
                    uniqueTriplets.insert(triplet);
                }
            }
        }
    }

    return vector<vector<int>>(uniqueTriplets.begin(), uniqueTriplets.end());
};

vector<vector<int>> threeSumOptimized(vector<int> &nums) {
    vector<vector<int>> result;
    int n = nums.size();

    sort(nums.begin(), nums.end());

    for (int i = 0; i < n - 2; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) {
            continue;  // skip duplicate first element
        }

        int left = i + 1;
        int right = n - 1;

        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];

            if (sum == 0) {
                result.push_back({nums[i], nums[left], nums[right]});

                while (left < right && nums[left] == nums[left + 1]) {
                    left++;
                }
                while (left < right && nums[right] == nums[right - 1]) {
                    right--;
                }

                left++;
                right--;
            } else if (sum < 0) {
                left++;
            } else {
                right--;
            }
        }
    }

    return result;
};

void printTriplets(vector<vector<int>> &triplets) {
    for (auto &triplet : triplets) {
        for (int value : triplet) {
            cout << value << " ";
        }
        cout << endl;
    }
};

int main() {
    vector<int> nums = {-1, 0, 1, 2, -1, -4};

    vector<vector<int>> brute = threeSumBrute(nums);
    vector<vector<int>> optimized = threeSumOptimized(nums);

    printTriplets(brute);
    cout << "---" << endl;
    printTriplets(optimized);

    return 0;
}

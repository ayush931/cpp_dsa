/**
 * Product of Array Except Self
 * Given an array nums, return an array where answer[i] is the product
 * of all elements except nums[i]. Do not use division.
 *
 * Input: nums = [1, 2, 3, 4]
 * Output: [24, 12, 8, 6]
 *
 * Brute force -> multiply all other elements for every index, O(n^2)
 * Optimized -> prefix and suffix products, O(n) time and O(1) extra space
 */

#include <iostream>
#include <vector>
using namespace std;

vector<int> productExceptSelfBrute(vector<int> &nums) {
    int n = nums.size();
    vector<int> result(n);

    for (int i = 0; i < n; i++) {
        long long product = 1;

        for (int j = 0; j < n; j++) {
            if (i != j) {
                product *= nums[j];
            }
        }

        result[i] = product;
    }

    return result;
};

vector<int> productExceptSelf(vector<int> &nums) {
    int n = nums.size();
    vector<int> result(n, 1);

    int prefix = 1;
    for (int i = 0; i < n; i++) {
        result[i] = prefix;
        prefix *= nums[i];
    }

    int suffix = 1;
    for (int i = n - 1; i >= 0; i--) {
        result[i] *= suffix;
        suffix *= nums[i];
    }

    return result;
};

int main() {
    vector<int> nums = {1, 2, 3, 4};

    vector<int> brute = productExceptSelfBrute(nums);
    vector<int> optimized = productExceptSelf(nums);

    for (int value : brute) {
        cout << value << " ";
    }
    cout << endl;

    for (int value : optimized) {
        cout << value << " ";
    }
    cout << endl;

    return 0;
}

// Special cases: zeros in the array, negative numbers, single element

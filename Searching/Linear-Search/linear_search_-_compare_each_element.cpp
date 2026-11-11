/**
 * Linear search -> compare each element with the target one by one
 * Works on both sorted and unsorted arrays
 * Time complexity -> O(n)
 * Space complexity -> O(1)
 * Best case -> O(1), when the target is the first element
 */

#include <iostream>
#include <vector>
using namespace std;

int linearSearch(vector<int> &arr, int target) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return -1;
};

int main() {
    vector<int> arr = {5, 3, 8, 1, 9};

    cout << linearSearch(arr, 8) << endl;  // 2
    cout << linearSearch(arr, 7) << endl;  // -1

    return 0;
}

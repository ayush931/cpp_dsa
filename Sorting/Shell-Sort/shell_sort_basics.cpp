/**
 * Shell sort -> generalization of insertion sort
 * It sorts elements that are far apart, then gradually reduces the gap
 * The last pass uses gap 1, which is ordinary insertion sort
 * Time complexity -> depends on the gap sequence; the n/2 halving sequence
 *   used below is O(n^2) worst case, and faster sequences such as Knuth or
 *   Ciura improve the average case
 * Space complexity -> O(1)
 * In-place, unstable
 */

#include <iostream>
#include <vector>
using namespace std;

void shellSort(vector<int> &arr) {
    int n = arr.size();

    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j = i;

            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }

            arr[j] = temp;
        }
    }
};

int main() {
    vector<int> arr = {9, 8, 3, 7, 5, 6, 4, 1};

    shellSort(arr);

    for (int el : arr) {
        cout << el << " ";
    }
    cout << endl;

    return 0;
}

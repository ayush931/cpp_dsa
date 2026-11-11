/**
 * Best Time to Buy and Sell Stock
 *
 * You are given an array prices where prices[i] is the price of a stock on
 * day i. You want to maximize profit by choosing one day to buy and a later
 * day to sell. Return the maximum profit. If no profit is possible, return 0.
 *
 * Example:
 * Input:  [7, 1, 5, 3, 6, 4]
 * Output: 5  (buy at 1, sell at 6)
 *
 * Constraints: 1 <= prices.length <= 10^5, 0 <= prices[i] <= 10^4.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Brute force: try every buy/sell pair -> O(n^2) time, O(1) space
int maxProfitBrute(vector<int> &prices) {
    int best = 0;

    for (int buy = 0; buy < prices.size(); buy++) {
        for (int sell = buy + 1; sell < prices.size(); sell++) {
            best = max(best, prices[sell] - prices[buy]);
        }
    }

    return best;
}

// One pass: track the minimum price seen so far -> O(n) time, O(1) space
int maxProfit(vector<int> &prices) {
    int minimumPrice = INT_MAX;
    int best = 0;

    for (int price : prices) {
        minimumPrice = min(minimumPrice, price);
        best = max(best, price - minimumPrice);
    }

    return best;
}

int main() {
    vector<int> first = {7, 1, 5, 3, 6, 4};
    vector<int> second = {7, 6, 4, 3, 1};
    vector<int> third = {2, 4, 1};
    vector<int> single = {5};

    cout << maxProfitBrute(first) << endl;  // 5
    cout << maxProfit(first) << endl;       // 5

    cout << maxProfit(second) << endl;  // 0 (prices only fall)
    cout << maxProfit(third) << endl;   // 2
    cout << maxProfit(single) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n^2)
 *   one pass -> O(n) time, O(1) space
 *
 * Special cases: strictly decreasing prices (profit 0), one day, equal prices,
 * the best buy is the last day, the best sell is the first day, zeros.
 */

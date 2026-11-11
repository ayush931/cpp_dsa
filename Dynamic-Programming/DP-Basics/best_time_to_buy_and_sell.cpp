/**
 * Best Time to Buy and Sell Stock - All Variants
 *
 * 1. At most one transaction:
 *      prices = [7,1,5,3,6,4] -> 5   (buy at 1, sell at 6)
 *
 * 2. Unlimited transactions:
 *      prices = [7,1,5,3,6,4] -> 7   (1->5 and 3->6)
 *
 * 3. At most k transactions:
 *      prices = [3,2,6,5,0,3], k = 2 -> 7   (2->6 and 0->3)
 *
 * 4. Cooldown of one day after selling:
 *      prices = [1,2,3,0,2] -> 3   (1->2, cooldown, 0->2)
 *
 * 5. Transaction fee:
 *      prices = [1,3,2,8,4,9], fee = 2 -> 8   (1->8 and 4->9)
 *
 * All solutions are O(n) or O(n * k) time. The one-transaction version also
 * shows an O(n^2) brute force for comparison.
 */

#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
using namespace std;

// Variant 1, brute force: try every buy day and every later sell day.
// O(n^2) time, O(1) space.
long long maxProfitOneBrute(const vector<int> &prices) {
    long long best = 0;

    for (int buy = 0; buy < (int)prices.size(); buy++) {
        for (int sell = buy + 1; sell < (int)prices.size(); sell++) {
            best = max(best, (long long)prices[sell] - prices[buy]);
        }
    }

    return best;
}

// Variant 1, optimized: remember the cheapest price seen so far.
// O(n) time, O(1) space.
long long maxProfitOne(const vector<int> &prices) {
    long long best = 0;
    long long minimumPrice = LLONG_MAX;

    for (int price : prices) {
        minimumPrice = min(minimumPrice, (long long)price);
        best = max(best, price - minimumPrice);
    }

    return best;
}

// Variant 2: every upward step can be captured. Buy just before the rise and
// sell just after it, so the answer is the sum of all positive differences.
// O(n) time, O(1) space.
long long maxProfitUnlimited(const vector<int> &prices) {
    long long profit = 0;

    for (int i = 1; i < (int)prices.size(); i++) {
        if (prices[i] > prices[i - 1]) {
            profit += prices[i] - prices[i - 1];
        }
    }

    return profit;
}

// Variant 3: at most k transactions. buy[t] is the best balance after buying
// with t transactions started, sell[t] is the best profit after selling with
// t completed transactions.
// O(n * k) time, O(k) space.
long long maxProfitK(const vector<int> &prices, int k) {
    int n = prices.size();
    if (n < 2 || k <= 0) {
        return 0;
    }

    // More than n / 2 transactions cannot improve on the unlimited answer.
    k = min(k, n / 2);

    const long long NEGATIVE_INFINITY = LLONG_MIN / 4;
    vector<long long> buy(k + 1, NEGATIVE_INFINITY);
    vector<long long> sell(k + 1, 0);

    for (int price : prices) {
        // Descending order keeps sell[t - 1] from the previous day, so a share
        // cannot be bought and sold multiple times on the same day.
        for (int t = k; t >= 1; t--) {
            buy[t] = max(buy[t], sell[t - 1] - price);
            sell[t] = max(sell[t], buy[t] + price);
        }
    }

    return sell[k];
}

// Variant 4: one-day cooldown after a sale. hold, sold, and rest are the best
// balances for the three possible states at the end of a day.
// O(n) time, O(1) space.
long long maxProfitCooldown(const vector<int> &prices) {
    if (prices.empty()) {
        return 0;
    }

    long long hold = -prices[0];  // holding a share
    long long sold = 0;           // sold today
    long long rest = 0;           // not holding, free to buy

    for (int i = 1; i < (int)prices.size(); i++) {
        long long previousSold = sold;

        sold = hold + prices[i];
        hold = max(hold, rest - prices[i]);
        rest = max(rest, previousSold);
    }

    return max(sold, rest);
}

// Variant 5: a fee is paid on every sale. cash is the best balance with no
// share, hold is the best balance while holding one.
// O(n) time, O(1) space.
long long maxProfitFee(const vector<int> &prices, int fee) {
    if (prices.empty()) {
        return 0;
    }

    long long hold = -prices[0];
    long long cash = 0;

    for (int i = 1; i < (int)prices.size(); i++) {
        long long previousCash = cash;

        cash = max(cash, hold + prices[i] - fee);
        hold = max(hold, previousCash - prices[i]);
    }

    return cash;
}

int main() {
    vector<int> first = {7, 1, 5, 3, 6, 4};

    cout << maxProfitOneBrute(first) << " " << maxProfitOne(first) << endl;
    // 5 5

    cout << maxProfitUnlimited(first) << endl;  // 7

    vector<int> kPrices = {3, 2, 6, 5, 0, 3};
    cout << maxProfitK(kPrices, 1) << " " << maxProfitK(kPrices, 2) << endl;
    // 4 7

    vector<int> cooldown = {1, 2, 3, 0, 2};
    cout << maxProfitCooldown(cooldown) << endl;  // 3

    vector<int> feePrices = {1, 3, 2, 8, 4, 9};
    cout << maxProfitFee(feePrices, 2) << endl;  // 8

    vector<int> decreasing = {9, 7, 4, 1};
    cout << maxProfitOne(decreasing) << " " << maxProfitUnlimited(decreasing)
         << endl;
    // 0 0

    vector<int> single = {5};
    cout << maxProfitOne(single) << " " << maxProfitK(single, 2) << endl;
    // 0 0

    vector<int> empty;
    cout << maxProfitCooldown(empty) << " " << maxProfitFee(empty, 3) << endl;
    // 0 0

    return 0;
}

/**
 * Complexity summary:
 *   one transaction, brute      O(n^2) time, O(1) space
 *   one transaction, optimized  O(n)   time, O(1) space
 *   unlimited                   O(n)   time, O(1) space
 *   at most k transactions      O(n*k) time, O(k) space
 *   cooldown                    O(n)   time, O(1) space
 *   fee                         O(n)   time, O(1) space
 *
 * Edge cases: empty input, one day, strictly decreasing prices (0), equal
 * prices (0), k = 0, and k larger than n / 2.
 */

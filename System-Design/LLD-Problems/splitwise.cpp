/**
 * Splitwise (Expense Sharing)
 *
 * A group tracks who paid for what and who should share the cost. Instead of
 * storing a payment for every pair, it keeps one directed balance:
 *
 *   owes[a][b] = how much a owes b
 *
 * When payer P covers amount A for participants with shares s[i] (sum A):
 *   for every participant i != P:
 *     owes[i][P] += s[i]
 *
 * The payer's own share cancels against the payment, so it creates no debt.
 * If the payer is not a participant, every share becomes a debt to the payer.
 *
 * Split types:
 *   Equal   -> amount / n each, with the remainder spread one unit at a time
 *   Exact   -> the caller supplies integer shares that must sum to the amount
 *   Percent -> integer percentages that must sum to 100; the leftover units go
 *              to the largest fractional remainders
 *
 * Settlement:
 *   net[u] = total owed to u - total u owes
 *   positive -> u should receive, negative -> u should pay
 *   repeatedly match the largest debtor with the largest creditor
 *
 * The greedy settlement is not guaranteed to use the absolute minimum number
 * of transfers (that problem is NP-hard in general), but it is simple, always
 * terminates, and produces at most n - 1 transfers. For small groups it is
 * useful, but even small groups can have a better settlement than this greedy
 * result. Exact minimization requires a separate search.
 *
 * All amounts are integers. Using doubles for money invites rounding bugs; if
 * fractional currency is needed, store cents or the smallest unit.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

enum class SplitType { Equal, Exact, Percent };

struct User {
    int id;
    string name;
};

struct Expense {
    int id;
    string description;
    int payer;
    long long amount;
    vector<int> participants;
    vector<long long> shares;
    SplitType type;
};

struct Transfer {
    int from;
    int to;
    long long amount;
};

class Group {
    map<int, User> users;
    vector<Expense> expenses;
    map<int, map<int, long long>> owes;  // owes[a][b] = a owes b
    int nextExpense = 1;

    bool validParticipants(const vector<int> &participants) const {
        if (participants.empty()) {
            return false;
        }

        set<int> unique;

        for (int participant : participants) {
            if (!users.count(participant)) {
                return false;
            }
            if (!unique.insert(participant).second) {
                return false;  // duplicate
            }
        }

        return true;
    }

    bool record(int payer, long long amount, const vector<int> &participants,
                const vector<long long> &shares, SplitType type,
                const string &description) {
        if (!users.count(payer)) {
            return false;
        }
        if (amount <= 0) {
            return false;
        }
        if (participants.size() != shares.size()) {
            return false;
        }
        if (!validParticipants(participants)) {
            return false;
        }

        long long total = 0;

        for (long long share : shares) {
            if (share < 0) {
                return false;
            }
            total += share;
        }

        if (total != amount) {
            return false;
        }

        for (size_t i = 0; i < participants.size(); i++) {
            int participant = participants[i];

            if (participant == payer) {
                continue;  // payer's own share cancels
            }

            owes[participant][payer] += shares[i];
        }

        expenses.push_back({nextExpense++, description, payer, amount,
                            participants, shares, type});

        return true;
    }

   public:
    bool addUser(int id, const string &name) {
        if (users.count(id)) {
            return false;
        }

        users[id] = {id, name};
        return true;
    }

    bool addEqualExpense(const string &description, int payer, long long amount,
                         const vector<int> &participants) {
        if (participants.empty()) {
            return false;
        }

        int count = participants.size();
        vector<long long> shares(count, amount / count);

        for (int i = 0; i < amount % count; i++) {
            shares[i]++;
        }

        return record(payer, amount, participants, shares, SplitType::Equal,
                      description);
    }

    bool addExactExpense(const string &description, int payer, long long amount,
                         const vector<int> &participants,
                         const vector<long long> &shares) {
        return record(payer, amount, participants, shares, SplitType::Exact,
                      description);
    }

    bool addPercentExpense(const string &description, int payer,
                           long long amount, const vector<int> &participants,
                           const vector<int> &percentages) {
        if (participants.empty()) {
            return false;
        }
        if (participants.size() != percentages.size()) {
            return false;
        }

        long long totalPercent = 0;

        for (int percentage : percentages) {
            if (percentage < 0) {
                return false;
            }
            totalPercent += percentage;
        }

        if (totalPercent != 100) {
            return false;
        }

        int count = participants.size();
        vector<long long> shares(count, 0);
        vector<pair<int, int>> remainders;  // {remainder, index}
        long long assigned = 0;

        for (int i = 0; i < count; i++) {
            shares[i] = amount * percentages[i] / 100;
            assigned += shares[i];
            remainders.push_back({amount * percentages[i] % 100, i});
        }

        // The truncation leaves a few units; give them to the largest
        // remainders.
        long long leftover = amount - assigned;

        sort(remainders.begin(), remainders.end(),
             [](const pair<int, int> &a, const pair<int, int> &b) {
                 if (a.first != b.first) {
                     return a.first > b.first;
                 }
                 return a.second < b.second;
             });

        for (int i = 0; i < leftover; i++) {
            shares[remainders[i % count].second]++;
        }

        return record(payer, amount, participants, shares, SplitType::Percent,
                      description);
    }

    // Positive means `a` owes `b`; negative means `b` owes `a`.
    long long balanceBetween(int a, int b) const {
        long long forward = 0;
        long long backward = 0;

        auto row = owes.find(a);
        if (row != owes.end()) {
            auto entry = row->second.find(b);
            if (entry != row->second.end()) {
                forward = entry->second;
            }
        }

        row = owes.find(b);
        if (row != owes.end()) {
            auto entry = row->second.find(a);
            if (entry != row->second.end()) {
                backward = entry->second;
            }
        }

        return forward - backward;
    }

    // Positive -> receives money, negative -> pays money.
    map<int, long long> netBalances() const {
        map<int, long long> net;

        for (const auto &[id, user] : users) {
            net[id] = 0;
        }

        for (const auto &[from, row] : owes) {
            for (const auto &[to, amount] : row) {
                net[from] -= amount;
                net[to] += amount;
            }
        }

        return net;
    }

    vector<Transfer> settle() const {
        vector<pair<int, long long>> creditors;
        vector<pair<int, long long>> debtors;

        for (const auto &[id, amount] : netBalances()) {
            if (amount > 0) {
                creditors.push_back({id, amount});
            } else if (amount < 0) {
                debtors.push_back({id, -amount});
            }
        }

        // Largest amounts first: fewer transfers in practice.
        auto byAmount = [](const pair<int, long long> &a,
                           const pair<int, long long> &b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }
            return a.first < b.first;
        };

        sort(creditors.begin(), creditors.end(), byAmount);
        sort(debtors.begin(), debtors.end(), byAmount);

        vector<Transfer> transfers;
        size_t i = 0;
        size_t j = 0;

        while (i < debtors.size() && j < creditors.size()) {
            long long amount = min(debtors[i].second, creditors[j].second);

            transfers.push_back({debtors[i].first, creditors[j].first, amount});

            debtors[i].second -= amount;
            creditors[j].second -= amount;

            if (debtors[i].second == 0) {
                i++;
            }
            if (creditors[j].second == 0) {
                j++;
            }
        }

        return transfers;
    }

    // Settle only between two users.
    long long settlePair(int a, int b) {
        long long net = balanceBetween(a, b);

        if (net > 0) {
            owes[a][b] -= net;
            if (owes[a][b] == 0) {
                owes[a].erase(b);
            }
            return net;
        }

        if (net < 0) {
            long long amount = -net;
            owes[b][a] -= amount;
            if (owes[b][a] == 0) {
                owes[b].erase(a);
            }
            return amount;
        }

        return 0;
    }

    string name(int id) const {
        auto entry = users.find(id);
        return entry == users.end() ? "unknown" : entry->second.name;
    }

    void printNet() const {
        for (const auto &[id, amount] : netBalances()) {
            cout << name(id) << " " << amount << endl;
        }
    }

    void printTransfers(const vector<Transfer> &transfers) const {
        if (transfers.empty()) {
            cout << "all settled" << endl;
            return;
        }

        for (const Transfer &transfer : transfers) {
            cout << name(transfer.from) << " pays " << name(transfer.to) << " "
                 << transfer.amount << endl;
        }
    }

    size_t expenseCount() const {
        return expenses.size();
    }
};

int main() {
    Group group;

    group.addUser(1, "Asha");
    group.addUser(2, "Ben");
    group.addUser(3, "Chen");
    group.addUser(4, "Dev");

    cout << group.addUser(1, "Duplicate") << endl;  // 0

    // Dinner: Asha pays 1000 for all four, equal split -> 250 each.
    cout << group.addEqualExpense("Dinner", 1, 1000, {1, 2, 3, 4}) << endl;
    // 1

    // Cab: Ben pays 600 for three people -> 200 each.
    cout << group.addEqualExpense("Cab", 2, 600, {1, 2, 3}) << endl;
    // 1

    // Hotel: Chen pays 900, split 50/30/20 among Asha, Ben, Chen.
    cout << group.addPercentExpense("Hotel", 3, 900, {1, 2, 3}, {50, 30, 20})
         << endl;
    // 1

    // Snacks: Dev pays 400, exact shares of 100 each.
    cout << group.addExactExpense("Snacks", 4, 400, {1, 2, 3, 4},
                                  {100, 100, 100, 100})
         << endl;
    // 1

    cout << group.expenseCount() << endl;  // 4

    // Direct balances: Ben owes Asha 250 from dinner, but Asha owes Ben 200
    // from the cab, so the net is 50.
    cout << group.balanceBetween(2, 1) << endl;  // 50
    cout << group.balanceBetween(1, 2) << endl;  // -50

    // Net positions: Asha is even, Ben pays, Chen and Dev receive.
    group.printNet();
    // Asha 0
    // Ben -220
    // Chen 170
    // Dev 50

    // Settlement uses at most three transfers.
    vector<Transfer> transfers = group.settle();
    group.printTransfers(transfers);
    // Ben pays Chen 170
    // Ben pays Dev 50

    // The settlement is a view; the underlying debts are unchanged until
    // settlePair is called. Ben owes Chen 270 from the hotel, but Chen owes Ben
    // 200 from the cab, so the net is 70.
    cout << group.balanceBetween(2, 3) << endl;  // 70
    cout << group.settlePair(2, 3) << endl;      // 70
    cout << group.balanceBetween(2, 3) << endl;  // 0

    // Invalid splits are rejected.
    Group validation;

    validation.addUser(1, "A");
    validation.addUser(2, "B");

    cout << validation.addEqualExpense("Bad", 1, 0, {1, 2}) << endl;    // 0
    cout << validation.addEqualExpense("Bad", 1, 100, {}) << endl;      // 0
    cout << validation.addEqualExpense("Bad", 1, 100, {1, 1}) << endl;  // 0
    cout << validation.addEqualExpense("Bad", 1, 100, {1, 9}) << endl;  // 0
    cout << validation.addExactExpense("Bad", 1, 100, {1, 2}, {40, 50})
         << endl;  // 0 (sum is 90)
    cout << validation.addExactExpense("Bad", 1, 100, {1, 2}, {40, -10})
         << endl;  // 0 (negative)
    cout << validation.addPercentExpense("Bad", 1, 100, {1, 2}, {50, 49})
         << endl;  // 0 (sum is 99)
    cout << validation.addExactExpense("Good", 1, 100, {1, 2}, {40, 60})
         << endl;  // 1

    // Payer not in the participant list: the whole amount is owed to the payer.
    Group payerOnly;
    payerOnly.addUser(1, "Payer");
    payerOnly.addUser(2, "X");
    payerOnly.addUser(3, "Y");
    payerOnly.addExactExpense("Gift", 1, 100, {2, 3}, {50, 50});

    payerOnly.printNet();
    // Payer 100
    // X -50
    // Y -50

    payerOnly.printTransfers(payerOnly.settle());
    // X pays Payer 50
    // Y pays Payer 50

    // Equal split with a remainder: 100 among 3 -> 34, 33, 33.
    Group remainder;
    remainder.addUser(1, "A");
    remainder.addUser(2, "B");
    remainder.addUser(3, "C");
    remainder.addEqualExpense("Remainder", 1, 100, {1, 2, 3});
    remainder.printNet();
    // A 66
    // B -33
    // C -33
    // A paid 100 and owes 34, so A receives 66. B and C each owe 33.

    remainder.printTransfers(remainder.settle());
    // B pays A 33
    // C pays A 33

    // An already balanced group has no transfers. Two loans cancel out.
    Group balanced;
    balanced.addUser(1, "A");
    balanced.addUser(2, "B");
    balanced.addExactExpense("A covers B", 1, 50, {2}, {50});
    balanced.addExactExpense("B covers A", 2, 50, {1}, {50});
    balanced.printNet();
    // A 0
    // B 0
    balanced.printTransfers(balanced.settle());  // all settled

    return 0;
}

/**
 * Time complexity:
 *   addExpense -> O(participants log participants) for validation and, for
 *   percent splits, O(n log n) to sort remainders
 *   balanceBetween -> O(log users)
 *   netBalances -> O(number of debt edges)
 *   settle -> O(edges + users log users) to collect and sort the net positions
 * Space complexity -> O(users + debt edges + expenses)
 *
 * Special cases:
 *   - amount <= 0 -> rejected
 *   - empty participant list -> rejected
 *   - duplicate participant -> rejected
 *   - unknown payer or participant -> rejected
 *   - exact shares that do not sum to the amount -> rejected
 *   - negative shares -> rejected
 *   - percentages that do not sum to 100 -> rejected
 *   - equal split with a remainder -> the first participants get one extra unit
 *   - percent split with a remainder -> the largest fractional remainders get
 *     the extra units
 *   - payer is a participant -> the payer's own share cancels
 *   - payer is not a participant -> every share is owed to the payer
 *   - a group that is already balanced -> settle returns no transfers
 *   - settlePair with no debt -> returns 0
 *   - floating point is never used, so totals stay exact
 *
 * Why store directed balances instead of every payment: an expense creates
 * n - 1 debts, and a naive history grows without bound. Netting pairwise
 * balances keeps the state small, and netBalances reduces the settlement to a
 * one-dimensional matching problem.
 *
 * The greedy settlement is not the minimum-transfer solution in every case.
 * For example, a cycle of debts can sometimes be collapsed into fewer
 * transfers than the greedy order finds. Minimum transfer settlement is a
 * variant of the NP-hard partition problem, so production systems use the
 * greedy method and accept a few extra transfers.
 *
 * Extensions:
 *   - multiple currencies with exchange rates
 *   - recurring expenses and monthly budgets
 *   - receipts and item-level splits
 *   - approvals and audit history
 *   - partial settlement and payment records
 */

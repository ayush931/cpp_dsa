/**
 * ATM
 *
 * An ATM ties together a card reader, a PIN check, an account ledger, and a
 * cash dispenser. The state machine is small but strict:
 *
 *   Idle -> insertCard -> CardInserted -> enterPin(correct) -> Authenticated
 *   CardInserted -> three wrong PINs -> card retained, back to Idle
 *   Authenticated -> withdraw / deposit stay authenticated; eject -> Idle
 *
 * Cash dispenser:
 *   - notes are held per denomination (2000, 500, 200, 100)
 *   - an amount must be a positive multiple of the smallest note
 *   - the dispenser must have an exact combination of notes, not just enough
 *     total value
 *
 * Exact change is not a greedy problem in general. With notes {500, 200, 100},
 * if the machine has one 500, three 200s, and no 100s, an amount of 600 is
 * payable as 200 + 200 + 200. Greedy would take 500 and then fail to make the
 * remaining 100. This implementation uses a small memoized search over the four
 * denominations, so it always finds an exact combination when one exists.
 *
 * Invariants:
 *   - a withdrawal debits the account only after the dispenser succeeds
 *   - a failed withdrawal changes neither the balance nor the note counts
 *   - the card is always ejected or retained at the end of a session
 *
 * A real ATM adds a card reader with a physical retain bin, a journal for crash
 * recovery, a limit per transaction and per day, network authorization, and a
 * reconciliation process between the ledger and the cash cassettes.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

const vector<int> DENOMINATIONS = {2000, 500, 200, 100};

struct Account {
    string number;
    int pin;
    long long balance;
};

struct Card {
    string number;
    string accountNumber;
};

class CashDispenser {
    map<int, int> notes;

    bool search(int index, int remaining, map<int, int> &chosen,
                set<pair<int, int>> &failed) const {
        if (remaining == 0) {
            return true;
        }
        if (index == static_cast<int>(DENOMINATIONS.size())) {
            return false;
        }

        pair<int, int> state = {index, remaining};
        if (failed.count(state)) {
            return false;
        }

        int denomination = DENOMINATIONS[index];
        auto available = notes.find(denomination);
        int limit = available == notes.end() ? 0 : available->second;
        int maximum = min(limit, remaining / denomination);

        // Try larger counts first so the payout uses fewer notes.
        for (int count = maximum; count >= 0; count--) {
            if (search(index + 1, remaining - count * denomination, chosen,
                       failed)) {
                chosen[denomination] = count;
                return true;
            }
        }

        failed.insert(state);
        return false;
    }

   public:
    CashDispenser(map<int, int> initialNotes)
        : notes(std::move(initialNotes)) {}

    // Returns the note breakdown, or an empty map when the amount cannot be
    // paid.
    map<int, int> dispense(int amount) {
        if (amount <= 0 || amount % DENOMINATIONS.back() != 0) {
            return {};
        }

        map<int, int> chosen;
        set<pair<int, int>> failed;

        if (!search(0, amount, chosen, failed)) {
            return {};
        }

        // Commit only after a full solution exists.
        for (const auto &[denomination, count] : chosen) {
            if (count == 0) {
                continue;
            }
            notes[denomination] -= count;
        }

        map<int, int> result;

        for (const auto &[denomination, count] : chosen) {
            if (count > 0) {
                result[denomination] = count;
            }
        }

        return result;
    }

    long long totalCash() const {
        long long total = 0;

        for (const auto &[denomination, count] : notes) {
            total += static_cast<long long>(denomination) * count;
        }

        return total;
    }

    int count(int denomination) const {
        auto entry = notes.find(denomination);
        return entry == notes.end() ? 0 : entry->second;
    }
};

enum class AtmState { Idle, CardInserted, Authenticated };

string stateName(AtmState state) {
    switch (state) {
        case AtmState::Idle:
            return "idle";
        case AtmState::CardInserted:
            return "card-inserted";
        case AtmState::Authenticated:
            return "authenticated";
    }

    return "unknown";
}

class ATM {
    map<string, Account> accounts;
    CashDispenser dispenser;
    AtmState state = AtmState::Idle;
    string activeCard;
    string activeAccount;
    int failedPins = 0;

    void endSession() {
        state = AtmState::Idle;
        activeCard.clear();
        activeAccount.clear();
        failedPins = 0;
    }

   public:
    ATM(map<string, Account> accountTable, map<int, int> initialNotes)
        : accounts(std::move(accountTable)),
          dispenser(std::move(initialNotes)) {}

    bool insertCard(const Card &card) {
        if (state != AtmState::Idle) {
            return false;
        }
        if (!accounts.count(card.accountNumber)) {
            return false;
        }

        activeCard = card.number;
        activeAccount = card.accountNumber;
        state = AtmState::CardInserted;
        failedPins = 0;

        return true;
    }

    // Returns 1 for success, 0 for a wrong PIN, and -1 when the card is
    // retained after the third wrong attempt.
    int enterPin(int pin) {
        if (state != AtmState::CardInserted) {
            return 0;
        }

        Account &account = accounts.at(activeAccount);

        if (pin == account.pin) {
            state = AtmState::Authenticated;
            failedPins = 0;
            return 1;
        }

        failedPins++;

        if (failedPins >= 3) {
            endSession();  // card retained by the machine
            return -1;
        }

        return 0;
    }

    long long balance() const {
        if (state != AtmState::Authenticated) {
            return -1;
        }

        return accounts.at(activeAccount).balance;
    }

    // Returns the note breakdown, or an empty map on failure.
    map<int, int> withdraw(int amount) {
        if (state != AtmState::Authenticated) {
            return {};
        }
        if (amount <= 0) {
            return {};
        }

        Account &account = accounts.at(activeAccount);

        if (amount > account.balance) {
            return {};
        }

        map<int, int> notes = dispenser.dispense(amount);
        if (notes.empty()) {
            return {};  // dispenser cannot make exact change
        }

        account.balance -= amount;
        return notes;
    }

    bool deposit(int amount) {
        if (state != AtmState::Authenticated) {
            return false;
        }
        if (amount <= 0) {
            return false;
        }

        accounts.at(activeAccount).balance += amount;
        return true;
    }

    bool ejectCard() {
        if (state == AtmState::Idle) {
            return false;
        }

        endSession();
        return true;
    }

    AtmState getState() const {
        return state;
    }

    long long cashAvailable() const {
        return dispenser.totalCash();
    }

    int noteCount(int denomination) const {
        return dispenser.count(denomination);
    }

    void printNotes(const map<int, int> &notes) const {
        if (notes.empty()) {
            cout << "no cash" << endl;
            return;
        }

        bool first = true;

        for (auto it = notes.rbegin(); it != notes.rend(); ++it) {
            if (!first) {
                cout << ", ";
            }
            cout << it->first << "x" << it->second;
            first = false;
        }

        cout << endl;
    }
};

int main() {
    map<string, Account> accounts = {{"ACC-1", {"ACC-1", 1234, 10000}},
                                     {"ACC-2", {"ACC-2", 9999, 500}}};

    map<int, int> initialNotes = {{2000, 2}, {500, 4}, {200, 5}, {100, 10}};

    ATM atm(accounts, initialNotes);

    cout << stateName(atm.getState()) << " cash=" << atm.cashAvailable()
         << endl;
    // idle cash=8000

    Card card{"CARD-1", "ACC-1"};

    cout << atm.withdraw(100).size() << endl;   // 0 (no card)
    cout << atm.insertCard(card) << endl;       // 1
    cout << atm.insertCard(card) << endl;       // 0 (a card is already inside)
    cout << stateName(atm.getState()) << endl;  // card-inserted

    // Two wrong PINs do not retain the card.
    cout << atm.enterPin(1111) << " " << atm.enterPin(2222) << endl;  // 0 0
    cout << atm.enterPin(1234) << endl;                               // 1
    cout << stateName(atm.getState()) << endl;  // authenticated

    cout << atm.balance() << endl;  // 10000

    // 2800 = 2000 + 500 + 200 + 100.
    map<int, int> first = atm.withdraw(2800);
    atm.printNotes(first);
    cout << atm.balance() << " " << atm.cashAvailable() << endl;
    // 7200 5200

    // More than the account balance.
    cout << atm.withdraw(8000).size() << endl;  // 0
    cout << atm.balance() << endl;              // 7200

    // Not a multiple of 100.
    cout << atm.withdraw(250).size() << endl;  // 0

    // More than the dispenser holds, even though the balance is enough.
    cout << atm.withdraw(5500).size() << endl;  // 0
    cout << atm.cashAvailable() << endl;        // 5200 (unchanged)

    // A deposit is immediate.
    cout << atm.deposit(1000) << endl;  // 1
    cout << atm.balance() << endl;      // 8200

    // Eject ends the session.
    cout << atm.ejectCard() << endl;            // 1
    cout << stateName(atm.getState()) << endl;  // idle
    cout << atm.balance() << endl;              // -1

    // Three wrong PINs retain the card.
    atm.insertCard(card);
    cout << atm.enterPin(1) << " " << atm.enterPin(2) << " " << atm.enterPin(3)
         << endl;                               // 0 0 -1
    cout << stateName(atm.getState()) << endl;  // idle

    // The card can be inserted again after retention in this simulation.
    atm.insertCard(card);
    cout << atm.enterPin(1234) << endl;  // 1

    // Withdraw the remaining cash in one transaction: 5200.
    map<int, int> rest = atm.withdraw(5200);
    atm.printNotes(rest);
    cout << atm.cashAvailable() << " " << atm.balance() << endl;
    // 0 3000

    // The dispenser is empty now.
    cout << atm.withdraw(100).size() << endl;  // 0
    atm.ejectCard();

    // A second account with a small balance.
    Card second{"CARD-2", "ACC-2"};
    atm.insertCard(second);
    atm.enterPin(9999);
    cout << atm.withdraw(600).size() << endl;  // 0 (balance is 500)
    cout << atm.withdraw(500).size() << endl;  // 0 (no cash left)
    cout << atm.balance() << endl;             // 500
    atm.ejectCard();

    // Unknown account: the card is rejected.
    cout << atm.insertCard({"CARD-3", "ACC-9"}) << endl;  // 0

    return 0;
}

/**
 * Time complexity:
 *   insertCard / enterPin / balance / deposit -> O(log accounts)
 *   withdraw -> O(amount^2) worst case: O(4 * amount) memo states, each trying
 *   up to amount / note
 * Space complexity -> O(accounts + notes) for the ledger and cassettes, plus
 * O(4 * amount) for the failed-state memo during one payout
 *
 * Special cases:
 *   - withdraw before inserting a card or entering a PIN -> empty result
 *   - insert a second card during a session -> rejected
 *   - wrong PIN -> no state change until the third failure, then the card is
 *     retained and the session ends
 *   - correct PIN after one or two failures -> authenticated and the counter
 *     resets
 *   - amount zero or negative -> rejected
 *   - amount not a multiple of the smallest note -> rejected
 *   - amount above the account balance -> rejected
 *   - amount the dispenser cannot make exactly -> rejected, balance unchanged
 *   - account balance and cassette counts are updated together only after a
 *     successful payout
 *   - eject with no card -> false
 *   - unknown account -> the card is rejected at insertion
 *   - a deposit does not change the cassette counts; real ATMs reconcile
 *     deposited notes separately
 *
 * Why exact change needs a search: the coin-change greedy algorithm is optimal
 * only for certain denomination systems. With {500, 200, 100}, greedy fails on
 * 600 when the machine has one 500, three 200s, and no 100s: it takes 500 and
 * cannot make the last 100, while 200 + 200 + 200 succeeds. The memoized search
 * explores the note counts and caches failed (denomination index, remaining
 * amount) states, so it stays fast for realistic amounts.
 *
 * A real ATM also enforces a per-transaction limit, a daily limit, and a
 * maximum note count per withdrawal. Those checks belong before the dispenser
 * call so a rejected request never touches the cassettes.
 */

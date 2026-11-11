/**
 * Library Management
 *
 * A library owns titles (books) and physical copies. Members borrow copies,
 * return them, and may reserve a title when every copy is out.
 *
 * Objects:
 *   Book       -> isbn, title, author (metadata only)
 *   Copy       -> copy id, isbn, availability, borrower, borrow day
 *   Member     -> id, name, active loans, unpaid fines
 *   Library    -> catalog, copies, members, and reservation queues
 *
 * Rules used here:
 *   - a member may hold at most 3 copies at once
 *   - a loan lasts 14 days
 *   - a late return costs 2 per day past the due date
 *   - a reservation queue is first-come-first-served per title
 *   - when a copy is returned, the first reserver gets it immediately
 *   - only the member at the front of the reservation queue may borrow that
 *     title while the queue is non-empty
 *
 * Reservation handoff:
 *   return copy -> mark it available -> look at the queue -> if the first
 *   member is under the loan limit, lend the copy immediately and pop the
 *   queue. The new loan starts on the return day.
 *
 * This model is single-threaded. A real library checks out copies inside a
 * transaction so two desks cannot lend the same copy, and it keeps an audit
 * log for every loan, return, fine payment, and reservation.
 */

#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

const int LOAN_DAYS = 14;
const int MAX_LOANS = 3;
const int FINE_PER_DAY = 2;

struct Book {
    string isbn;
    string title;
    string author;
};

struct Copy {
    int id;
    string isbn;
    bool available = true;
    int borrower = -1;
    int borrowedOn = 0;
};

struct Member {
    int id;
    string name;
    vector<int> loans;  // copy ids
    int fines = 0;
};

class Library {
    map<string, Book> books;
    map<int, Copy> copies;
    map<int, Member> members;
    map<string, deque<int>> reservations;  // isbn -> member ids
    int nextCopyId = 1;

    Copy *findAvailableCopy(const string &isbn) {
        for (auto &[id, copy] : copies) {
            if (copy.isbn == isbn && copy.available) {
                return &copy;
            }
        }

        return nullptr;
    }

    bool lend(int memberId, Copy &copy, int day) {
        Member &member = members.at(memberId);

        if (member.loans.size() >= MAX_LOANS) {
            return false;
        }

        copy.available = false;
        copy.borrower = memberId;
        copy.borrowedOn = day;
        member.loans.push_back(copy.id);

        return true;
    }

    void eraseLoan(Member &member, int copyId) {
        member.loans.erase(
            remove(member.loans.begin(), member.loans.end(), copyId),
            member.loans.end());
    }

   public:
    bool addBook(const string &isbn, const string &title,
                 const string &author) {
        if (books.count(isbn)) {
            return false;
        }

        books[isbn] = {isbn, title, author};
        return true;
    }

    // Returns the new copy id, or -1 for an unknown isbn.
    int addCopy(const string &isbn) {
        if (!books.count(isbn)) {
            return -1;
        }

        int id = nextCopyId++;
        copies[id] = {id, isbn, true, -1, 0};

        return id;
    }

    bool addMember(int id, const string &name) {
        if (members.count(id)) {
            return false;
        }

        members[id] = {id, name, {}, 0};
        return true;
    }

    bool reserve(int memberId, const string &isbn) {
        if (!members.count(memberId) || !books.count(isbn)) {
            return false;
        }

        deque<int> &queue = reservations[isbn];

        if (find(queue.begin(), queue.end(), memberId) != queue.end()) {
            return false;
        }

        queue.push_back(memberId);
        return true;
    }

    // Returns true when the copy was lent. Fails when the member is unknown,
    // the title is unknown, every copy is out, the member is at the limit, or
    // another member is first in the reservation queue.
    bool borrow(int memberId, const string &isbn, int day) {
        if (!members.count(memberId) || !books.count(isbn)) {
            return false;
        }

        Member &member = members.at(memberId);

        if (member.loans.size() >= MAX_LOANS) {
            return false;
        }

        deque<int> &queue = reservations[isbn];

        if (!queue.empty() && queue.front() != memberId) {
            return false;
        }

        Copy *copy = findAvailableCopy(isbn);
        if (copy == nullptr) {
            return false;
        }

        if (!lend(memberId, *copy, day)) {
            return false;
        }

        if (!queue.empty() && queue.front() == memberId) {
            queue.pop_front();
        }

        return true;
    }

    // Returns the fine charged for this return, or -1 on failure.
    int returnCopy(int memberId, int copyId, int day) {
        if (!members.count(memberId) || !copies.count(copyId)) {
            return -1;
        }

        Copy &copy = copies.at(copyId);
        Member &member = members.at(memberId);

        if (copy.available || copy.borrower != memberId) {
            return -1;
        }

        int dueDay = copy.borrowedOn + LOAN_DAYS;
        int lateDays = max(0, day - dueDay);
        int fine = lateDays * FINE_PER_DAY;

        member.fines += fine;
        eraseLoan(member, copyId);

        copy.available = true;
        copy.borrower = -1;
        copy.borrowedOn = 0;

        // Hand the copy to the first reserver when possible.
        deque<int> &queue = reservations[copy.isbn];

        while (!queue.empty()) {
            int nextMember = queue.front();

            if (!members.count(nextMember)) {
                queue.pop_front();
                continue;
            }

            if (lend(nextMember, copy, day)) {
                queue.pop_front();
                break;
            }

            // The reserver is at the loan limit; leave the copy available and
            // keep the reservation so it can be served after a return.
            break;
        }

        return fine;
    }

    bool payFine(int memberId, int amount) {
        if (!members.count(memberId) || amount <= 0) {
            return false;
        }

        Member &member = members.at(memberId);

        if (member.fines < amount) {
            return false;
        }

        member.fines -= amount;
        return true;
    }

    void printTitle(const string &isbn) const {
        auto book = books.find(isbn);

        if (book == books.end()) {
            cout << isbn << " unknown" << endl;
            return;
        }

        int total = 0;
        int available = 0;

        for (const auto &[id, copy] : copies) {
            if (copy.isbn != isbn) {
                continue;
            }

            total++;
            if (copy.available) {
                available++;
            }
        }

        auto queue = reservations.find(isbn);
        int waiting = queue == reservations.end() ? 0 : queue->second.size();

        cout << isbn << " \"" << book->second.title << "\" copies=" << total
             << " available=" << available << " waiting=" << waiting << endl;
    }

    void printMember(int memberId) const {
        auto entry = members.find(memberId);

        if (entry == members.end()) {
            cout << memberId << " unknown" << endl;
            return;
        }

        const Member &member = entry->second;

        cout << member.id << " " << member.name
             << " loans=" << member.loans.size() << " fines=" << member.fines;

        for (int copyId : member.loans) {
            cout << " copy" << copyId;
        }

        cout << endl;
    }
};

int main() {
    Library library;

    library.addBook("ISBN-1", "The C++ Programming Language", "Stroustrup");
    library.addBook("ISBN-2", "Introduction to Algorithms", "CLRS");
    library.addBook("ISBN-3", "Designing Data-Intensive Applications",
                    "Kleppmann");

    cout << library.addBook("ISBN-1", "Duplicate", "Nobody") << endl;  // 0

    int copy1 = library.addCopy("ISBN-1");
    int copy2 = library.addCopy("ISBN-1");
    int copy3 = library.addCopy("ISBN-2");
    int copy4 = library.addCopy("ISBN-3");

    cout << copy1 << " " << copy2 << " " << copy3 << " " << copy4 << endl;
    // 1 2 3 4
    cout << library.addCopy("ISBN-9") << endl;  // -1

    library.addMember(10, "Asha");
    library.addMember(11, "Ben");
    library.addMember(12, "Chen");

    cout << library.addMember(10, "Duplicate") << endl;  // 0

    library.printTitle("ISBN-1");  // copies=2 available=2 waiting=0

    // Asha borrows both copies of ISBN-1.
    cout << library.borrow(10, "ISBN-1", 0) << endl;  // 1
    cout << library.borrow(10, "ISBN-1", 1) << endl;  // 1

    library.printMember(10);       // loans=2 fines=0 copy1 copy2
    library.printTitle("ISBN-1");  // copies=2 available=0 waiting=0

    // The title is out, so Ben cannot borrow it; he reserves it instead.
    cout << library.borrow(11, "ISBN-1", 2) << endl;  // 0
    cout << library.reserve(11, "ISBN-1") << endl;    // 1
    cout << library.reserve(11, "ISBN-1") << endl;    // 0 (already queued)
    library.printTitle("ISBN-1");                     // available=0 waiting=1

    // Chen joins the queue behind Ben.
    cout << library.reserve(12, "ISBN-1") << endl;  // 1

    // Asha returns one copy on day 20. It is 6 days late: 6 * 2 = 12.
    cout << library.returnCopy(10, copy1, 20) << endl;  // 12

    // The copy goes straight to Ben, so it is not available.
    library.printTitle("ISBN-1");  // copies=2 available=0 waiting=1
    library.printMember(11);       // loans=1 fines=0 copy1
    library.printMember(10);       // loans=1 fines=12 copy2

    // Chen is first in the queue now, but both copies are still out.
    cout << library.borrow(12, "ISBN-1", 20) << endl;  // 0

    // Asha pays part of the fine.
    cout << library.payFine(10, 5) << endl;    // 1
    cout << library.payFine(10, 100) << endl;  // 0
    library.printMember(10);                   // fines=7

    // Ben returns the copy on day 30: due day 34, so no fine.
    cout << library.returnCopy(11, copy1, 30) << endl;  // 0
    library.printMember(12);  // Chen received the copy

    // Wrong borrower cannot return a copy.
    cout << library.returnCopy(12, copy2, 31) << endl;  // -1

    // Loan limit: Asha already holds copy2, then borrows two more titles.
    cout << library.borrow(10, "ISBN-2", 30) << endl;  // 1
    cout << library.borrow(10, "ISBN-3", 30) << endl;  // 1
    cout << library.borrow(10, "ISBN-1", 30) << endl;  // 0 (3-loan limit)

    library.printMember(10);  // loans=3

    // Unknown ids fail cleanly.
    cout << library.borrow(99, "ISBN-1", 1) << endl;  // 0
    cout << library.returnCopy(10, 999, 1) << endl;   // -1
    cout << library.reserve(99, "ISBN-1") << endl;    // 0

    // Returning a copy with no queue makes it available.
    cout << library.returnCopy(12, copy1, 40) << endl;  // 0
    library.printTitle("ISBN-1");  // copies=2 available=1 waiting=0

    return 0;
}

/**
 * Time complexity:
 *   addBook / addMember -> O(log n)
 *   addCopy -> O(log n)
 *   borrow -> O(copies) to find an available copy, plus O(log n) map work
 *   returnCopy -> O(log n) for the map lookups, plus O(loans) to erase the loan
 *   and O(1) for the reservation handoff
 *   reserve -> O(queue length) for the duplicate check
 *   printTitle -> O(copies)
 * Space complexity -> O(books + copies + members + reservations)
 *
 * Special cases:
 *   - unknown isbn, member, or copy -> false or -1, never a crash
 *   - duplicate isbn or member id -> rejected
 *   - a member at the 3-loan limit -> borrow fails
 *   - every copy is out -> borrow fails; the member can reserve
 *   - duplicate reservation -> rejected
 *   - a reservation queue blocks members who are not at the front
 *   - returning a copy that is already available or belongs to another member
 *     -> -1
 *   - a late return accrues a fine; on time or early is free
 *   - a reserver at the loan limit keeps the queue position and the returned
 *     copy stays available
 *   - paying more than the outstanding fine -> rejected
 *   - a returned copy with an empty queue becomes available
 *
 * The reservation handoff is the subtle part. The copy is marked available
 * before the queue is checked, and `lend` flips it back to borrowed. If the
 * first reserver cannot take it, the copy must remain available rather than
 * being held hostage by the queue.
 *
 * Extensions:
 *   - renewals with a renewal limit
 *   - holds with an expiry window
 *   - per-member loan limits by category (student, faculty)
 *   - search by title, author, or subject
 *   - a waitlist notification service instead of immediate handoff
 */

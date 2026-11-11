/**
 * Hotel Booking
 *
 * A hotel has rooms and reservations. The central rule is that two active
 * reservations for the same room may not overlap.
 *
 * Dates are integer day numbers (day 0, day 1, ...). A stay is the half-open
 * interval [checkIn, checkOut):
 *
 *   [10, 13) covers nights 10, 11, and 12
 *   [13, 15) starts on the checkout day of the previous stay
 *
 * Two intervals overlap exactly when:
 *
 *   newCheckIn < existingCheckOut && existingCheckIn < newCheckOut
 *
 * The half-open rule is what allows a checkout and a check-in on the same day.
 * Treating the interval as closed would block that turnover and lose a night.
 *
 * Reservation lifecycle:
 *   Reserved -> CheckedIn -> Completed
 *   Reserved -> Cancelled
 *
 * Availability counts Reserved and CheckedIn reservations; Completed and
 * Cancelled reservations are ignored.
 *
 * Pricing: nights * nightly rate. Cancellation before check-in is fully
 * refunded; cancellation on or after check-in is not refunded in this model.
 *
 * Concurrency: two booking requests can read availability at the same time.
 * A real system locks the room's date range or uses a conditional insert with
 * a database exclusion constraint, then confirms the reservation. The in-memory
 * model here is single-threaded.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

enum class ReservationStatus { Reserved, CheckedIn, Completed, Cancelled };

string statusName(ReservationStatus status) {
    switch (status) {
        case ReservationStatus::Reserved:
            return "reserved";
        case ReservationStatus::CheckedIn:
            return "checked-in";
        case ReservationStatus::Completed:
            return "completed";
        case ReservationStatus::Cancelled:
            return "cancelled";
    }

    return "unknown";
}

struct Room {
    int id;
    string type;
    long long pricePerNight;
    int capacity;
};

struct Reservation {
    int id;
    int roomId;
    string guest;
    int checkIn;
    int checkOut;
    long long total;
    ReservationStatus status = ReservationStatus::Reserved;
};

class Hotel {
    map<int, Room> rooms;
    map<int, Reservation> reservations;
    int nextReservation = 1;

    bool active(ReservationStatus status) const {
        return status == ReservationStatus::Reserved ||
               status == ReservationStatus::CheckedIn;
    }

    bool overlaps(int roomId, int checkIn, int checkOut) const {
        for (const auto &[id, reservation] : reservations) {
            if (reservation.roomId != roomId) {
                continue;
            }
            if (!active(reservation.status)) {
                continue;
            }

            if (checkIn < reservation.checkOut &&
                reservation.checkIn < checkOut) {
                return true;
            }
        }

        return false;
    }

   public:
    bool addRoom(int id, const string &type, long long pricePerNight,
                 int capacity) {
        if (rooms.count(id)) {
            return false;
        }
        if (pricePerNight <= 0 || capacity <= 0) {
            return false;
        }

        rooms[id] = {id, type, pricePerNight, capacity};
        return true;
    }

    bool isAvailable(int roomId, int checkIn, int checkOut) const {
        if (!rooms.count(roomId)) {
            return false;
        }
        if (checkIn < 0 || checkOut <= checkIn) {
            return false;
        }

        return !overlaps(roomId, checkIn, checkOut);
    }

    // Room ids sorted by price, then id.
    vector<int> search(const string &type, int checkIn, int checkOut) const {
        vector<int> result;

        if (checkIn < 0 || checkOut <= checkIn) {
            return result;
        }

        for (const auto &[id, room] : rooms) {
            if (room.type != type) {
                continue;
            }
            if (!isAvailable(id, checkIn, checkOut)) {
                continue;
            }

            result.push_back(id);
        }

        sort(result.begin(), result.end(), [this](int first, int second) {
            const Room &a = rooms.at(first);
            const Room &b = rooms.at(second);

            if (a.pricePerNight != b.pricePerNight) {
                return a.pricePerNight < b.pricePerNight;
            }

            return a.id < b.id;
        });

        return result;
    }

    // Returns the reservation id, or -1 when the room is unknown, the dates are
    // invalid, or the room is already booked for an overlapping stay.
    int book(const string &guest, int roomId, int checkIn, int checkOut) {
        if (guest.empty()) {
            return -1;
        }
        if (!isAvailable(roomId, checkIn, checkOut)) {
            return -1;
        }

        const Room &room = rooms.at(roomId);
        int nights = checkOut - checkIn;
        long long total = nights * room.pricePerNight;

        int id = nextReservation++;
        reservations[id] = {id,
                            roomId,
                            guest,
                            checkIn,
                            checkOut,
                            total,
                            ReservationStatus::Reserved};

        return id;
    }

    // Books the cheapest available room of the requested type.
    int bookBest(const string &guest, const string &type, int checkIn,
                 int checkOut) {
        vector<int> available = search(type, checkIn, checkOut);

        if (available.empty()) {
            return -1;
        }

        return book(guest, available.front(), checkIn, checkOut);
    }

    bool checkIn(int reservationId, int day) {
        auto entry = reservations.find(reservationId);
        if (entry == reservations.end()) {
            return false;
        }

        Reservation &reservation = entry->second;

        if (reservation.status != ReservationStatus::Reserved) {
            return false;
        }
        if (day < reservation.checkIn || day >= reservation.checkOut) {
            return false;
        }

        reservation.status = ReservationStatus::CheckedIn;
        return true;
    }

    bool checkOut(int reservationId, int day) {
        auto entry = reservations.find(reservationId);
        if (entry == reservations.end()) {
            return false;
        }

        Reservation &reservation = entry->second;

        if (reservation.status != ReservationStatus::CheckedIn) {
            return false;
        }
        if (day < reservation.checkIn) {
            return false;
        }

        reservation.status = ReservationStatus::Completed;
        return true;
    }

    // Returns the refund amount, or -1 when cancellation is not allowed.
    long long cancel(int reservationId, int day) {
        auto entry = reservations.find(reservationId);
        if (entry == reservations.end()) {
            return -1;
        }

        Reservation &reservation = entry->second;

        if (reservation.status != ReservationStatus::Reserved) {
            return -1;
        }

        reservation.status = ReservationStatus::Cancelled;

        // Full refund only before the stay begins.
        return day < reservation.checkIn ? reservation.total : 0;
    }

    string status(int reservationId) const {
        auto entry = reservations.find(reservationId);
        return entry == reservations.end() ? "unknown"
                                           : statusName(entry->second.status);
    }

    long long total(int reservationId) const {
        auto entry = reservations.find(reservationId);
        return entry == reservations.end() ? -1 : entry->second.total;
    }

    void printReservation(int reservationId) const {
        auto entry = reservations.find(reservationId);

        if (entry == reservations.end()) {
            cout << reservationId << " unknown" << endl;
            return;
        }

        const Reservation &reservation = entry->second;

        cout << "R" << reservation.id << " " << reservation.guest
             << " room=" << reservation.roomId << " [" << reservation.checkIn
             << ", " << reservation.checkOut << ") total=" << reservation.total
             << " " << statusName(reservation.status) << endl;
    }

    void printSearch(const vector<int> &ids) const {
        cout << "[";

        for (size_t i = 0; i < ids.size(); i++) {
            if (i > 0) {
                cout << ", ";
            }
            cout << ids[i];
        }

        cout << "]" << endl;
    }
};

int main() {
    Hotel hotel;

    hotel.addRoom(101, "single", 100, 1);
    hotel.addRoom(102, "single", 120, 1);
    hotel.addRoom(201, "double", 180, 2);
    hotel.addRoom(202, "double", 200, 2);
    hotel.addRoom(301, "suite", 300, 4);

    cout << hotel.addRoom(101, "duplicate", 1, 1) << endl;  // 0
    cout << hotel.addRoom(999, "bad", 0, 1) << endl;        // 0

    // Search a type and a date range.
    hotel.printSearch(hotel.search("single", 10, 13));  // [101, 102]
    hotel.printSearch(hotel.search("double", 10, 13));  // [201, 202]
    hotel.printSearch(hotel.search("suite", 10, 13));   // [301]

    // Asha books room 101 for nights 10, 11, and 12.
    int first = hotel.book("Asha", 101, 10, 13);
    cout << first << endl;  // 1
    hotel.printReservation(first);
    // R1 Asha room=101 [10, 13) total=300 reserved

    // Room 101 is no longer available for an overlapping stay.
    hotel.printSearch(hotel.search("single", 10, 13));  // [102]
    cout << hotel.book("Ben", 101, 12, 14) << endl;     // -1 (overlap)
    cout << hotel.book("Ben", 101, 9, 10) << endl;      // 2 (back-to-back:
                                                        // [9, 10) ends when
                                                        // [10, 13) starts)
    hotel.printReservation(2);  // R2 Ben room=101 [9, 10) total=100 reserved

    // A checkout-day check-in is allowed: [13, 15) starts when [10, 13) ends.
    int third = hotel.book("Chen", 101, 13, 15);
    cout << third << endl;  // 3
    hotel.printReservation(third);
    // R3 Chen room=101 [13, 15) total=200 reserved

    // Now the full range [9, 15) overlaps all three stays.
    cout << hotel.isAvailable(101, 9, 15) << endl;  // 0

    // Invalid dates.
    cout << hotel.book("Dev", 102, 12, 12) << endl;  // -1 (zero nights)
    cout << hotel.book("Dev", 102, 13, 12) << endl;  // -1 (checkout before in)
    cout << hotel.book("Dev", 102, -1, 2) << endl;   // -1 (negative day)
    cout << hotel.isAvailable(999, 1, 2) << endl;    // 0 (unknown room)

    // Unknown reservation ids.
    cout << hotel.checkIn(99, 10) << " " << hotel.checkOut(99, 10) << " "
         << hotel.cancel(99, 10) << endl;
    // 0 0 -1

    // Check-in must be on or after the arrival day and before checkout.
    cout << hotel.checkIn(first, 9) << endl;   // 0 (too early)
    cout << hotel.checkIn(first, 10) << endl;  // 1
    hotel.printReservation(first);             // checked-in

    // A checked-in reservation cannot be cancelled.
    cout << hotel.cancel(first, 10) << endl;  // -1

    // Checkout completes the stay and frees the dates.
    cout << hotel.checkOut(first, 13) << endl;       // 1
    hotel.printReservation(first);                   // completed
    cout << hotel.isAvailable(101, 10, 13) << endl;  // 1

    // Room 102 was never booked, so it is still available.
    hotel.printSearch(hotel.search("single", 10, 13));  // [101, 102]

    // Cancel Ben's stay before arrival: full refund.
    cout << hotel.cancel(2, 5) << endl;  // 100
    hotel.printReservation(2);           // cancelled

    // Cancel Chen's stay on the arrival day: no refund under this policy.
    cout << hotel.cancel(3, 13) << endl;  // 0
    hotel.printReservation(3);            // cancelled

    // Cancelling twice fails.
    cout << hotel.cancel(3, 13) << endl;  // -1

    // A cancelled range becomes bookable again.
    cout << hotel.isAvailable(101, 13, 15) << endl;  // 1
    int fourth = hotel.book("Dev", 101, 13, 15);
    cout << fourth << endl;  // 4

    // Best-room booking picks the cheapest available single.
    int fifth = hotel.bookBest("Eve", "single", 20, 22);
    cout << fifth << endl;  // 5 (room 101, 200 total)
    hotel.printReservation(fifth);
    // R5 Eve room=101 [20, 22) total=200 reserved

    // The suite is more expensive but still available.
    hotel.printSearch(hotel.search("suite", 20, 22));  // [301]

    // Checkout before check-in is rejected.
    cout << hotel.checkOut(fourth, 12) << endl;  // 0 (not checked in)

    // A reservation that is checked in can check out, and the room frees up.
    hotel.checkIn(fourth, 13);
    cout << hotel.checkOut(fourth, 15) << endl;      // 1
    cout << hotel.isAvailable(101, 13, 15) << endl;  // 1

    // The total is computed from nights, not from the number of calendar days
    // touched: [10, 13) is three nights.
    cout << hotel.total(1) << endl;  // 300

    return 0;
}

/**
 * Time complexity:
 *   addRoom -> O(log rooms)
 *   isAvailable -> O(reservations) to scan overlapping stays
 *   search -> O(rooms * reservations + rooms log rooms)
 *   book -> O(reservations) for the availability check
 *   checkIn / checkOut / cancel -> O(log reservations)
 * Space complexity -> O(rooms + reservations)
 *
 * Special cases:
 *   - checkIn < 0 -> invalid
 *   - checkOut <= checkIn -> invalid (at least one night)
 *   - unknown room -> not available
 *   - overlapping active reservation -> booking rejected
 *   - a stay that starts on another stay's checkout day -> allowed, because the
 *     interval is half-open
 *   - cancelled and completed reservations do not block availability
 *   - check-in before the arrival day or on/after checkout -> rejected
 *   - check-out before check-in or without checking in -> rejected
 *   - cancellation after check-in -> rejected
 *   - cancellation before arrival -> full refund
 *   - cancellation on or after arrival -> no refund in this policy
 *   - cancelling twice -> -1
 *   - a cancelled range becomes bookable again
 *   - price is nights * rate; a one-night stay costs one rate
 *
 * The overlap test is the heart of the design. Drawing the intervals as
 * half-open ranges makes the rule symmetric and avoids the classic off-by-one
 * that blocks a same-day turnover.
 *
 * Production concerns:
 *   - transactions: check availability and insert in one atomic step
 *   - a database exclusion constraint on (roomId, daterange) is the safest
 *     guard against concurrent double booking
 *   - overbooking policies, waitlists, and upgrade rules
 *   - seasonal rates, taxes, deposits, and cancellation windows
 *   - idempotency keys so a retried booking does not create two reservations
 */

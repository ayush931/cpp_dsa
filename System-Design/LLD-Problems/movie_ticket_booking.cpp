/**
 * Movie Ticket Booking
 *
 * A cinema booking system must reserve seats without double-selling them, even
 * when several users are choosing at the same time. The core idea is a
 * temporary lock followed by a confirmation:
 *
 *   Available --lock--> Locked --confirm--> Booked
 *   Locked --expiry/cancel--> Available
 *   Booked --cancel before showtime--> Available
 *
 * A lock has an owner and an expiry time. While it is active, nobody else can
 * take those seats. If the user does not pay before the expiry, the lock is
 * released automatically and the seats return to the pool.
 *
 * Seat pricing:
 *   rows 0-1 -> regular, base price
 *   rows 2-3 -> premium, base + 50
 *   row 4+   -> recliner, base + 120
 *
 * Booking lifecycle:
 *   Locked -> Confirmed -> Cancelled
 *   Locked -> Cancelled
 *   Locked -> Expired
 *
 * Atomicity:
 *   lockSeats validates every requested seat before changing any of them. If
 *   one seat is taken, the whole request fails and no partial lock is left
 *   behind. The same rule applies to confirmation and cancellation.
 *
 * Concurrency:
 *   this model is single-threaded. A real system performs the validate-and-lock
 *   step in a transaction or a Redis Lua script keyed by show id, with a unique
 *   constraint on (showId, seatId). The lock TTL prevents a crashed client from
 *   holding seats forever.
 *
 * A production system also adds payments, convenience fees, seat categories
 * per auditorium, showtime transitions, refund windows, and idempotency keys
 * for retries.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

enum class SeatStatus { Available, Locked, Booked };

enum class BookingStatus { Locked, Confirmed, Cancelled, Expired };

string seatStatusName(SeatStatus status) {
    switch (status) {
        case SeatStatus::Available:
            return "available";
        case SeatStatus::Locked:
            return "locked";
        case SeatStatus::Booked:
            return "booked";
    }

    return "unknown";
}

string bookingStatusName(BookingStatus status) {
    switch (status) {
        case BookingStatus::Locked:
            return "locked";
        case BookingStatus::Confirmed:
            return "confirmed";
        case BookingStatus::Cancelled:
            return "cancelled";
        case BookingStatus::Expired:
            return "expired";
    }

    return "unknown";
}

struct Seat {
    int row;
    int column;
    string type;
    long long price;
    SeatStatus status = SeatStatus::Available;
    int bookingId = -1;
};

struct Show {
    int id;
    string movie;
    long long startTime;
    map<pair<int, int>, Seat> seats;
};

struct Booking {
    int id;
    int showId;
    string user;
    vector<pair<int, int>> seats;
    long long total = 0;
    long long expiresAt = 0;
    BookingStatus status = BookingStatus::Locked;
};

class MovieTicketSystem {
    map<int, Show> shows;
    map<int, Booking> bookings;
    int nextBooking = 1;

    Seat *findSeat(int showId, int row, int column) {
        auto show = shows.find(showId);
        if (show == shows.end()) {
            return nullptr;
        }

        auto seat = show->second.seats.find({row, column});
        if (seat == show->second.seats.end()) {
            return nullptr;
        }

        return &seat->second;
    }

    void releaseSeats(Booking &booking) {
        auto show = shows.find(booking.showId);
        if (show == shows.end()) {
            return;
        }

        for (const auto &position : booking.seats) {
            auto seat = show->second.seats.find(position);
            if (seat == show->second.seats.end()) {
                continue;
            }

            if (seat->second.bookingId != booking.id) {
                continue;
            }

            seat->second.status = SeatStatus::Available;
            seat->second.bookingId = -1;
        }
    }

    void expireBookings(long long now) {
        for (auto &[id, booking] : bookings) {
            if (booking.status != BookingStatus::Locked) {
                continue;
            }
            if (now < booking.expiresAt) {
                continue;
            }

            releaseSeats(booking);
            booking.status = BookingStatus::Expired;
        }
    }

   public:
    bool addShow(int id, const string &movie, long long startTime, int rows,
                 int columns, long long basePrice) {
        if (shows.count(id)) {
            return false;
        }
        if (rows <= 0 || columns <= 0 || basePrice <= 0) {
            return false;
        }

        Show show{id, movie, startTime, {}};

        for (int row = 0; row < rows; row++) {
            for (int column = 0; column < columns; column++) {
                string type = "regular";
                long long price = basePrice;

                if (row >= 4) {
                    type = "recliner";
                    price += 120;
                } else if (row >= 2) {
                    type = "premium";
                    price += 50;
                }

                show.seats[{row, column}] = {
                    row, column, type, price, SeatStatus::Available, -1};
            }
        }

        shows[id] = show;
        return true;
    }

    // Locks every requested seat or none of them. Returns the booking id, or -1
    // on an invalid show, an invalid seat, an unavailable seat, or a started
    // show.
    int lockSeats(int showId, const string &user,
                  const vector<pair<int, int>> &positions, long long now,
                  long long lockTtl = 5) {
        expireBookings(now);

        auto show = shows.find(showId);
        if (show == shows.end()) {
            return -1;
        }
        if (user.empty() || positions.empty()) {
            return -1;
        }
        if (now >= show->second.startTime) {
            return -1;
        }
        if (lockTtl <= 0) {
            return -1;
        }

        set<pair<int, int>> unique;

        for (const auto &position : positions) {
            if (!unique.insert(position).second) {
                return -1;  // duplicate seat
            }

            auto seat = show->second.seats.find(position);
            if (seat == show->second.seats.end()) {
                return -1;
            }
            if (seat->second.status != SeatStatus::Available) {
                return -1;
            }
        }

        int id = nextBooking++;
        Booking booking{id,
                        showId,
                        user,
                        positions,
                        0,
                        now + lockTtl,
                        BookingStatus::Locked};

        for (const auto &position : positions) {
            Seat &seat = show->second.seats.at(position);

            seat.status = SeatStatus::Locked;
            seat.bookingId = id;
            booking.total += seat.price;
        }

        bookings[id] = booking;
        return id;
    }

    bool confirm(int bookingId, long long now) {
        expireBookings(now);

        auto entry = bookings.find(bookingId);
        if (entry == bookings.end()) {
            return false;
        }

        Booking &booking = entry->second;

        if (booking.status != BookingStatus::Locked) {
            return false;
        }
        if (now >= booking.expiresAt) {
            return false;
        }

        auto show = shows.find(booking.showId);
        if (show == shows.end()) {
            return false;
        }

        for (const auto &position : booking.seats) {
            auto seat = show->second.seats.find(position);
            if (seat == show->second.seats.end()) {
                return false;
            }
            if (seat->second.status != SeatStatus::Locked) {
                return false;
            }
            if (seat->second.bookingId != booking.id) {
                return false;
            }
        }

        for (const auto &position : booking.seats) {
            Seat &seat = show->second.seats.at(position);
            seat.status = SeatStatus::Booked;
            seat.bookingId = booking.id;
        }

        booking.status = BookingStatus::Confirmed;
        return true;
    }

    // Returns the refund amount, or -1 when cancellation is not allowed.
    long long cancel(int bookingId, long long now) {
        expireBookings(now);

        auto entry = bookings.find(bookingId);
        if (entry == bookings.end()) {
            return -1;
        }

        Booking &booking = entry->second;
        auto show = shows.find(booking.showId);
        if (show == shows.end()) {
            return -1;
        }

        if (booking.status == BookingStatus::Locked) {
            releaseSeats(booking);
            booking.status = BookingStatus::Cancelled;
            return 0;
        }

        if (booking.status == BookingStatus::Confirmed) {
            if (now >= show->second.startTime) {
                return -1;
            }

            releaseSeats(booking);
            booking.status = BookingStatus::Cancelled;
            return booking.total;
        }

        return -1;
    }

    string seatStatus(int showId, int row, int column) {
        Seat *seat = findSeat(showId, row, column);
        return seat == nullptr ? "unknown" : seatStatusName(seat->status);
    }

    string status(int bookingId) const {
        auto entry = bookings.find(bookingId);
        return entry == bookings.end()
                   ? "unknown"
                   : bookingStatusName(entry->second.status);
    }

    long long total(int bookingId) const {
        auto entry = bookings.find(bookingId);
        return entry == bookings.end() ? -1 : entry->second.total;
    }

    int availableSeats(int showId) {
        auto show = shows.find(showId);
        if (show == shows.end()) {
            return -1;
        }

        int count = 0;

        for (const auto &[position, seat] : show->second.seats) {
            if (seat.status == SeatStatus::Available) {
                count++;
            }
        }

        return count;
    }

    void printBooking(int bookingId) const {
        auto entry = bookings.find(bookingId);

        if (entry == bookings.end()) {
            cout << bookingId << " unknown" << endl;
            return;
        }

        const Booking &booking = entry->second;

        cout << "B" << booking.id << " " << booking.user
             << " show=" << booking.showId << " seats=";

        for (std::size_t i = 0; i < booking.seats.size(); i++) {
            if (i > 0) {
                cout << ",";
            }
            cout << "(" << booking.seats[i].first << ","
                 << booking.seats[i].second << ")";
        }

        cout << " total=" << booking.total << " expires=" << booking.expiresAt
             << " " << bookingStatusName(booking.status) << endl;
    }
};

int main() {
    MovieTicketSystem system;

    // Five rows, six seats per row. Row 0-1 regular, 2-3 premium, 4 recliner.
    system.addShow(1, "Dune", 20, 5, 6, 200);
    system.addShow(2, "Interstellar", 10, 4, 4, 150);

    cout << system.addShow(1, "Duplicate", 30, 1, 1, 100) << endl;  // 0
    cout << system.addShow(3, "Bad", 30, 0, 5, 100) << endl;        // 0

    cout << system.availableSeats(1) << endl;  // 30

    // Asha locks two regular seats at time 0 for 5 units.
    int first = system.lockSeats(1, "Asha", {{0, 0}, {0, 1}}, 0);
    cout << first << endl;  // 1
    system.printBooking(first);
    // B1 Asha show=1 seats=(0,0),(0,1) total=400 expires=5 locked

    cout << system.seatStatus(1, 0, 0) << endl;  // locked
    cout << system.availableSeats(1) << endl;    // 28

    // Ben cannot take one of the locked seats.
    cout << system.lockSeats(1, "Ben", {{0, 0}}, 1) << endl;  // -1
    cout << system.availableSeats(1) << endl;                 // 28

    // Confirming before the expiry turns the seats into permanent bookings.
    cout << system.confirm(first, 4) << endl;    // 1
    system.printBooking(first);                  // confirmed
    cout << system.seatStatus(1, 0, 0) << endl;  // booked

    // Booked seats cannot be locked.
    cout << system.lockSeats(1, "Ben", {{0, 1}}, 4) << endl;  // -1

    // Chen locks a premium seat at time 0. It expires at 5.
    int second = system.lockSeats(1, "Chen", {{2, 2}}, 0);
    cout << second << endl;                // 2
    cout << system.total(second) << endl;  // 250 (200 + 50)

    // At time 6 the lock is expired, so confirmation fails and the seat frees
    // up.
    cout << system.confirm(second, 6) << endl;   // 0
    system.printBooking(second);                 // expired
    cout << system.seatStatus(1, 2, 2) << endl;  // available
    cout << system.availableSeats(1) << endl;    // 28 (two booked, rest free)

    // The seat can be locked again after expiry.
    int third = system.lockSeats(1, "Chen", {{2, 2}}, 6);
    cout << third << endl;  // 3
    system.printBooking(third);
    // B3 Chen ... total=250 expires=11 locked

    // Cancel a lock: no refund, seats return immediately.
    cout << system.cancel(third, 7) << endl;     // 0
    cout << system.seatStatus(1, 2, 2) << endl;  // available

    // Lock and confirm a recliner, then cancel before showtime for a full
    // refund.
    int fourth = system.lockSeats(1, "Dev", {{4, 4}}, 10);
    cout << fourth << endl;                      // 4
    cout << system.total(fourth) << endl;        // 320 (200 + 120)
    cout << system.confirm(fourth, 11) << endl;  // 1
    cout << system.cancel(fourth, 12) << endl;   // 320
    system.printBooking(fourth);                 // cancelled
    cout << system.seatStatus(1, 4, 4) << endl;  // available

    // Cancellation after the show starts is rejected.
    int fifth = system.lockSeats(1, "Eve", {{4, 5}}, 18);
    cout << fifth << endl;                      // 5
    cout << system.confirm(fifth, 19) << endl;  // 1
    cout << system.cancel(fifth, 20) << endl;   // -1 (show started)
    system.printBooking(fifth);                 // confirmed

    // Locking after the show starts is rejected.
    cout << system.lockSeats(1, "Far", {{3, 3}}, 20) << endl;  // -1

    // Invalid requests.
    cout << system.lockSeats(99, "Gita", {{0, 0}}, 1) << endl;  // -1
    cout << system.lockSeats(1, "Gita", {}, 1) << endl;         // -1
    cout << system.lockSeats(1, "Gita", {{9, 9}}, 1) << endl;   // -1
    cout << system.lockSeats(1, "Gita", {{1, 1}, {1, 1}}, 1)
         << endl;                                          // -1 (duplicate)
    cout << system.lockSeats(1, "", {{1, 1}}, 1) << endl;  // -1
    cout << system.lockSeats(1, "Gita", {{1, 1}}, 1, 0) << endl;  // -1 (TTL 0)

    // A second show is independent.
    cout << system.availableSeats(2) << endl;  // 16
    int secondShow = system.lockSeats(2, "Hari", {{0, 0}}, 1);
    cout << secondShow << " " << system.seatStatus(2, 0, 0) << endl;
    // 6 locked
    cout << system.seatStatus(1, 0, 0) << endl;  // booked

    // Confirming an already confirmed booking fails.
    cout << system.confirm(first, 5) << endl;  // 0

    // Cancelling an expired booking fails.
    cout << system.cancel(second, 7) << endl;  // -1

    // Unknown ids.
    cout << system.confirm(99, 1) << " " << system.cancel(99, 1) << endl;
    // 0 -1
    cout << system.status(99) << " " << system.seatStatus(99, 0, 0) << endl;
    // unknown unknown

    return 0;
}

/**
 * Time complexity:
 *   addShow -> O(rows * columns)
 *   lockSeats -> O(bookings + requested seats) because expired locks are swept
 *   confirm -> O(bookings + seats in the booking)
 *   cancel -> O(bookings + seats in the booking)
 *   seatStatus / availableSeats -> O(log seats) or O(seats)
 * Space complexity -> O(shows * seats + bookings)
 *
 * Special cases:
 *   - unknown show or seat -> rejected
 *   - empty seat list -> rejected
 *   - duplicate seat in one request -> rejected
 *   - any seat unavailable -> the whole request fails, no partial lock
 *   - lock TTL <= 0 -> rejected
 *   - locking at or after showtime -> rejected
 *   - confirming at or after the lock expiry -> rejected and the lock expires
 *   - confirming twice -> rejected
 *   - cancelling a lock -> free, seats return
 *   - cancelling a confirmed booking before showtime -> full refund
 *   - cancelling a confirmed booking at or after showtime -> rejected
 *   - cancelling an expired or already cancelled booking -> -1
 *   - an expired lock releases its seats automatically
 *   - a released seat can be locked again
 *   - seats from different shows are independent
 *   - the same user can hold multiple bookings; a production system may limit
 *     that
 *
 * Why locks are necessary: a user needs time to pay. Without a lock, two users
 * could both see the same seat as available and both pay. With a lock, the
 * seat is reserved for a short window, and an expiry protects the inventory
 * from abandoned carts.
 *
 * Why the TTL is part of the seat state: the lock is only meaningful with an
 * owner and a deadline. Storing it on the seat makes the availability check a
 * local decision; storing it only on the booking would require scanning all
 * bookings for every seat request.
 *
 * Extensions:
 *   - payment capture before confirmation
 *   - convenience fees and taxes
 *   - seat maps with aisles and wheelchair spaces
 *   - per-show pricing and dynamic pricing
 *   - refund windows with partial refunds
 *   - idempotency keys for retries
 *   - a distributed lock or database constraint for multi-node safety
 */

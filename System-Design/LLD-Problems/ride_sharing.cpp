/**
 * Ride Sharing
 *
 * A ride-hailing system matches riders with nearby available drivers, tracks
 * the ride through a state machine, and computes a fare when the ride ends.
 *
 * Objects:
 *   Location -> integer (x, y) coordinates
 *   Driver   -> location, availability, rating, completed rides
 *   Rider    -> identity
 *   Ride     -> pickup, drop, driver, status, distance, fare
 *
 * Ride lifecycle:
 *   Requested -> Accepted -> Started -> Completed
 *   Requested or Accepted -> Cancelled
 *
 * Matching:
 *   - consider only available drivers
 *   - ignore drivers farther than the pickup radius (15 units)
 *   - choose the smallest pickup distance
 *   - break ties by higher rating, then lower driver id
 *
 * Distance is Manhattan distance: |x1 - x2| + |y1 - y2|. It is cheap to
 * compute and models a grid of streets better than straight-line distance.
 * A real system uses road-network ETA, traffic, driver direction, and
 * acceptance probability.
 *
 * Fare:
 *   base 50 + 12 * ride distance, with a minimum of 80
 *
 * A ride request is created even when no driver is available. It stays in
 * Requested state so a later match can pick it up when a driver frees up. This
 * is the difference between "no drivers right now" and "the request failed".
 *
 * Driver location updates to the drop-off point on completion, so the next
 * match starts from where the driver actually is.
 *
 * Cancellation:
 *   - before a driver accepts -> free
 *   - after acceptance but before the trip starts -> cancellation fee 20
 *   - after the trip starts -> not allowed
 *
 * A real system adds payments, surge multipliers, pooled rides, driver
 * dispatch offers with timeouts, and a rating system for both sides.
 */

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

struct Location {
    int x;
    int y;
};

int distanceBetween(const Location &a, const Location &b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

struct Driver {
    int id;
    string name;
    Location location;
    bool available = true;
    double rating = 5.0;
    int completedRides = 0;
};

struct Rider {
    int id;
    string name;
};

enum class RideStatus { Requested, Accepted, Started, Completed, Cancelled };

string statusName(RideStatus status) {
    switch (status) {
        case RideStatus::Requested:
            return "requested";
        case RideStatus::Accepted:
            return "accepted";
        case RideStatus::Started:
            return "started";
        case RideStatus::Completed:
            return "completed";
        case RideStatus::Cancelled:
            return "cancelled";
    }

    return "unknown";
}

struct Ride {
    int id;
    int riderId;
    int driverId = -1;
    Location pickup;
    Location drop;
    int distance = 0;
    RideStatus status = RideStatus::Requested;
    long long fare = 0;
};

class RideSharingSystem {
    map<int, Driver> drivers;
    map<int, Rider> riders;
    map<int, Ride> rides;
    int nextRide = 1;
    int maximumPickupDistance;

    long long calculateFare(int distance) const {
        long long fare = 50 + 12LL * distance;
        return max(80LL, fare);
    }

    void freeDriver(int driverId) {
        auto entry = drivers.find(driverId);
        if (entry == drivers.end()) {
            return;
        }

        entry->second.available = true;
    }

   public:
    explicit RideSharingSystem(int maximumPickupDistance = 15)
        : maximumPickupDistance(maximumPickupDistance) {}

    bool addDriver(int id, const string &name, Location location,
                   double rating = 5.0) {
        if (drivers.count(id)) {
            return false;
        }
        if (rating < 0 || rating > 5) {
            return false;
        }

        drivers[id] = {id, name, location, true, rating, 0};
        return true;
    }

    bool addRider(int id, const string &name) {
        if (riders.count(id)) {
            return false;
        }

        riders[id] = {id, name};
        return true;
    }

    // Returns the ride id, or -1 for an unknown rider or an invalid trip. The
    // ride is created even when no driver is currently available.
    int requestRide(int riderId, Location pickup, Location drop) {
        if (!riders.count(riderId)) {
            return -1;
        }

        int distance = distanceBetween(pickup, drop);
        if (distance == 0) {
            return -1;
        }

        int id = nextRide++;

        rides[id] = {
            id, riderId, -1, pickup, drop, distance, RideStatus::Requested, 0};

        tryMatch(id);

        return id;
    }

    // Finds the best available driver for a requested ride. Returns the driver
    // id, or -1 when no driver is within range.
    int tryMatch(int rideId) {
        auto entry = rides.find(rideId);
        if (entry == rides.end()) {
            return -1;
        }

        Ride &ride = entry->second;

        if (ride.status != RideStatus::Requested) {
            return -1;
        }

        int bestDriver = -1;
        int bestDistance = INT_MAX;
        double bestRating = -1;

        for (const auto &[id, driver] : drivers) {
            if (!driver.available) {
                continue;
            }

            int pickupDistance = distanceBetween(driver.location, ride.pickup);

            if (pickupDistance > maximumPickupDistance) {
                continue;
            }

            bool better = pickupDistance < bestDistance;

            if (pickupDistance == bestDistance) {
                better = driver.rating > bestRating ||
                         (driver.rating == bestRating && id < bestDriver);
            }

            if (better) {
                bestDriver = id;
                bestDistance = pickupDistance;
                bestRating = driver.rating;
            }
        }

        if (bestDriver == -1) {
            return -1;
        }

        drivers.at(bestDriver).available = false;
        ride.driverId = bestDriver;
        ride.status = RideStatus::Accepted;

        return bestDriver;
    }

    bool startRide(int rideId) {
        auto entry = rides.find(rideId);
        if (entry == rides.end()) {
            return false;
        }

        Ride &ride = entry->second;

        if (ride.status != RideStatus::Accepted) {
            return false;
        }

        ride.status = RideStatus::Started;
        return true;
    }

    bool completeRide(int rideId) {
        auto entry = rides.find(rideId);
        if (entry == rides.end()) {
            return false;
        }

        Ride &ride = entry->second;

        if (ride.status != RideStatus::Started) {
            return false;
        }

        ride.status = RideStatus::Completed;
        ride.fare = calculateFare(ride.distance);

        Driver &driver = drivers.at(ride.driverId);
        driver.location = ride.drop;
        driver.completedRides++;
        driver.available = true;

        return true;
    }

    // Returns the cancellation fee (0 or 20), or -1 when cancellation is not
    // allowed.
    int cancelRide(int rideId) {
        auto entry = rides.find(rideId);
        if (entry == rides.end()) {
            return -1;
        }

        Ride &ride = entry->second;

        if (ride.status == RideStatus::Requested) {
            ride.status = RideStatus::Cancelled;
            return 0;
        }

        if (ride.status == RideStatus::Accepted) {
            ride.status = RideStatus::Cancelled;
            freeDriver(ride.driverId);
            return 20;
        }

        return -1;
    }

    string status(int rideId) const {
        auto entry = rides.find(rideId);
        return entry == rides.end() ? "unknown"
                                    : statusName(entry->second.status);
    }

    long long fare(int rideId) const {
        auto entry = rides.find(rideId);
        return entry == rides.end() ? -1 : entry->second.fare;
    }

    int driverOf(int rideId) const {
        auto entry = rides.find(rideId);
        return entry == rides.end() ? -1 : entry->second.driverId;
    }

    bool driverAvailable(int driverId) const {
        auto entry = drivers.find(driverId);
        return entry == drivers.end() ? false : entry->second.available;
    }

    Location driverLocation(int driverId) const {
        auto entry = drivers.find(driverId);
        return entry == drivers.end() ? Location{-1, -1}
                                      : entry->second.location;
    }

    void printRide(int rideId) const {
        auto entry = rides.find(rideId);

        if (entry == rides.end()) {
            cout << rideId << " unknown" << endl;
            return;
        }

        const Ride &ride = entry->second;

        cout << "R" << ride.id << " rider=" << ride.riderId
             << " driver=" << ride.driverId << " (" << ride.pickup.x << ","
             << ride.pickup.y << ")->(" << ride.drop.x << "," << ride.drop.y
             << ") distance=" << ride.distance << " fare=" << ride.fare << " "
             << statusName(ride.status) << endl;
    }

    void printDrivers() const {
        for (const auto &[id, driver] : drivers) {
            cout << "D" << id << " " << driver.name << " (" << driver.location.x
                 << "," << driver.location.y << ") "
                 << (driver.available ? "available" : "busy")
                 << " rides=" << driver.completedRides
                 << " rating=" << driver.rating << endl;
        }
    }
};

int main() {
    RideSharingSystem system(15);

    system.addDriver(1, "Ravi", {0, 0}, 4.8);
    system.addDriver(2, "Mira", {5, 5}, 4.9);
    system.addDriver(3, "Zoya", {20, 20}, 5.0);

    cout << system.addDriver(1, "Duplicate", {0, 0}) << endl;  // 0
    cout << system.addDriver(4, "Bad", {0, 0}, 6.0) << endl;   // 0

    system.addRider(10, "Asha");
    system.addRider(11, "Ben");
    system.addRider(12, "Chen");
    system.addRider(13, "Dev");

    // Asha: pickup (2,2), drop (10,2), distance 8.
    // D1 is 4 away, D2 is 6 away, D3 is 36 away -> D1.
    int first = system.requestRide(10, {2, 2}, {10, 2});
    cout << first << " " << system.driverOf(first) << endl;  // 1 1
    system.printRide(first);
    // R1 rider=10 driver=1 (2,2)->(10,2) distance=8 fare=0 accepted

    // Ben: pickup (4,4), drop (4,10), distance 6.
    // D2 is 2 away and available -> D2.
    int second = system.requestRide(11, {4, 4}, {4, 10});
    cout << second << " " << system.driverOf(second) << endl;  // 2 2

    // Chen: pickup (15,15), drop (18,15), distance 3.
    // D3 is 10 away, within the 15-unit radius -> D3.
    int third = system.requestRide(12, {15, 15}, {18, 15});
    cout << third << " " << system.driverOf(third) << endl;  // 3 3

    // Dev: pickup (8,2), drop (8,5). Every driver is busy, so the ride is
    // created in Requested state.
    int fourth = system.requestRide(13, {8, 2}, {8, 5});
    cout << fourth << " " << system.driverOf(fourth) << endl;  // 4 -1
    system.printRide(fourth);
    // R4 rider=13 driver=-1 (8,2)->(8,5) distance=3 fare=0 requested

    // Invalid trips and unknown riders.
    cout << system.requestRide(10, {1, 1}, {1, 1}) << endl;  // -1 (same point)
    cout << system.requestRide(99, {1, 1}, {2, 2}) << endl;  // -1

    // Start and complete Asha's ride. Fare = 50 + 12 * 8 = 146.
    cout << system.startRide(first) << endl;     // 1
    cout << system.completeRide(first) << endl;  // 1
    cout << system.fare(first) << endl;          // 146
    system.printRide(first);                     // completed
    // D1 moves to (10,2) and becomes available.
    cout << system.driverLocation(1).x << " " << system.driverLocation(1).y
         << " " << system.driverAvailable(1) << endl;
    // 10 2 1

    // Dev's request can now be matched: D1 is 2 units from (8,2).
    cout << system.tryMatch(fourth) << endl;  // 1

    // Completing a ride before starting it fails.
    cout << system.completeRide(second) << endl;  // 0

    // Cancel Ben's accepted ride: fee 20, D2 is freed.
    cout << system.cancelRide(second) << endl;  // 20
    system.printRide(second);                   // cancelled
    cout << system.driverAvailable(2) << endl;  // 1

    // A cancelled ride cannot be started or cancelled again.
    cout << system.startRide(second) << " " << system.cancelRide(second)
         << endl;
    // 0 -1

    // Start Chen's ride, then cancellation is no longer allowed.
    cout << system.startRide(third) << endl;   // 1
    cout << system.cancelRide(third) << endl;  // -1

    // Complete it: distance 3 -> max(80, 50 + 36) = 86.
    cout << system.completeRide(third) << endl;  // 1
    cout << system.fare(third) << endl;          // 86
    system.printRide(third);                     // completed

    // Completing twice fails.
    cout << system.completeRide(third) << endl;  // 0

    // A free cancellation before a driver accepts.
    system.addRider(14, "Far");
    int fifth = system.requestRide(14, {100, 100}, {105, 100});
    cout << fifth << " " << system.driverOf(fifth) << endl;  // 5 -1
    cout << system.cancelRide(fifth) << endl;                // 0

    // A new driver near the far rider can be matched.
    system.addDriver(4, "Noor", {95, 95}, 4.7);
    int sixth = system.requestRide(14, {100, 100}, {105, 100});
    cout << sixth << " " << system.driverOf(sixth) << endl;  // 6 4

    // A driver already on a trip is not offered a second ride.
    system.addRider(15, "Gita");
    int seventh = system.requestRide(15, {95, 95}, {96, 95});
    cout << seventh << " " << system.driverOf(seventh) << endl;
    // 7 -1 (Noor is busy; D1, D2, D3 are too far)

    // Finish Dev's ride: distance 3 -> 86. D1 ends at (8,5).
    system.startRide(fourth);
    system.completeRide(fourth);
    cout << system.fare(fourth) << endl;  // 86

    // D1 is now free at (8,5), but that is far from Gita's pickup at (95,95),
    // so the request remains unmatched. Noor is still on ride 6.
    cout << system.driverOf(seventh) << endl;  // -1

    // Finish Noor's ride and retry Gita's request.
    system.startRide(sixth);
    system.completeRide(sixth);
    cout << system.fare(sixth) << endl;        // 50 + 12 * 5 = 110
    cout << system.tryMatch(seventh) << endl;  // 4
    cout << system.driverOf(seventh) << endl;  // 4

    // A driver who is busy cannot be matched to another ride.
    system.addRider(16, "Hari");
    int eighth = system.requestRide(16, {96, 95}, {98, 95});
    cout << system.driverOf(eighth) << endl;  // -1 (Noor is on ride 7)

    system.printDrivers();

    return 0;
}

/**
 * Time complexity:
 *   addDriver / addRider -> O(log n)
 *   requestRide -> O(drivers) for matching
 *   tryMatch -> O(drivers)
 *   startRide / completeRide / cancelRide -> O(log rides + log drivers)
 * Space complexity -> O(drivers + riders + rides)
 *
 * Special cases:
 *   - unknown rider -> request rejected
 *   - pickup equal to drop -> request rejected
 *   - no available driver within the radius -> the ride stays Requested
 *   - a driver farther than the radius is not offered the ride
 *   - equal pickup distance -> higher rating wins, then lower driver id
 *   - a busy driver is never matched
 *   - start before acceptance -> rejected
 *   - complete before start -> rejected
 *   - cancel after start -> rejected
 *   - cancel before acceptance -> free
 *   - cancel after acceptance -> fee 20 and the driver is freed
 *   - complete twice -> rejected
 *   - a completed driver moves to the drop-off point
 *   - a ride can be matched later with tryMatch
 *   - a rider can have more than one active request in this simple model; a
 *     production system usually rejects that
 *   - the minimum fare applies to very short rides
 *
 * Why request creation is separate from matching: demand can exceed supply.
 * Returning an error when every driver is busy loses the request. Keeping it
 * in Requested state lets a dispatcher retry, batch, or offer it to a driver
 * who is about to finish. The client can show "looking for a driver" instead
 * of a hard failure.
 *
 * Why the driver moves to the drop-off: the next pickup distance must be
 * measured from where the driver actually is. Leaving the location at the old
 * spot would systematically under-estimate ETAs and match the wrong driver.
 *
 * Extensions:
 *   - ETA from a road graph instead of Manhattan distance
 *   - surge pricing by demand/supply per zone
 *   - pooled rides with multiple riders
 *   - driver offer timeout and re-dispatch
 *   - upfront fare, tolls, taxes, and tips
 *   - cancellation policy by elapsed time
 *   - ratings for riders and drivers
 */

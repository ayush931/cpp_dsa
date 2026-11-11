/**
 * Parking Lot
 *
 * A multi-floor parking lot assigns a compatible spot to an arriving vehicle
 * when one is free, issues a ticket, and charges for the time spent inside.
 *
 * Core objects:
 *   Vehicle      -> plate number and vehicle type
 *   ParkingSpot  -> floor, spot type, and the plate currently parked there
 *   Ticket       -> plate, assigned spot, and entry hour
 *   ParkingLot   -> owns spots and active tickets
 *
 * Spot compatibility:
 *   Motorcycle -> motorcycle, compact, or large
 *   Car        -> compact or large
 *   Truck      -> large only
 * A smaller vehicle may use a bigger spot, but never the other way around.
 *
 * Allocation: first fit by floor, then by spot id. That keeps cars close to the
 * entrance and makes the demo deterministic. A real lot may prefer the spot
 * closest to an elevator or reserve a floor for a vehicle type.
 *
 * Pricing: a rate per spot type for each whole hour, with a one-hour minimum.
 * The ticket records the entry hour, so the fee is a pure function of
 * (spot type, entry hour, exit hour).
 *
 * Invariants:
 *   - one plate can have at most one active ticket
 *   - an occupied spot always has exactly one active ticket
 *   - unparking removes the ticket and frees the spot atomically
 *
 * Scaling: the linear scan for a free spot is O(spots). A production lot keeps
 * a free list per (floor, spot type) or a min-heap keyed by distance to the
 * entrance, and a database row per ticket for crash recovery.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

enum class VehicleType { Motorcycle, Car, Truck };
enum class SpotType { Motorcycle, Compact, Large };

string vehicleName(VehicleType type) {
    switch (type) {
        case VehicleType::Motorcycle:
            return "motorcycle";
        case VehicleType::Car:
            return "car";
        case VehicleType::Truck:
            return "truck";
    }

    return "unknown";
}

string spotName(SpotType type) {
    switch (type) {
        case SpotType::Motorcycle:
            return "motorcycle";
        case SpotType::Compact:
            return "compact";
        case SpotType::Large:
            return "large";
    }

    return "unknown";
}

bool canFit(VehicleType vehicle, SpotType spot) {
    if (vehicle == VehicleType::Motorcycle) {
        return true;
    }
    if (vehicle == VehicleType::Car) {
        return spot != SpotType::Motorcycle;
    }

    return spot == SpotType::Large;
}

int hourlyRate(SpotType spot) {
    switch (spot) {
        case SpotType::Motorcycle:
            return 2;
        case SpotType::Compact:
            return 5;
        case SpotType::Large:
            return 10;
    }

    return 0;
}

struct Vehicle {
    string plate;
    VehicleType type;
};

struct ParkingSpot {
    int id;
    int floor;
    SpotType type;
    string plate;  // empty when free

    bool isFree() const {
        return plate.empty();
    }
};

struct Ticket {
    string id;
    string plate;
    int spotId;
    int entryHour;
};

class ParkingLot {
    vector<ParkingSpot> spots;
    map<string, Ticket> tickets;        // ticket id -> ticket
    map<string, string> plateToTicket;  // plate -> active ticket id
    int nextTicket = 1;

    ParkingSpot *findFreeSpot(VehicleType type) {
        for (ParkingSpot &spot : spots) {
            if (spot.isFree() && canFit(type, spot.type)) {
                return &spot;
            }
        }

        return nullptr;
    }

    ParkingSpot *findSpot(int id) {
        for (ParkingSpot &spot : spots) {
            if (spot.id == id) {
                return &spot;
            }
        }

        return nullptr;
    }

   public:
    ParkingLot(int floors, int motorcyclePerFloor, int compactPerFloor,
               int largePerFloor) {
        int id = 1;

        for (int floor = 0; floor < floors; floor++) {
            for (int i = 0; i < motorcyclePerFloor; i++) {
                spots.push_back({id++, floor, SpotType::Motorcycle, ""});
            }

            for (int i = 0; i < compactPerFloor; i++) {
                spots.push_back({id++, floor, SpotType::Compact, ""});
            }

            for (int i = 0; i < largePerFloor; i++) {
                spots.push_back({id++, floor, SpotType::Large, ""});
            }
        }
    }

    // Returns the new ticket id, or an empty string when the vehicle cannot
    // park.
    string park(const Vehicle &vehicle, int hour) {
        if (hour < 0) {
            return "";
        }
        if (plateToTicket.count(vehicle.plate)) {
            return "";  // already inside
        }

        ParkingSpot *spot = findFreeSpot(vehicle.type);
        if (spot == nullptr) {
            return "";  // lot full for this vehicle type
        }

        string ticketId = "T" + to_string(nextTicket++);

        spot->plate = vehicle.plate;
        tickets[ticketId] = {ticketId, vehicle.plate, spot->id, hour};
        plateToTicket[vehicle.plate] = ticketId;

        return ticketId;
    }

    // Returns the fee, or -1 for an unknown ticket or an invalid exit hour.
    int unpark(const string &ticketId, int hour) {
        auto entry = tickets.find(ticketId);
        if (entry == tickets.end()) {
            return -1;
        }

        const Ticket ticket = entry->second;

        if (hour < ticket.entryHour) {
            return -1;
        }

        ParkingSpot *spot = findSpot(ticket.spotId);
        if (spot == nullptr) {
            return -1;
        }

        int hours = max(1, hour - ticket.entryHour);
        int fee = hours * hourlyRate(spot->type);

        spot->plate.clear();
        plateToTicket.erase(ticket.plate);
        tickets.erase(entry);

        return fee;
    }

    int available(SpotType type) const {
        int count = 0;

        for (const ParkingSpot &spot : spots) {
            if (spot.isFree() && spot.type == type) {
                count++;
            }
        }

        return count;
    }

    int occupiedCount() const {
        return static_cast<int>(tickets.size());
    }

    void printStatus() const {
        cout << "occupied=" << tickets.size()
             << " free: motorcycle=" << available(SpotType::Motorcycle)
             << " compact=" << available(SpotType::Compact)
             << " large=" << available(SpotType::Large) << endl;
    }

    void printTicket(const string &ticketId) const {
        auto entry = tickets.find(ticketId);

        if (entry == tickets.end()) {
            cout << ticketId << " unknown" << endl;
            return;
        }

        const Ticket &ticket = entry->second;
        const ParkingSpot *spot = nullptr;

        for (const ParkingSpot &candidate : spots) {
            if (candidate.id == ticket.spotId) {
                spot = &candidate;
            }
        }

        cout << ticket.id << " " << ticket.plate << " spot=" << ticket.spotId
             << " floor=" << (spot ? spot->floor : -1)
             << " type=" << (spot ? spotName(spot->type) : "unknown")
             << " entry=" << ticket.entryHour << endl;
    }
};

int main() {
    // Two floors: 1 motorcycle, 2 compact, and 1 large spot per floor.
    ParkingLot lot(2, 1, 2, 1);

    lot.printStatus();  // occupied=0 free: motorcycle=2 compact=4 large=2

    string car1 = lot.park({"CAR-1", VehicleType::Car}, 9);
    string truck1 = lot.park({"TRUCK-1", VehicleType::Truck}, 10);
    string bike1 = lot.park({"BIKE-1", VehicleType::Motorcycle}, 9);

    cout << car1 << " " << truck1 << " " << bike1 << endl;  // T1 T2 T3

    lot.printTicket(car1);    // T1 CAR-1 spot=2 floor=0 type=compact entry=9
    lot.printTicket(truck1);  // T2 TRUCK-1 spot=4 floor=0 type=large entry=10
    lot.printTicket(bike1);  // T3 BIKE-1 spot=1 floor=0 type=motorcycle entry=9

    // A truck cannot use a compact spot, so the second truck takes floor 1.
    string truck2 = lot.park({"TRUCK-2", VehicleType::Truck}, 11);
    lot.printTicket(truck2);  // T4 TRUCK-2 spot=8 floor=1 type=large entry=11

    // The lot is full for trucks, but not for cars.
    cout << lot.park({"TRUCK-3", VehicleType::Truck}, 11) << endl;  // empty

    // A motorcycle takes the first compatible free spot, which is compact spot
    // 3 on floor 0; the next motorcycle takes the motorcycle spot on floor 1.
    string bike2 = lot.park({"BIKE-2", VehicleType::Motorcycle}, 12);
    lot.printTicket(bike2);  // T5 BIKE-2 spot=3 floor=0 type=compact entry=12
    string bike3 = lot.park({"BIKE-3", VehicleType::Motorcycle}, 12);
    lot.printTicket(
        bike3);  // T6 BIKE-3 spot=5 floor=1 type=motorcycle entry=12

    lot.printStatus();  // occupied=6 free: motorcycle=0 compact=2 large=0

    // Same plate cannot park twice.
    cout << lot.park({"CAR-1", VehicleType::Car}, 13) << endl;  // empty

    // Three hours in a compact spot: 3 * 5 = 15.
    cout << lot.unpark(car1, 12) << endl;  // 15
    cout << lot.unpark(car1, 12) << endl;  // -1 (ticket is gone)

    // One hour minimum: entering and leaving in the same hour costs one rate.
    cout << lot.unpark(bike1, 9) << endl;  // 2

    // Exit before entry is invalid.
    cout << lot.unpark(truck1, 9) << endl;  // -1

    // Truck parked for 4 hours: 4 * 10 = 40.
    cout << lot.unpark(truck1, 14) << endl;  // 40

    // A freed spot can be reused.
    string car2 = lot.park({"CAR-2", VehicleType::Car}, 13);
    lot.printTicket(car2);  // T7 CAR-2 spot=2 floor=0 type=compact entry=13

    // Unknown ticket.
    cout << lot.unpark("T999", 20) << endl;  // -1

    // A zero-hour stay still costs the one-hour minimum.
    string bike4 = lot.park({"BIKE-4", VehicleType::Motorcycle}, 20);
    cout << lot.unpark(bike4, 20) << endl;  // 2

    lot.printStatus();  // occupied=4 free: motorcycle=1 compact=2 large=1

    return 0;
}

/**
 * Time complexity:
 *   park -> O(spots) to find the first compatible free spot
 *   unpark -> O(spots) to find the spot, O(log tickets) for the maps
 *   available -> O(spots)
 *   printStatus -> O(spots)
 * Space complexity -> O(spots + active tickets)
 *
 * Special cases:
 *   - the lot is full for a vehicle type -> park returns an empty ticket
 *   - the same plate is already inside -> rejected, so one car cannot hold two
 *     tickets
 *   - a motorcycle may use a compact or large spot, but a truck may not use a
 *     compact spot
 *   - exit hour equal to entry hour -> one-hour minimum
 *   - exit hour before entry hour -> invalid, returns -1
 *   - unknown ticket -> -1
 *   - unparking twice -> the second call returns -1 because the ticket is gone
 *   - negative entry hour -> rejected
 *   - a freed spot is immediately reusable
 *
 * Why first-fit by floor and id: it is deterministic, keeps the demo readable,
 * and packs vehicles into the lowest floor first. If the lot wants to minimize
 * walking distance, store a distance-to-entrance per spot and scan in that
 * order instead.
 *
 * Concurrency: two entry gates could race for the same spot. A production
 * implementation guards allocation with a per-floor mutex or an atomic
 * compare-and-swap on the spot state, and writes the ticket before opening the
 * barrier so a crash cannot leave a free car inside.
 *
 * Extension points:
 *   - pricing strategies: flat, hourly, weekday/weekend, lost-ticket fee
 *   - reservations and monthly passes
 *   - electric charging spots and accessible spots
 *   - license-plate recognition instead of printed tickets
 */

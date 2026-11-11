/**
 * Elevator System
 *
 * A bank of elevators receives floor requests and decides which car serves
 * each one. The interesting parts are the scheduling policy and the per-car
 * movement rule.
 *
 * Per-car state:
 *   floor      -> current floor
 *   direction  -> up, down, or idle
 *   targets    -> floors where the car must stop
 *
 * Movement uses the LOOK algorithm: keep moving in the current direction while
 * a target lies ahead; when none remains, reverse. This avoids the starvation
 * of always returning to the ground floor and is cheaper than a full SCAN to
 * the end of the building.
 *
 * Dispatch policy:
 *   cost = distance from the car to the request
 *        + 2 * number of pending stops
 *
 * The pending-stop term prefers a car that is already free or lightly loaded,
 * even if it is slightly farther away. Ties go to the lower car id so the
 * demo is deterministic.
 *
 * A production dispatcher also considers direction compatibility (a car moving
 * up should not promise a down request far below it), passenger count, door
 * dwell time, and energy use. Assignment can be centralized or each car can
 * bid on requests.
 *
 * The simulation advances in ticks. One tick moves a car one floor, or opens
 * its doors at a target floor. Real systems are event-driven, but the tick
 * model makes the state transitions easy to inspect.
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>
using namespace std;

enum class Direction { Idle, Up, Down };

string directionName(Direction direction) {
    switch (direction) {
        case Direction::Idle:
            return "idle";
        case Direction::Up:
            return "up";
        case Direction::Down:
            return "down";
    }

    return "unknown";
}

class Elevator {
    int id;
    int floor;
    Direction direction = Direction::Idle;
    set<int> targets;

   public:
    Elevator(int id, int startFloor) : id(id), floor(startFloor) {}

    int getId() const {
        return id;
    }

    int getFloor() const {
        return floor;
    }

    bool isIdle() const {
        return targets.empty();
    }

    int pendingStops() const {
        return static_cast<int>(targets.size());
    }

    void request(int target) {
        targets.insert(target);
    }

    // Estimated ticks until this car could reach the requested floor. A car
    // with pending stops pays a penalty because it must serve them first.
    int cost(int requestedFloor) const {
        return abs(floor - requestedFloor) + 2 * pendingStops();
    }

    // One tick: stop at the current floor, or move one floor toward a target.
    void step() {
        if (targets.empty()) {
            direction = Direction::Idle;
            return;
        }

        // Doors open at a target. The car stays on this floor for this tick.
        if (targets.count(floor)) {
            targets.erase(floor);
            if (targets.empty()) {
                direction = Direction::Idle;
            }
            return;
        }

        if (direction == Direction::Idle) {
            auto above = targets.lower_bound(floor);

            if (above == targets.end()) {
                direction = Direction::Down;
            } else if (above == targets.begin()) {
                direction = Direction::Up;
            } else {
                auto below = prev(above);
                int upDistance = *above - floor;
                int downDistance = floor - *below;

                direction = upDistance <= downDistance ? Direction::Up
                                                       : Direction::Down;
            }
        }

        // Reverse only when no target remains ahead in the current direction.
        if (direction == Direction::Up) {
            bool ahead = false;

            for (int target : targets) {
                if (target > floor) {
                    ahead = true;
                }
            }

            if (!ahead) {
                direction = Direction::Down;
            }
        } else if (direction == Direction::Down) {
            bool ahead = false;

            for (int target : targets) {
                if (target < floor) {
                    ahead = true;
                }
            }

            if (!ahead) {
                direction = Direction::Up;
            }
        }

        floor += direction == Direction::Up ? 1 : -1;
    }

    void print() const {
        cout << "E" << id << "@" << floor << " " << directionName(direction)
             << " stops=" << targets.size();
    }
};

class ElevatorSystem {
    vector<Elevator> elevators;
    int maximumFloor;

   public:
    ElevatorSystem(const vector<int> &startFloors, int maximumFloor)
        : maximumFloor(maximumFloor) {
        for (int i = 0; i < static_cast<int>(startFloors.size()); i++) {
            elevators.push_back(Elevator(i, startFloors[i]));
        }
    }

    // Returns the chosen elevator id, or -1 for an invalid floor.
    int request(int floor) {
        if (floor < 0 || floor > maximumFloor) {
            return -1;
        }

        int best = 0;

        for (int i = 1; i < static_cast<int>(elevators.size()); i++) {
            if (elevators[i].cost(floor) < elevators[best].cost(floor)) {
                best = i;
            }
        }

        elevators[best].request(floor);
        return elevators[best].getId();
    }

    void stepAll() {
        for (Elevator &elevator : elevators) {
            elevator.step();
        }
    }

    void printState() const {
        for (int i = 0; i < static_cast<int>(elevators.size()); i++) {
            if (i > 0) {
                cout << " | ";
            }
            elevators[i].print();
        }

        cout << endl;
    }

    bool allIdle() const {
        for (const Elevator &elevator : elevators) {
            if (!elevator.isIdle()) {
                return false;
            }
        }

        return true;
    }
};

int main() {
    // Three cars at floors 0, 5, and 9 in a ten-floor building (0..9).
    ElevatorSystem system({0, 5, 9}, 9);

    system.printState();  // E0@0 idle stops=0 | E1@5 idle stops=0 | E2@9 idle
                          // stops=0

    cout << system.request(6) << endl;  // 1 (closest car, floor 5)
    cout << system.request(1) << endl;  // 0 (closest car, floor 0)
    cout << system.request(9) << endl;  // 2 (already at floor 9)
    cout << system.request(3)
         << endl;  // 1 (one pending stop, cost 4 beats car 0's 5)

    // Invalid floors are rejected and do not change any car.
    cout << system.request(-1) << " " << system.request(10) << endl;  // -1 -1

    system.printState();  // E0@0 idle stops=1 | E1@5 idle stops=2 | E2@9 idle
                          // stops=1

    // Advance the simulation. Each tick moves a car one floor or opens doors.
    for (int tick = 1; tick <= 6; tick++) {
        system.stepAll();
        cout << "t" << tick << " ";
        system.printState();
    }

    // By tick 6 every car has served its stops: E0 at 1, E1 at 3, E2 at 9.
    for (int tick = 7; tick <= 8; tick++) {
        system.stepAll();
        cout << "t" << tick << " ";
        system.printState();
    }

    cout << system.allIdle() << endl;  // 1

    // Drain any remaining requests (none here).
    while (!system.allIdle()) {
        system.stepAll();
    }

    system.printState();               // all cars idle, no pending stops
    cout << system.allIdle() << endl;  // 1

    // A request for the floor below still needs a move tick, then a door-open
    // tick.
    cout << system.request(0) << endl;  // 0
    system.stepAll();                   // move from 1 to 0
    system.stepAll();                   // open doors and clear the target
    system.printState();                // E0@0 idle stops=0

    // Two cars in a small building: the nearest idle car wins.
    ElevatorSystem small({2, 7}, 7);
    cout << small.request(6) << endl;  // 1 (distance 1)
    cout << small.request(0) << endl;  // 0 (distance 2)
    small.stepAll();
    small.printState();  // E0@1 down stops=1 | E1@6 down stops=1

    return 0;
}

/**
 * Time complexity:
 *   request -> O(cars) to compare costs, plus O(log stops) to insert into the
 *   target set
 *   step -> O(stops) for the direction scan
 *   stepAll -> O(cars * stops)
 * Space complexity -> O(cars * stops) for the pending target sets
 *
 * Special cases:
 *   - request below 0 or above the top floor -> rejected, returns -1
 *   - request for the current floor -> the car opens its doors on the next tick
 *   - a car with no targets -> idle and available immediately
 *   - two cars with equal cost -> the lower id wins, so dispatch is stable
 *   - the top or bottom floor -> the direction reverses because no target lies
 *     ahead
 *   - a request added while the car is moving -> inserted into the target set,
 *     so it is served in LOOK order rather than first-come-first-served
 *   - one elevator and many requests -> every request is queued
 *   - a car that is full -> a real system skips pickups; this model only tracks
 *     floors, not capacity
 *
 * Why LOOK instead of nearest-target-only: a car that always chases the single
 * nearest target changes direction constantly and can starve far floors. LOOK
 * sweeps in one direction, serving everything on the way, then reverses.
 *
 * Real dispatch adds direction-aware cost. A car moving up at floor 3 should
 * cost less for a request at floor 8 than a car moving down at floor 3, even
 * though the raw distances are equal, because the second car must finish its
 * down sweep first. The pending-stop penalty in this file approximates that
 * idea without modeling the full sweep.
 */

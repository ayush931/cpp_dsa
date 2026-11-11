/**
 * Snake and Ladder
 *
 * A 100-square board with ladders that move a player up and snakes that move a
 * player down. Players take turns rolling a six-sided die; the first player to
 * land exactly on square 100 wins.
 *
 * Rules used here:
 *   - every player starts at square 0 (before square 1)
 *   - a roll moves the player forward by the die value
 *   - a roll that would pass square 100 is ignored; the player stays put
 *   - after landing, a ladder or snake is applied once
 *   - a jump never chains: landing on the foot of a ladder does not then
 *     trigger a second ladder, even if the destination is another start
 *   - turns rotate in player order; a six does not grant an extra turn
 *
 * The "exact roll" rule matters. Without it, a player at 98 who rolls 4 would
 * either win or bounce, depending on the variant. This implementation ignores
 * the overshoot, which is the most common friendly rule.
 *
 * Board validation:
 *   - a ladder must go up: 1 < start < end <= 100
 *   - a snake must go down: 1 <= end < start < 100
 *   - a square can be the start of only one jump
 *   - square 100 cannot be a jump start because the game ends there
 *
 * The dice is deterministic in this file so the demo is reproducible. A real
 * game uses a random source, and an online game needs a server-side roll with
 * an audit trail so clients cannot tamper with the result.
 */

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

class Dice {
    uint32_t state;

   public:
    explicit Dice(uint32_t seed = 42) : state(seed) {}

    int roll() {
        // A small linear congruential generator: deterministic, portable, and
        // good enough for a board-game demo.
        state = state * 1103515245u + 12345u;
        return static_cast<int>((state >> 16) % 6) + 1;
    }
};

class Board {
    map<int, int> jumps;

   public:
    bool addLadder(int start, int end) {
        if (start <= 1 || start >= end || end > 100) {
            return false;
        }
        if (jumps.count(start)) {
            return false;
        }

        jumps[start] = end;
        return true;
    }

    bool addSnake(int start, int end) {
        if (start <= 1 || end >= start || end < 1 || start >= 100) {
            return false;
        }
        if (jumps.count(start)) {
            return false;
        }

        jumps[start] = end;
        return true;
    }

    // Returns the destination after applying at most one jump.
    int destination(int square) const {
        auto entry = jumps.find(square);
        return entry == jumps.end() ? square : entry->second;
    }

    bool hasJump(int square) const {
        return jumps.count(square) > 0;
    }
};

struct Player {
    string name;
    int position = 0;
};

struct TurnResult {
    int player = -1;
    int roll = 0;
    int from = 0;
    int landed = 0;
    int finalSquare = 0;
    bool jumped = false;
    bool won = false;
};

class SnakeAndLadderGame {
    Board board;
    Dice dice;
    vector<Player> players;
    int current = 0;
    bool finished = false;

   public:
    SnakeAndLadderGame(Board board, Dice dice, vector<string> names)
        : board(std::move(board)), dice(dice) {
        for (const string &name : names) {
            players.push_back({name, 0});
        }
    }

    TurnResult takeTurn() {
        TurnResult result;

        if (finished || players.empty()) {
            return result;
        }

        result.player = current;
        result.from = players[current].position;
        result.roll = dice.roll();

        int target = result.from + result.roll;
        bool overshot = target > 100;

        // An overshoot is ignored; the player needs an exact roll for 100.
        if (overshot) {
            target = result.from;
        }

        result.landed = target;

        int afterJump = overshot ? target : board.destination(target);
        result.jumped = afterJump != target;
        result.finalSquare = afterJump;

        players[current].position = afterJump;

        if (afterJump == 100) {
            result.won = true;
            finished = true;
        }

        current = (current + 1) % players.size();
        return result;
    }

    // Test helper: move a specific player by a fixed roll, bypassing the dice.
    TurnResult advance(int playerIndex, int roll) {
        TurnResult result;

        if (finished || playerIndex < 0 ||
            playerIndex >= static_cast<int>(players.size())) {
            return result;
        }

        result.player = playerIndex;
        result.from = players[playerIndex].position;
        result.roll = roll;

        int target = result.from + roll;
        bool overshot = target > 100;
        if (overshot) {
            target = result.from;
        }

        result.landed = target;

        int afterJump = overshot ? target : board.destination(target);
        result.jumped = afterJump != target;
        result.finalSquare = afterJump;

        players[playerIndex].position = afterJump;

        if (afterJump == 100) {
            result.won = true;
            finished = true;
        }

        return result;
    }

    void play(int maximumTurns = 1000) {
        if (players.empty()) {
            return;
        }

        int turns = 0;

        while (!finished && turns < maximumTurns) {
            TurnResult result = takeTurn();
            turns++;

            cout << players[result.player].name << " roll=" << result.roll
                 << " " << result.from << "->" << result.landed;

            if (result.jumped) {
                cout << " jump->" << result.finalSquare;
            }

            cout << (result.won ? " WIN" : "") << endl;
        }

        if (!finished) {
            cout << "turn limit reached" << endl;
        }
    }

    int position(int playerIndex) const {
        return players[playerIndex].position;
    }

    string playerName(int playerIndex) const {
        return players[playerIndex].name;
    }

    void printResult(const TurnResult &result) const {
        if (result.player < 0) {
            cout << "no turn" << endl;
            return;
        }

        cout << players[result.player].name << " roll=" << result.roll << " "
             << result.from << "->" << result.landed;

        if (result.jumped) {
            cout << " jump->" << result.finalSquare;
        }
        if (result.won) {
            cout << " WIN";
        }

        cout << endl;
    }

    bool isFinished() const {
        return finished;
    }
};

Board standardBoard() {
    Board board;

    vector<pair<int, int>> ladders = {{2, 38},  {7, 14},  {8, 31},  {15, 26},
                                      {21, 42}, {28, 84}, {36, 44}, {51, 67},
                                      {71, 91}, {78, 98}, {87, 94}};

    vector<pair<int, int>> snakes = {{16, 6},  {46, 25}, {49, 11}, {62, 19},
                                     {64, 60}, {74, 53}, {89, 68}, {92, 88},
                                     {95, 75}, {99, 80}};

    for (const auto &[start, end] : ladders) {
        board.addLadder(start, end);
    }
    for (const auto &[start, end] : snakes) {
        board.addSnake(start, end);
    }

    return board;
}

int main() {
    Board board = standardBoard();

    // Ladders go up, snakes go down.
    cout << board.destination(2) << " " << board.destination(16) << endl;
    // 38 6
    cout << board.hasJump(28) << " " << board.hasJump(30) << endl;  // 1 0

    // Invalid jumps are rejected.
    Board invalid;
    cout << invalid.addLadder(50, 40) << endl;   // 0 (ladder must go up)
    cout << invalid.addSnake(40, 50) << endl;    // 0 (snake must go down)
    cout << invalid.addLadder(1, 10) << endl;    // 0 (start must be above 1)
    cout << invalid.addLadder(90, 101) << endl;  // 0 (past the board)
    cout << invalid.addLadder(20, 30) << endl;   // 1
    cout << invalid.addSnake(20, 10) << endl;    // 0 (start already used)

    // Edge cases with two players, tested without the dice.
    SnakeAndLadderGame edge(standardBoard(), Dice(7), {"A", "B"});

    // A snake at 16 sends the player down to 6.
    edge.printResult(edge.advance(0, 16));  // A roll=16 0->16 jump->6
    cout << edge.position(0) << endl;       // 6

    // A ladder at 28 sends the player up to 84.
    edge.printResult(edge.advance(0, 22));  // A roll=22 6->28 jump->84
    cout << edge.position(0) << endl;       // 84

    // From 84, a roll of 6 lands on 90 (no jump).
    edge.printResult(edge.advance(0, 6));  // A roll=6 84->90
    cout << edge.position(0) << endl;      // 90

    // From 90, a roll of 9 is impossible on a die, so use several rolls to show
    // the snake at 99. From 90, a 6 goes to 96, then a 3 lands on 99 and drops
    // to 80.
    edge.advance(0, 6);                    // 90 -> 96
    edge.printResult(edge.advance(0, 3));  // A roll=3 96->99 jump->80
    cout << edge.position(0) << endl;      // 80

    // Walk to 97: 80 -> 86 -> 92 (snake to 88) -> 94 -> 97.
    edge.advance(0, 6);                    // 86
    edge.advance(0, 6);                    // 92 -> 88
    edge.advance(0, 6);                    // 94
    edge.printResult(edge.advance(0, 3));  // A roll=3 94->97
    cout << edge.position(0) << endl;      // 97

    // An overshoot is ignored; the player needs an exact roll.
    edge.printResult(edge.advance(0, 4));  // A roll=4 97->97
    cout << edge.position(0) << endl;      // 97

    // A roll of 3 wins.
    edge.printResult(edge.advance(0, 3));  // A roll=3 97->100 WIN
    cout << edge.position(0) << endl;      // 100

    // No further turns after the game ends.
    cout << edge.advance(0, 1).won << endl;  // 0

    // A full deterministic game with two players.
    SnakeAndLadderGame game(standardBoard(), Dice(42), {"Asha", "Ben"});
    game.play();

    cout << game.position(0) << " " << game.position(1) << endl;

    return 0;
}

/**
 * Time complexity:
 *   roll -> O(1)
 *   takeTurn -> O(log jumps) for the jump lookup, so O(1) for a fixed board
 *   play -> O(turns)
 * Space complexity -> O(players + jumps)
 *
 * Special cases:
 *   - a roll that overshoots 100 -> the player stays on the current square
 *   - landing exactly on 100 -> the player wins and the game stops
 *   - landing on a snake or ladder -> one jump is applied
 *   - a jump destination that is also a jump start -> no chain; the turn ends
 *   - a ladder start of 1 -> rejected; square 1 is the first square, and a
 *     ladder there would be a free move from the start
 *   - a jump start of 100 -> rejected because the game ends there
 *   - duplicate jump starts -> rejected
 *   - a snake that ends below 1 or a ladder above 100 -> rejected
 *   - an empty player list -> takeTurn returns an empty result and play
 *     returns immediately
 *   - a turn limit prevents an infinite game when the dice never cooperates
 *   - no extra turn for rolling a six in this variant
 *
 * Why the overshoot rule exists: if passing 100 won the game, the last few
 * squares would be irrelevant. Requiring an exact roll makes the endgame a
 * small probability puzzle: from 97, only a 3 wins; a 4, 5, or 6 is wasted.
 *
 * Extensions:
 *   - two dice and a choice of which to use
 *   - extra turn on a six, but three sixes in a row loses the turn
 *   - a bounce-back rule for overshoots
 *   - snakes and ladders that chain
 *   - a server-authoritative roll with a cryptographic seed commitment
 */

/**
 * Leaderboard
 *
 * A leaderboard supports:
 *   - addScore(player, score): add points to a player (create the player)
 *   - reset(player): remove a player
 *   - rank(player): 1-based rank, where ties share the same rank
 *   - topK(k): the k highest-scoring players
 *
 * Brute force:
 *   - keep a vector of (player, score)
 *   - update -> O(n) scan
 *   - rank -> O(n) scan for scores greater than the player's
 *   - topK -> O(n log n) sort on every query
 *
 * Optimized:
 *   - hash map player -> score -> O(1) update
 *   - ordered map score -> set of players -> O(log n) insert/remove and
 *     O(k) topK
 *   - Fenwick tree over score buckets -> O(log n) rank
 *
 * This is the same idea as a Redis sorted set (`ZADD`, `ZREVRANK`,
 * `ZREVRANGE`), which uses a skip list plus a hash map.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class LeaderboardBrute {
    vector<pair<string, int>> entries;

    int findIndex(const string &player) {
        for (int i = 0; i < entries.size(); i++) {
            if (entries[i].first == player) {
                return i;
            }
        }

        return -1;
    }

   public:
    void addScore(const string &player, int score) {
        int index = findIndex(player);

        if (index == -1) {
            entries.push_back({player, score});
        } else {
            entries[index].second += score;
        }
    }

    void reset(const string &player) {
        int index = findIndex(player);

        if (index != -1) {
            entries.erase(entries.begin() + index);
        }
    }

    int rank(const string &player) {
        int index = findIndex(player);

        if (index == -1) {
            return -1;
        }

        int better = 0;

        for (auto &entry : entries) {
            if (entry.second > entries[index].second) {
                better++;
            }
        }

        return better + 1;
    }

    vector<string> topK(int k) {
        vector<pair<string, int>> sorted = entries;

        sort(sorted.begin(), sorted.end(),
             [](pair<string, int> &a, pair<string, int> &b) {
                 if (a.second != b.second) {
                     return a.second > b.second;
                 }
                 return a.first <
                        b.first;  // deterministic tie-break for display
             });

        vector<string> result;

        for (int i = 0; i < k && i < sorted.size(); i++) {
            result.push_back(sorted[i].first);
        }

        return result;
    }
};

class FenwickTree {
    vector<int> tree;

   public:
    FenwickTree(int size) {
        tree.assign(size + 1, 0);
    }

    void add(int index, int delta) {
        for (int i = index; i < tree.size(); i += i & -i) {
            tree[i] += delta;
        }
    }

    // Number of players with score <= the given bucket.
    int prefixSum(int index) {
        int sum = 0;

        for (int i = index; i > 0; i -= i & -i) {
            sum += tree[i];
        }

        return sum;
    }
};

class Leaderboard {
    static const int MAX_SCORE = 1000;

    unordered_map<string, int> scoreOf;
    map<int, set<string>> playersAtScore;  // descending iteration for topK
    FenwickTree fenwick;
    int totalPlayers;

    void fenwickAdd(int score, int delta) {
        // Scores are 0..MAX_SCORE, Fenwick indices are 1..MAX_SCORE + 1.
        fenwick.add(score + 1, delta);
    }

   public:
    Leaderboard() : fenwick(MAX_SCORE + 2) {
        totalPlayers = 0;
    }

    void addScore(const string &player, int score) {
        auto found = scoreOf.find(player);
        long long newScore = score;

        if (found != scoreOf.end()) {
            int oldScore = found->second;
            newScore += oldScore;

            playersAtScore[oldScore].erase(player);
            if (playersAtScore[oldScore].empty()) {
                playersAtScore.erase(oldScore);
            }

            fenwickAdd(oldScore, -1);
            totalPlayers--;
        }

        if (newScore < 0) {
            newScore = 0;
        }
        if (newScore > MAX_SCORE) {
            newScore = MAX_SCORE;
        }

        scoreOf[player] = newScore;
        playersAtScore[newScore].insert(player);
        fenwickAdd(newScore, 1);
        totalPlayers++;
    }

    void reset(const string &player) {
        auto found = scoreOf.find(player);

        if (found == scoreOf.end()) {
            return;
        }

        int score = found->second;

        playersAtScore[score].erase(player);
        if (playersAtScore[score].empty()) {
            playersAtScore.erase(score);
        }

        fenwickAdd(score, -1);
        totalPlayers--;
        scoreOf.erase(found);
    }

    int rank(const string &player) {
        auto found = scoreOf.find(player);

        if (found == scoreOf.end()) {
            return -1;
        }

        int score = found->second;

        // Players with a strictly higher score, plus one. Ties share a rank.
        int better = totalPlayers - fenwick.prefixSum(score + 1);

        return better + 1;
    }

    vector<string> topK(int k) {
        vector<string> result;
        if (k <= 0) {
            return result;
        }

        for (auto it = playersAtScore.rbegin();
             it != playersAtScore.rend() && result.size() < k; ++it) {
            for (const string &player : it->second) {
                result.push_back(player);

                if (result.size() == k) {
                    break;
                }
            }
        }

        return result;
    }
};

void print(vector<string> &players) {
    for (string &player : players) {
        cout << player << " ";
    }
    cout << endl;
}

int main() {
    Leaderboard board;

    board.addScore("ayush", 100);
    board.addScore("riya", 80);
    board.addScore("sam", 100);
    board.addScore("neha", 60);

    cout << board.rank("ayush") << endl;  // 1
    cout << board.rank("sam") << endl;    // 1 (tie)
    cout << board.rank("riya") << endl;   // 3
    cout << board.rank("neha") << endl;   // 4

    vector<string> top = board.topK(3);
    print(top);  // ayush sam riya (tie order is lexicographic by set)

    board.addScore("riya", 50);           // 130
    cout << board.rank("riya") << endl;   // 1
    cout << board.rank("ayush") << endl;  // 2

    board.reset("sam");
    cout << board.rank("sam") << endl;    // -1
    cout << board.rank("ayush") << endl;  // 1

    vector<string> afterReset = board.topK(2);
    print(afterReset);  // riya ayush

    LeaderboardBrute brute;
    brute.addScore("ayush", 100);
    brute.addScore("riya", 80);
    brute.addScore("sam", 100);

    cout << brute.rank("riya") << endl;  // 3
    vector<string> bruteTop = brute.topK(2);
    print(bruteTop);  // ayush sam

    return 0;
}

/**
 * Time complexity:
 *   brute -> O(n) update/rank, O(n log n) topK
 *   optimized -> O(log n) update/reset/rank, O(k) topK
 * Space complexity -> O(n + MAX_SCORE) for the maps and the Fenwick tree
 *
 * Special cases: a missing player (rank -1), ties at the same score, a player
 * updated many times, a score of 0, a score above MAX_SCORE (clamped here),
 * topK larger than the number of players, resetting a missing player, and
 * negative scores (clamped here).
 *
 * For an unbounded score range, coordinate-compress the scores first or use a
 * balanced order-statistic tree. A real service shards the leaderboard by
 * region or game mode, then merges the top lists.
 */

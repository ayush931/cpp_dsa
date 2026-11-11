/**
 * Accounts Merge: rows are [name,email,...]. Shared email means same person;
 * matching names alone do not. Assume accounts connected by email have the
 * same name. Union account indices through an email->first owner map, then
 * collect unique sorted emails per component. Keep name-only accounts.
 * Expected O(E*alpha(A) + E log E) time, O(A+E) space (excluding string costs),
 * for A accounts, E email occurrences. Prerequisite: DSU with path compression.
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class DisjointSet {
    vector<int> parent, size;

   public:
    explicit DisjointSet(int n) : parent(n), size(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return;
        }
        if (size[a] < size[b]) {
            swap(a, b);
        }
        parent[b] = a;
        size[a] += size[b];
    }
};

vector<vector<string>> mergeAccounts(const vector<vector<string>> &accounts) {
    int n = static_cast<int>(accounts.size());
    DisjointSet groups(n);
    unordered_map<string, int> owner;
    for (int i = 0; i < n; ++i) {
        if (accounts[i].empty()) {
            throw invalid_argument("account needs a name");
        }
        for (size_t j = 1; j < accounts[i].size(); ++j) {
            auto [position, inserted] = owner.emplace(accounts[i][j], i);
            if (!inserted) {
                groups.unite(i, position->second);
            }
        }
    }
    vector<vector<string>> emails(n), result;
    for (const auto &[email, index] : owner) {
        emails[groups.find(index)].push_back(email);
    }
    for (int i = 0; i < n; ++i) {
        if (groups.find(i) != i) {
            continue;
        }
        auto &list = emails[i];
        sort(list.begin(), list.end());
        list.insert(list.begin(), accounts[i][0]);
        result.push_back(list);
    }
    sort(result.begin(), result.end());  // deterministic presentation
    return result;
}

int main() {
    auto result = mergeAccounts({{"John", "a@mail", "b@mail"},
                                 {"John", "b@mail", "c@mail"},
                                 {"John", "different@mail"},
                                 {"Mary"}});
    assert(result.size() == 3);
    assert((result[0] == vector<string>{"John", "a@mail", "b@mail", "c@mail"}));
    assert((result[2] == vector<string>{"Mary"}));
    assert(mergeAccounts({}).empty());
    for (const auto &account : result) {
        for (const auto &entry : account) {
            cout << entry << ' ';
        }
        cout << '\n';
    }
}

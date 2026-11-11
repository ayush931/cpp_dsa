/**
 * Leader / parent of the group:- To uniquely identify a group, we will pick any
 * element from the group & make it representative / leader / parent of the
 * group
 *
 * 1. Union(a, b):- adds the group where element b belongs to the group where
 * the elements a belongs or vice-versa.
 * 2. find(x) / get(x):- this will be used to find which group x belongs to, we
 * will return the parent of the group that x belongs to
 * 3. log*n:- this represents that if you have the value of n, and you
 * repeatedly apply log2n on this value then in how many operations you can
 * reduce it to <= 1
 */

// DSU (disjoint set union) data structure

#include <iostream>
#include <vector>
using namespace std;

int find(vector<int> &parent, int x) {
    // this method returns which group/cluster x belongs to
    return parent[x] = (parent[x] == x) ? x : find(parent, parent[x]);
}

void Union(vector<int> &parent, vector<int> &rank, int a, int b) {
    a = find(parent, a);
    b = find(parent, b);

    if (a == b) {
        return;  // already in the same set: nothing to do
    }

    // Union by rank: attach the shallower tree under the deeper one.
    if (rank[a] < rank[b]) {
        int temporary = a;
        a = b;
        b = temporary;
    }

    parent[b] = a;

    if (rank[a] == rank[b]) {
        rank[a]++;
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    // n -> elements, m -> number of queries

    vector<int> parent(n + 1);
    vector<int> rank(n + 1, 0);

    for (int i = 0; i <= n; i++) {
        parent[i] = i;
    }

    while (m--) {
        string str;
        cin >> str;
        if (str == "union") {
            int x, y;
            cin >> x >> y;
            Union(parent, rank, x, y);
        } else {
            int x;
            cin >> x;
            cout << find(parent, x) << "\n";
        }
    }
    return 0;
}
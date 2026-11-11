// cycle detection using bfs

#include <bits/stdc++.h>

#include <unordered_set>
#include <vector>
using namespace std;

vector<list<int>> graph;
int v;  // no of vertices

void add_edge(int src, int dist, bool bi_dir = true) {
    graph[src].push_back(dist);
    if (bi_dir) {
        graph[dist].push_back(src);
    }
};

void display() {
    for (int i = 0; i < graph.size(); i++) {
        cout << i << "->";
        for (auto el : graph[i]) {
            cout << el << " , ";
        }
        cout << "\n";
    }
};

bool bfs(int src) {
    unordered_set<int> visited;
    queue<int> qu;
    vector<int> par(v, -1);
    qu.push(src);
    visited.insert(src);

    while (not qu.empty()) {
        int curr = qu.front();
        qu.pop();

        for (auto neighbour : graph[curr]) {
            if (visited.count(neighbour) and par[curr] != neighbour) {
                return true;
            }
            if (!visited.count(neighbour)) {
                visited.insert(neighbour);
                par[neighbour] = curr;
                qu.push(neighbour);
            }
        }
    }

    return false;
};

bool has_cycle() {
    // bfs() starts a fresh search for every component, so disconnected graphs
    // are handled as well.
    for (int i = 0; i < v; i++) {
        if (bfs(i)) {
            return true;
        }
    }
    return false;
};

int main() {
    cin >> v;
    graph.resize(v, list<int>());

    int e;
    cin >> e;

    while (e--) {
        int s, d;
        cin >> s >> d;
        add_edge(s, d, false);
    }

    display();
    bool b = has_cycle();
    cout << b << "\n";

    return 0;
}

// Time complexity -> O(b + a)

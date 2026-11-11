// cycle detection using dfs

#include <bits/stdc++.h>
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

bool dfs(int src, int parent, unordered_set<int> &visited) {
    visited.insert(src);
    for (auto neighbour : graph[src]) {
        if (visited.count(neighbour) and neighbour != parent) {
            // cycle detected
            return true;
        }
        if (!visited.count(neighbour)) {
            bool res = dfs(neighbour, src, visited);
            if (res == true) {
                return true;
            }
        }
    }
    return false;
};

bool has_cycle() {
    unordered_set<int> visited;
    bool result = false;
    for (int i = 0; i < v; i++) {
        if (!visited.count(i)) {
            result = dfs(i, -1, visited);
            if (result == true) {
                return true;
            }
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
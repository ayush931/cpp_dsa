/**
 * Skip list -> probabilistic alternative to a balanced binary search tree
 * Every node has a tower of forward pointers
 * A node is promoted to the next level with probability 1/2
 * Search, insert and delete -> O(log n) average, O(n) worst
 * Simpler to implement than a balanced tree
 */

#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

const int MAX_LEVEL = 16;

class Node {
   public:
    int value;
    vector<Node *> forward;

    Node(int v, int level) {
        value = v;
        forward.resize(level, NULL);
    }
};

class SkipList {
    Node *head;
    int level;

    int randomLevel() {
        int currentLevel = 1;

        while (rand() % 2 == 0 && currentLevel < MAX_LEVEL) {
            currentLevel++;
        }

        return currentLevel;
    }

   public:
    SkipList() {
        head = new Node(-1, MAX_LEVEL);
        level = 1;
    }

    bool search(int target) {
        Node *current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->forward[i] != NULL &&
                   current->forward[i]->value < target) {
                current = current->forward[i];
            }
        }

        current = current->forward[0];
        return current != NULL && current->value == target;
    }

    void insert(int value) {
        vector<Node *> update(MAX_LEVEL, NULL);
        Node *current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->forward[i] != NULL &&
                   current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current == NULL || current->value != value) {
            int newLevel = randomLevel();

            if (newLevel > level) {
                for (int i = level; i < newLevel; i++) {
                    update[i] = head;
                }
                level = newLevel;
            }

            Node *newNode = new Node(value, newLevel);

            for (int i = 0; i < newLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }

    void erase(int value) {
        vector<Node *> update(MAX_LEVEL, NULL);
        Node *current = head;

        for (int i = level - 1; i >= 0; i--) {
            while (current->forward[i] != NULL &&
                   current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current != NULL && current->value == value) {
            for (int i = 0; i < level; i++) {
                if (update[i]->forward[i] != current) {
                    break;
                }
                update[i]->forward[i] = current->forward[i];
            }

            delete current;

            while (level > 1 && head->forward[level - 1] == NULL) {
                level--;
            }
        }
    }
};

int main() {
    SkipList list;

    list.insert(3);
    list.insert(6);
    list.insert(7);
    list.insert(9);
    list.insert(12);

    cout << list.search(6) << endl;  // 1
    cout << list.search(8) << endl;  // 0

    list.erase(6);
    cout << list.search(6) << endl;  // 0

    return 0;
}

// Special cases: duplicate insertion, deleting a missing value,
// empty list, level overflow, worst case without balancing

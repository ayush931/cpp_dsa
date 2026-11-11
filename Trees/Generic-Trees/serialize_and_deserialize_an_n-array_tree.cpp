/**
 * Serialize and Deserialize an N-array tree.
 * Serialization is the process of converting an object into a format that can
 * be stored or transmitted. Deserialization is the process of converting the
 * serialized string back into an object. Serialization is to store tree in a
 * file so that it can be later restored. The structure of tree must be
 * maintained. Deserialization is reading tree back from file.
 */

#include <bits/stdc++.h>
using namespace std;

class Node {
   public:
    int data;
    vector<Node *> children;

    Node(int data) {
        this->data = data;
    }
};

void levelOrderTraversal(Node *root) {
    if (!root) {
        return;
    }

    queue<Node *> q;
    q.push(root);

    while (!q.empty()) {
        int size = q.size();
        while (size--) {
            Node *curr = q.front();
            q.pop();

            cout << curr->data << " ";

            for (auto child : curr->children) {
                q.push(child);
            }
        }
        cout << endl;
    }
}

void serialiseTree(Node *root, string &ans) {
    if (!root) {
        return;
    }
    ans += to_string(root->data) + ":" + to_string(root->children.size()) + ":";

    for (auto child : root->children) {
        ans += to_string(child->data) + ",";
    }

    if (root->children.size()) {
        ans.pop_back();
    }

    ans += "\n";
    for (auto child : root->children) {
        serialiseTree(child, ans);
    }
}

Node *deserialiseTreeHelper(int nodeValue, unordered_map<int, string> &map) {
    Node *node = new Node(nodeValue);
    string nodeStr = map[nodeValue];  // "2:30,40"
    int breakPos = nodeStr.find(':');
    int childCount = stoi(nodeStr.substr(0, breakPos));
    if (childCount == 0) {
        return node;
    }

    string childStr = nodeStr.substr(breakPos + 1);
    int start = 0;

    for (int i = 0; i < childCount; i++) {
        int end = childStr.find(',', start);
        if (end == string::npos) {
            end = childStr.size();
        }
        int childValue = stoi(childStr.substr(start, end - start));
        node->children.push_back(deserialiseTreeHelper(childValue, map));
        start = end + 1;
    }
    return node;
}

Node *deserialiseTree(string str) {
    if (str == "") {
        return NULL;
    }

    unordered_map<int, string> map;
    int start = 0;

    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '\n') {
            string nodeStr = str.substr(start, i - start);
            int breakPos = nodeStr.find(':');
            int nodeValue = stoi(nodeStr.substr(0, breakPos));
            map[nodeValue] = nodeStr.substr(breakPos + 1);
            start = i + 1;
        }
    }

    int rootValue = stoi(str.substr(0, str.find(':')));
    return deserialiseTreeHelper(rootValue, map);
}

int main() {
    Node *root = new Node(10);
    root->children.push_back(new Node(2));
    root->children.push_back(new Node(34));
    root->children.push_back(new Node(56));
    root->children.push_back(new Node(100));
    root->children[1]->children.push_back(new Node(1));
    root->children[3]->children.push_back(new Node(7));
    root->children[3]->children.push_back(new Node(8));
    root->children[3]->children.push_back(new Node(9));

    cout << "Original Tree\n";
    levelOrderTraversal(root);

    string serialised = "";

    serialiseTree(root, serialised);

    cout << "\nSerialized Tree\n";
    cout << serialised << endl;

    Node *newRoot = deserialiseTree(serialised);

    cout << "Deserialized Tree\n";
    levelOrderTraversal(newRoot);

    return 0;
}
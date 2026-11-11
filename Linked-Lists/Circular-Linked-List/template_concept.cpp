#include <iostream>
using namespace std;

template <typename T>

class Node {  // template class
   public:
    T val;
    Node *next;

    Node(T data) {
        val = data;
        next = NULL;
    }
};

int main() {
    Node<int> *node1 = new Node<int>(3);
    cout << node1->val << endl;

    Node<char> *node2 = new Node<char>('a');
    cout << node2->val << endl;

    delete node1;
    delete node2;

    return 0;
}

#include <iostream>

using namespace std;

// A linked list node
class Node {
public:
    int data;
    Node* next;

    // Constructor to initialize a new node with data
    Node(int new_data) {
        this->data = new_data;
        this->next = nullptr;
    }
};

// Function to traverse and print the singly linked list
void traverseList(const Node* head) {

    // Base condition is when the head is nullptr
    if (head == nullptr) {
        cout << endl;
        return;
    }

    // Printing the current node data
    cout << head->data << " ";

    // Moving to the next node
    traverseList(head->next);
}

// Driver code
int main() {
  
    // Create a hard-coded linked list:
    // 10 -> 20 -> 30 -> 40
    Node* head = new Node(10);
    head->next = new Node(20);
    head->next->next = new Node(30);
    head->next->next->next = new Node(40);

    // Example of traversing the node and printing
    traverseList(head);

    return 0;
}
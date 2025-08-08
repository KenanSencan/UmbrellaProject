// C++ Implementation to insert a node before
// a given key using iteration
#include <bits/stdc++.h>
using namespace std;

class Node
{
  public:
    int data;
    Node* next;

    Node(int x)
    {
        data = x;
        next = nullptr;
    }
};

// Iterative function to insert a new node with value
// newData before the node with the given key
Node* insertBeforeKey(Node* head, int key, int newData)
{

    // If the list is empty
    if (head == nullptr)
    {
        return nullptr;
    }

    // Special case: if the key is at the head
    if (head->data == key)
    {
        Node* new_node = new Node(newData);
        new_node->next = head;
        return new_node;
    }

    // Initialize current and previous pointers
    Node* curr = head;
    Node* prev = nullptr;

    // Traverse the list to find the key
    while (curr != nullptr && curr->data != key)
    {
        prev = curr;
        curr = curr->next;
    }

    // If the key was found
    if (curr != nullptr)
    {
        Node* new_node = new Node(newData);
        prev->next = new_node;
        new_node->next = curr;
    }

    return head;
}

void printList(Node* node)
{
    Node* curr = node;
    while (curr != nullptr)
    {
        cout << curr->data << " ";
        curr = curr->next;
    }
    cout << endl;
}

int main()
{
    // Create a hard-coded linked list:
    // 1 -> 2 -> 3 -> 4 -> 5
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(3);
    head->next->next->next = new Node(4);
    head->next->next->next->next = new Node(5);

    int newData = 6;
    int key = 2;

    head = insertBeforeKey(head, key, newData);

    printList(head);

    return 0;
}
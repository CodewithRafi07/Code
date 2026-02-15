// Create three nodes and traverse using a doubly linked list

#include<iostream>
using namespace std;

struct node
{
    int data;
    node* next;
    node *prev;
};

int main() {
    node* head = new node();
    head -> data = 10;
    head -> next = new node();
    head -> next -> data = 20;
    head -> next -> prev = head;
    head -> next -> next = new node();
    head -> next -> next -> data = 30;
    head -> next -> next -> prev = head -> next;

    head -> next -> next -> next = nullptr;
    head -> prev = nullptr;

    cout << "Forward Traversal: ";
    node* temp = head;
    while (temp != nullptr) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << endl;

    cout << "Backward Traversal: ";
    temp = head -> next -> next;
    while (temp != nullptr) {
        cout << temp->data << " ";
        temp = temp->prev;
    }
    cout << endl; 

    return 0;

}

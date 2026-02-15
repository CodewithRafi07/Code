//Insert a new node at a specific posuition in a singly linked list

#include<iostream>
using namespace std;

struct node {
    int data;
    node* next;
};

void insertnewnode(node*& head, int value, int pos) {
    node* nn = new node();
    nn->data = value;
    node* prev = nullptr;
    node* temp = head;

    for (int i = 0; i <= pos; i++) {
        if (temp == nullptr) {
            cout << "Position out of bounds." << endl;
            delete nn; // Clean up allocated memory
            return;
        }

        prev = temp;
        temp = temp->next;
    }
    nn->next = prev->next;
    prev->next = nn;
}

void print(node* head) {
    node* temp = head;
    while (temp != nullptr) {
        cout << temp->data << " ";
        temp = temp->next;
    }
    cout << "NULL" << endl;
}

int main() {
    node *head = new node();
    head -> data = 10;
    head -> next = new node();
    head -> next -> data = 20;
    head -> next -> next = new node();
    head -> next -> next -> data = 30;
    head -> next -> next -> next = new node();
    head -> next -> next -> next -> data = 40;
    head -> next -> next -> next -> next = nullptr;

    cout << "Before inserting new node: ";
    print(head);

    int value, pos;
    cout << "Enter value to insert: ";
    cin >> value;
    cout << "Enter position to insert at: ";
    cin >> pos;

    insertnewnode(head, value, pos);

    cout << "After inserting new node: ";
    print(head);

    return 0;
}
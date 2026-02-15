// Delete Last Node

#include<iostream>
using namespace std;

struct node
{
    int data;
    node* next;
};

void display(node* head) {
    cout << "Linked list: ";
    while (head != nullptr)
    {
        cout << head -> data << " -> ";
        head = head -> next;
    }

    cout << "NULL" << endl;
}

void deleteLastNode(node*& head) {
    if (head == nullptr)
    {
        cout << "List is Empty";
        return;
    }

    if (head->next == nullptr)
    {
        delete head;
        head = nullptr;
        return;
    }

    // Traverse to the second last node
    node* temp;
    temp = head;
    while (temp->next->next != nullptr) {
        temp = temp->next;
    }

    // temp is now the second last node
    delete temp->next;
    temp->next = nullptr;
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

    cout << "Before delating last node: "; 
    display(head);

    deleteLastNode(head);
    cout << "After deleting last node: ";

    display(head);
    return 0;

}


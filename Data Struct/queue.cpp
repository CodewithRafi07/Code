#include<iostream>
using namespace std;
#define size 4

class Queue {
private: 
    int arr[size];
    int front, rear;

public:
    Queue() {
        front = -1;
        rear = -1;
    }

    void enqueue(int x) {
        if (rear == size - 1) {
            cout << "Overflow!" << endl;
        }
        else if (front == -1 && rear == -1) {
            front ++;
            rear ++;
            arr[rear] = x;
        }
        else {
            rear++;
            arr[rear] = x;
        }
    }

    void dequeue() {
        if (front == -1 && rear == -1) {
            cout << "Empty!" << endl;
        }
        else if (front == rear) {
            cout << "Deleted: " << arr[front] << endl;
            front = -1;
            rear = -1;
        }
        else {
            cout << "Deleted: " << arr[front] << endl;
            front++;
        }
    }

    void display() {
        if (front == -1 && rear == -1) {
            cout << "Queue is Empty!" << endl;
        }
        else {
            cout << "Queue: ";
            for (int i = front; i <= rear; i++) {
                cout << arr[i] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    Queue q;

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);
    q.display();

    q.enqueue(50);   // Overflow হবে
    q.display();

    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.display();     // Empty দেখাবে

    q.enqueue(60);   // আবার ঢোকানো যাবে
    q.display();

    return 0;
}

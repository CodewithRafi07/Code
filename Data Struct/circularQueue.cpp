#include <iostream>
using namespace std;
#define size 4

class CircularQueue {
private:
    int arr[size];
    int front, rear;

public:
    CircularQueue() {
        front = -1;
        rear = -1;
    }

    void enqueue(int x) {
        // Full condition: (rear+1)%size == front
        if ((rear + 1) % size == front) {
            cout << "Queue Overflow!" << endl;
            return;
        }
        else if (front == -1 && rear == -1) { // Empty
            front = (rear + 1)%size;
            rear = (rear + 1)%size;
            arr[rear] = x;
        }
        else {
            rear = (rear + 1) % size;
            arr[rear] = x;
        }
    }

    void dequeue() {
        // Empty condition
        if (front == -1 && rear == -1) {
            cout << "Queue Underflow!" << endl;
            return;
        }
        else if (front == rear) { // only one element
            cout << "Deleted: " << arr[front] << endl;
            front = -1;
            rear = -1;
        }
        else {
            cout << "Deleted: " << arr[front] << endl;
            front = (front + 1) % size; // circular increment
        }
    }

    void display() {
        if (front == -1 && rear == -1) {
            cout << "Queue is Empty!" << endl;
            return;
        }

        cout << "Queue: ";
        int i = front;
        while (true) {
            cout << arr[i] << " ";
            if (i == rear) break;
            i = (i + 1) % size; // circular increment
        }
        cout << endl;
    }
};

int main() {
    CircularQueue q;

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);   // Overflow হবে
    q.display();

    q.dequeue();
    q.enqueue(50);
    q.display();

    q.dequeue();
    q.dequeue();
    q.enqueue(60);
    q.enqueue(70);
    q.display();

    return 0;
}

#include<iostream>
using namespace std;
#define size 4

class queue{
    private: 
    int arr[size];
    int front, rear;

    public:
    queue() {
        front = -1;
        rear = -1;
    }

void enqueue(int x) {
    if (rear == size - 1){
        cout << "OverFlow";
    }
    else if (front == -1 && rear == -1) {
        front ++;
        rear ++;
        arr[rear] = x;
    }
    else{
        rear++;
        arr[rear] = x;
    }
}

void dequeue() {
    if (front == -1 && rear == -1){
        cout << "Empty" << endl;
    }
    else if (front == rear){
        front = -1;
        rear = -1;
    }
    else {
        int x = arr[front];
        cout << "Deleted" << x << endl;
        front ++;
    }
}

void display() {
    if (front == -1 && rear == -1) {
        cout << "Empty. GO BC" << endl;
    }

    else{
        for (int i = front; i <= rear; i++){
            cout << arr[i] << " " << endl;
        }
    }
    cout << endl;
}

};

int main() {
    queue q;

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    q.enqueue(40);
    q.display();
    q.enqueue(50);
    q.display();

    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.dequeue();
    q.display();
    q.enqueue(60);
    q.display();
    return 0;
}
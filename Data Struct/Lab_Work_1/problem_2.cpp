//Take an array of 10 elements and print the largest and smallest element.

#include <iostream>
using namespace std;

int main() {
    int arr[10], largest, smallest;

    cout << "Enter 10 elements:\n";
    for (int i = 0; i < 10; i++) {
        cin >> arr[i];
    }

    largest = smallest = arr[0];

    for (int i = 1; i < 10; i++) {
        if (arr[i] > largest) {
            largest = arr[i];
        }
        if (arr[i] < smallest) {
            smallest = arr[i];
        }
    }

    cout << "Largest element: " << largest << endl;
    cout << "Smallest element: " << smallest << endl;

    return 0;
}
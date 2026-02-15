//Problem 1: Take an array of 10 elements and add 5 with each element.
//Print the array before and after adding.

#include <iostream>
using namespace std;

int main() {
    int arr[10];

    cout << "Enter 10 elements:\n";
    for (int i = 0; i < 10; i++) {
        cin >> arr[i];
    }

    cout << "Original array: ";
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }

    for (int i = 0; i < 10; i++) {
        arr[i] += 5;
    }

    cout << "\nArray after adding 5: ";
    for (int i = 0; i < 10; i++) {
        cout << arr[i] << " ";
    }

    cout << endl;
    return 0;
}
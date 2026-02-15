#include<iostream>
using namespace std;

int binarysrc(int arr[], int size, int target) {
    int start = 0;
    int end = size - 1;
    
    while (start <= end) {
        int mid = (start + end)/2;

        if (arr[mid] == target) 
        return mid;

        else if (arr[mid] < target)
        start = mid + 1;

        else 
        end = mid - 1;
    }
    return -1;
}

int main() {
    int arr[10] = {11,21,32,43,52,59,62,65,68,73};
    int target;

    cout << "Enter Number to search: ";
    cin >> target;

    int result = binarysrc(arr, 10, target);

    if (result != -1)
    cout << "Number found at index: " << result << endl;

    else 
     cout << "Number not found at index." << endl;

    return 0;

}

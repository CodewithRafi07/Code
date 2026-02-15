/*Given NN numbers, find the one that is of the highest value and print it.
Input The first line of the input will contain NN (0<N<1000<N<100).
The following line will contain NN integers, each between 1 and 1000.
Output Print the maximum.*/

#include <iostream>
using namespace std;

int main() {
    int N;
    cin >> N;  // Number of integers
    
    int maxVal = -1;  // Since numbers are between 1 and 1000
    for (int i = 0; i < N; i++) {
        int x;
        cin >> x;
        if (x > maxVal) {
            maxVal = x;
        }
    }
    
    cout << maxVal << endl;  // Print maximum
    return 0;
}

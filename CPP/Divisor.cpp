#include<iostream>
using namespace std;

int main() {
    int a;

    //cout << "Er: ";
    cin >> a;
    for (int i = 1; i <= a; i++)
    {
        if (a%i==0)
        {
            cout << "The divisor is " << i <<endl;
        }
    }
    return 0;
}
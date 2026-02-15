#include <iostream>
using namespace std;

int main() {
    int x, m, n;
    cin >> x >> m >> n;

    int sum = m + n;

    if (sum == 0){
        cout << "Invalid Ratio" ;
        return 0;
    }
    int a = x * m / sum;
    int b = x * n / sum;

    cout << a << " " << b << endl;

    return 0;
}

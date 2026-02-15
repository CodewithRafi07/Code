#include <iostream>
using namespace std;

class Number {
private:
    int value;
public:
    Number(int v) { value = v; }

    bool isPrime() {
        if (value < 2) return false;
        for (int i = 2; i * i <= value; i++) {
            if (value % i == 0) return false;
        }
        return true;
    }

    int reverseNumber() {
        int n = value, rev = 0;
        while (n > 0) {
            rev = rev * 10 + (n % 10);
            n /= 10;
        }
        return rev;
    }

    int sumOfDigits() {
        int n = value, sum = 0;
        while (n > 0) {
            sum += (n % 10);
            n /= 10;
        }
        return sum;
    }

    void display() {
        cout << "Number: " << value << endl;
        cout << "Prime: " << (isPrime() ? "Yes" : "No") << endl;
        cout << "Reversed: " << reverseNumber() << endl;
        cout << "Sum of Digits: " << sumOfDigits() << endl;
        cout << "--------------------------" << endl;
    }
};

int main() {
    Number n1(17), n2(123), n3(44);
    n1.display();
    n2.display();
    n3.display();
    return 0;
}

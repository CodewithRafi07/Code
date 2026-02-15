#include <iostream>
#include <iomanip>
#include <ctime>   // for getting current year
using namespace std;

class Car {
private:
    string brand;
    string model;
    int year;
    float price;

public:
    // Constructor
    Car(string b, string m, int y, float p) {
        brand = b;
        model = m;
        year = y;
        price = p;
    }

    // Method to display car info
    void display() {
        cout << "Brand : " << brand << endl;
        cout << "Model : " << model << endl;
        cout << "Year  : " << year << endl;
        cout << "Price : $" << price << endl;
        cout << "---------------------------------" << endl;
    }

       // Method to apply depreciation
    void applyDepreciation(float discount) {
            price = price - price * (discount / 100);
    }

        // Method to check antique (older than 25 years)
    bool isAntique() {
        time_t t = time(0);          // current time
        tm* now = localtime(&t);     // convert to tm structure
        int currentYear = now->tm_year + 1900;
        return (currentYear - year) > 25;
    }
};


int main() {
    // Create 3 car objects
    Car c1("Toyota", "Corolla", 1995, 15000.00);
    Car c2("Tesla", "Model S", 2020, 79999.99);
    Car c3("Ford", "Mustang", 1967, 30000.00);

    cout << "Initial Car Details:\n";
    c1.display();
    c2.display();
    c3.display();

    cout << "\nApplying Depreciation...\n";
    c1.applyDepreciation(10); // 10% depreciation
    c2.applyDepreciation(5);  // 5% depreciation
    c3.applyDepreciation(20); // 20% depreciation

    cout << "\nUpdated Car Details:\n";
    c1.display();
    c2.display();
    c3.display();

    cout << "\nAntique Check:\n";
    cout << "Car 1 Antique? " << (c1.isAntique() ? "Yes" : "No") << endl;
    cout << "Car 2 Antique? " << (c2.isAntique() ? "Yes" : "No") << endl;
    cout << "Car 3 Antique? " << (c3.isAntique() ? "Yes" : "No") << endl;

    return 0;
}
#include <iostream>
#include <iomanip>
using namespace std;

class Book {
private:
    string title;
    string author;
    int pages;
    float price;

public:
    // Constructor
    Book(string t, string a, int p, float pr) {
        title = t;
        author = a;
        pages = p;
        price = pr;
    }

    // Method to display book information
    void display() {
        //cout << fixed << setprecision(2); // for price formatting
        
        cout << "Title : " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Pages : " << pages << endl;
        cout << "Price : $" << price << endl;
        cout << "-----------------------------" << endl;
    }

    // Method to apply discount
    /*void applyDiscount(float percent) {
        if (percent > 0 && percent <= 100) {
            price -= (price * percent / 100);
        } else {
            cout << "Invalid discount percentage!" << endl;
        }
    }*/

     void applyDiscount(float discount) {
       price = price - price * (discount / 100); 
   }

};

// Main function
int main() {
    // Creating three book objects
    Book b1("C++ Programming", "Bjarne Stroustrup", 1200, 59.99);
    Book b2("Clean Code", "Robert C. Martin", 464, 39.99);
    Book b3("The Pragmatic Programmer", "Andrew Hunt", 352, 45.50);

    cout << "Initial Book Details:\n";
    b1.display();
    b2.display();
    b3.display();

    // Applying discounts
    cout << "\nApplying Discounts...\n";
    b1.applyDiscount(10); // 10% discount
    b2.applyDiscount(15); // 15% discount
    b3.applyDiscount(5);  // 5% discount

    cout << "\nUpdated Book Details:\n";
    b1.display();
    b2.display();
    b3.display();

    return 0;
}

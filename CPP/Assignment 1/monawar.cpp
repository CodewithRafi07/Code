//Problem 3: Car Class

#include<bits/stdc++.h>
using namespace std;

class Car
{
    private:
        string brand;
        string model;
        int year;
        float price;
        float regular_price;

    public:
        void input()
        {
            cout << "Enter the Car name: ";
            cin >> brand;
            
            cout << "Enter the Car model: ";
            cin >> model;
            
            cout << "Enter the car  Year: ";
            cin >> year;
            
            cout << "Enter the Car price: ";
            cin >> price;

        }

   
        void displayinfo(int current_year)
    {
        cout << "\n-Car Information-" << "\nBrand: " << brand << "\nModel: " << model << "\nYear: " << year << endl;
        

        if (regular_price > 0)
        {
            cout << "Regular Price: " << regular_price << endl;
            cout << "Discounted Price: " << price << endl;
        }
        else
        {
            cout << "Price: " << price << endl;
        }

            if (current_year - year > 25)
            {
                cout << "This is an antique." << endl;
            }
            else
            {
                cout << "This isn't an antique." << endl;
            }
    }

    void discount(float parcent)
    {
        regular_price = price;
        price = price - (price * parcent / 100);

    }
};


int main()
{
    int current_year;
    cout << "Enter current year: ";
    cin >> current_year;

    Car c1, c2, c3;

    cout << "\nEnter info for Car[1]: \n";
    c1.input();
   
    cout << "\nEnter info for Car[2]: \n";
    c2.input();
   
    cout << "\nEnter info for Car[3]: \n";
    c3.input();

    c1.discount(10);
    c2.discount(10);
    c3.discount(10);

    c1.displayinfo(current_year);
    c2.displayinfo(current_year);
    c3.displayinfo(current_year);


    return 0;
}
/*Name: Md Rafi Khan
ID: 41240302538
Lab No: Lab_2
Course Code: CSE 1360*/

//Problem_1: Student Class

#include<iostream>
#include<string>

using namespace std;

class student {
    private:
    string name;
    int rollNubmer;
    float marks[5];
    float totalMarks;

    public:
    student(string n, int roll, float m[]) {
        name = n;
        rollNubmer = roll;
        totalMarks = 0;
        for (int i = 0; i < 5; i++) {
            marks[i] = m[i];
        }

        calculatetotalmarks();
    }

    void calculatetotalmarks() {
        totalMarks = 0;
        for (int i = 0; i < 5; i++){
            totalMarks = totalMarks + marks[i];
        }
    }

    float getpercentage() {
        return totalMarks / 5.0;
    }

        void displayinfo() {
            cout << "Name: " << name << endl;
            cout << "Roll Number: " << rollNubmer << endl;
            cout << "Marks in 5 Sub: ";
            for (int i = 0; i < 5; i++) {
                cout << marks[i] << " ";
            }

            cout << endl;
            cout << "Total Marks: " << totalMarks << endl;
            cout << "Percentage: " << getpercentage()<< endl;
            cout << "--------------------------" << endl;
        }
};

int main() {
    float m1[5] = {85.5, 90.0, 78.5, 88.0, 92.0};
    float m2[5] = {75.0, 80.5, 70.0, 68.5, 77.0};
    float m3[5] = {95.0, 93.5, 89.0, 97.0, 94.5};

    student student1("Rafi", 2538, m1);
    student student2("Najmul", 2539, m2);
    student student3("Farhan", 2554, m3);

     
    cout << "Student Information:\n";
    student1.displayinfo();
    student2.displayinfo();
    student3.displayinfo();

    return 0;
}





//Problem_2: Car Class

#include<iostream>
#include<string>
#include<ctime>
using namespace std;

class car
{
private:
    string brand;
    string model;
    int year;
    float prize;
public:
    car(string b, string m, int y, float pr) {
        brand = b;
        model = m;
        year = y;
        prize = pr;
    }

    void displayinfo() {
        cout << "Brand: " << brand << endl;
        cout << "Model: " << model << endl;
        cout << "Year: " << year << endl;
        cout << "Prize: " << prize << endl;
        cout << (isAntique() ? "Status: Antique Car" : "Status: Modern Car") << endl;
        cout << "--------------------------" << endl;
    }

    void depreciation(float percentage) {
        float depammout = prize * (percentage / 100);
        prize = prize - depammout;
    }

    bool isAntique() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        int currentYear = 1900 + ltm->tm_year;
        return (currentYear - year) > 25;
    }
    };

    int main() {
    
    car car1("Toyota", "Corolla", 1995, 5000);
    car car2("Tesla", "Model S", 2020, 80000);
    car car3("Ford", "Mustang", 1970, 25000);

    cout << "Before Depreciation:\n";
    car1.displayinfo();
    car2.displayinfo();
    car3.displayinfo();

    car1.depreciation(10); 
    car2.depreciation(15);
    car3.depreciation(5);

    cout << "\nAfter Depreciation:\n";
    car1.displayinfo();
    car2.displayinfo();
    car3.displayinfo();

    return 0;

    }
#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int rollNumber;
    float marks[5];
    float totalMarks;

public:
    // Constructor to initialize the attributes
    Student(string n, int roll, float m[]) {
        name = n;
        rollNumber = roll;
        totalMarks = 0;
        for (int i = 0; i < 5; i++) {
            marks[i] = m[i];
        }
        calculateTotalMarks(); // Automatically calculate total marks on creation
    }

    // Method to calculate the total marks
    void calculateTotalMarks() {
        totalMarks = 0;
        for (int i = 0; i < 5; i++) {
            totalMarks += marks[i];
        }
    }

    // Method to return percentage
    float getPercentage() {
        return totalMarks / 5.0; // Assuming each subject is out of 100
    }

    // Method to display student information
    void displayInfo() {
        cout << "Name: " << name << endl;
        cout << "Roll Number: " << rollNumber << endl;
        cout << "Marks in 5 Subjects: ";
        for (int i = 0; i < 5; i++) {
            cout << marks[i] << " ";
        }
        cout << endl;
        cout << "Total Marks: " << totalMarks << endl;
        cout << "Percentage: " << getPercentage() << "%" << endl;
        cout << "--------------------------" << endl;
    }
};

int main() {
    float m1[5] = {85.5, 90.0, 78.5, 88.0, 92.0};
    float m2[5] = {75.0, 80.5, 70.0, 68.5, 77.0};
    float m3[5] = {95.0, 93.5, 89.0, 97.0, 94.5};

    Student student1("Alice", 101, m1);
    Student student2("Bob", 102, m2);
    Student student3("Charlie", 103, m3);

    cout << "Student Information:\n";
    student1.displayInfo();
    student2.displayInfo();
    student3.displayInfo();

    return 0;
}

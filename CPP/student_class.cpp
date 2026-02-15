#include <iostream>
#include <iomanip>
using namespace std;

class Student {
private:
    string name;
    int rollNumber;
    float marks[5];
    float totalMarks;

public:
    // Constructor
    Student(string n, int roll, float m[5], float tm = 0) {
        name = n;
        rollNumber = roll;
        totalMarks = tm; 
        for (int i = 0; i < 5; i++) {
            marks[i] = m[i];
        }
    }

        // Method to calculate total marks
    void calculateTotal() {
        totalMarks = 0;
        for (int i = 0; i < 5; i++) {
            totalMarks += marks[i];
        }
    }

// Method to calculate percentage
    float getPercentage() {
        return (totalMarks / 500.0) * 100;  // total 500 marks (5 subjects × 100)
    }

    // Method to display student info
    void display() {
        cout << "Name       : " << name << endl;
        cout << "Roll Number: " << rollNumber << endl;
        cout << "Marks      : ";
        for (int i = 0; i < 5; i++) {
            cout << marks[i] << " ";
        }
        cout << endl;
        cout << "Total Marks: " << totalMarks << endl;
        cout << "Percentage : " << getPercentage() << "%" << endl;
        cout << "----------------------------------" << endl;
    }
};

int main() {
    // Marks for 3 students
    float m1[5] = {85, 90, 78, 88, 92};
    float m2[5] = {70, 65, 80, 75, 68};
    float m3[5] = {95, 98, 92, 90, 96};

    // Creating objects
    Student s1("Alice", 101, m1);
    Student s2("Bob", 102, m2);
    Student s3("Charlie", 103, m3);

    // Calculate totals
    s1.calculateTotal();
    s2.calculateTotal();
    s3.calculateTotal();

    // Display info
    cout << "Student Details:\n";
    s1.display();
    s2.display();
    s3.display();

    return 0;
}
#include <iostream>
using namespace std;

// Base Class
class Person {
public:
    string name;
    int age;

    void inputPerson(string n, int a) {
        name = n;
        age = a;
    }

    void displayPerson() {
        cout << "Name: " << name << endl;
        cout << "Age : " << age << endl;
    }
};

// Derived Class 1
class Student : public Person {
public:
    int id;
    void inputStudent(int i) {
        id = i;
    }
    void displayStudent() {
        cout << "Student ID: " << id << endl;
    }
};

// Derived Class 2
class Teacher : public Person {
public:
    string subject;
    void inputTeacher(string sub) {
        subject = sub;
    }
    void displayTeacher() {
        cout << "Subject: " << subject << endl;
    }
};

int main() {
    // Student object
    Student s;
    s.inputPerson("Noman", 20);
    s.inputStudent(220);
    cout << "--- Student Info ---" << endl;
    s.displayPerson();
    s.displayStudent();

    cout << endl;

    // Teacher object
    Teacher t;
    t.inputPerson("Rafi", 35);
    t.inputTeacher("Physics");
    cout << "--- Teacher Info ---" << endl;
    t.displayPerson();
    t.displayTeacher();

    return 0;
}

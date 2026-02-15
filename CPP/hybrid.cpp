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

// Derived Class 1 (Single Inheritance)
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

// Derived Class 2 (Hierarchical Inheritance)
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

// Derived Class 3 (Multiple Inheritance)
class Researcher : public Student, public Teacher {
public:
    string topic;
    void inputResearch(string t) {
        topic = t;
    }
    void displayResearch() {
        cout << "Research Topic: " << topic << endl;
    }
};

int main() {
    Researcher r;

    // Person থেকে নাম বয়স আসবে Teacher এর মাধ্যমে
    r.Teacher::inputPerson("Anonymous", 25);
    r.inputStudent(220);
    r.inputTeacher("Computer Science");
    r.inputResearch("Artificial Intelligence");

    cout << "--- Researcher Info ---" << endl;
    r.Teacher::displayPerson(); // Person info via Teacher
    r.displayStudent();
    r.displayTeacher();
    r.displayResearch();

    return 0;
}

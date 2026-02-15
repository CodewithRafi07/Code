#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct Student {
    int id;
    string name;
    float marks;
};

class StudentSystem {
    vector<Student> students;
public:
    void addStudent() {
        Student s;
        cout << "\nEnter ID: ";
        cin >> s.id;
        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, s.name);
        cout << "Enter Marks: ";
        cin >> s.marks;
        students.push_back(s);
        cout << "\n✅ Student Added!\n";
    }

    void viewAll() {
        cout << "\n--- Student List ---\n";
        cout << left << setw(10) << "ID" << setw(20) << "Name" << setw(10) << "Marks" << "\n";
        for (auto &s : students)
            cout << left << setw(10) << s.id << setw(20) << s.name << setw(10) << s.marks << "\n";
    }

    void searchStudent() {
        int id;
        cout << "\nEnter ID to Search: ";
        cin >> id;
        for (auto &s : students) {
            if (s.id == id) {
                cout << "\nFound → " << s.name << " | Marks: " << s.marks << "\n";
                return;
            }
        }
        cout << "\n❌ Not Found!\n";
    }
};

int main() {
    StudentSystem sys;
    int choice;
    do {
        cout << "\n====== Student Management System ======\n";
        cout << "1. Add Student\n2. View All\n3. Search\n4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: sys.addStudent(); break;
            case 2: sys.viewAll(); break;
            case 3: sys.searchStudent(); break;
            case 4: cout << "\nExiting...\n"; break;
            default: cout << "\nInvalid Option!\n";
        }
    } while (choice != 4);
}

#include<iostream>
using namespace std;

class person
{
private:
    /* data */
public:
string name;
int age;
    person(){

    }

    void display(){
        cout << "Name" << " : "<< name << endl;
        cout << "Age" << " : "<< age << endl;
    }
};

class student
{
private:
    /* data */
public:
int id;
    student(){

    }

    void display(){
        cout << "ID" <<" : " << id << endl;
    }
};

class child: public person, public student{
    public:

    child() {

    }
     void display() {
        person::display();   // Name আর Age দেখাবে
        student::display();  // ID দেখাবে
    }

};

int main() {
    child obj;
    obj.name = "Noman";
    obj.age = 40;
    obj.id = 220;
    // obj.display;  // Error
    //obj.person::display();  // correct
    //obj.student::display(); // correct
    obj.display();
}


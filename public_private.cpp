#include <iostream>
using namespace std;

class Student {
private:
    string name;
    int age;

public:
    Student() {
        name = "Test";
        age = 10;
    }

    void setName(string n) {
        name = n;
    }

    string getName() {
        return name;
    }

    void setAge(int a) {
        age = a;
    }

    int getAge() {  
        return age;
    }
};

int main() {
    Student s1;

    cout << "Student name: " << s1.getName() << endl; 
    cout << "Age: " << s1.getAge() << endl; 

    return 0;
}

#include <bits/stdc++.h>
using namespace std;

struct student {
    string name; // ← FIXED: use string
    int age;
    float GPA;
} ku2[5];

int main() {
    for (int i = 0; i < 5; i++) {
        cout << "Enter name, age, and GPA for student " << (i + 1) << ": ";
        cin >> ku2[i].name >> ku2[i].age >> ku2[i].GPA;
    }

    cout << '\n';
    for (int i = 0; i < 5; i++) {
        if(ku2[i].GPA <= 1.2){
            cout << ku2[i].name << " is " << ku2[i].age << " years old with GPA " << ku2[i].GPA << '\n';
        }
        
    }
}
                
#include <iostream>
using namespace std;

int main() {
    char start;
    cin >> start;

    for (char i = start; i <= 'Z'; i++) {
        for (char j = start; j <= i; j++) {
            cout << j;
        }
        cout << endl;
    }

    return 0;
}

#include <iostream>
#include <string>
using namespace std;

int main() {
    int t, amount, bank = 0;
    string command;

    cin >> t;

    for (int a = 0; a < t; a++) {
        cin >> command;

        if (command == "donate") {
            cin >> amount;
            bank += amount;
        } else if (command == "report") {
            cout << bank << '\n';
        }
    }

    return 0;
}

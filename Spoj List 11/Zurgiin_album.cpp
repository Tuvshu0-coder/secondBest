#include <iostream>
using namespace std;

int main() {
    int N, p;
    cin >> N >> p;

    int from_front = p / 2;
    int from_back = (N / 2) - (p / 2);

    cout << min(from_front, from_back) << endl;
    return 0;
}
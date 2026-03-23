#include <iostream>
using namespace std;

int main() {
    int x, y;
    cin >> x >> y;

    int distance = 0;
    int direction = 1;
    int step = 1;
    int current = x;

    while (true) {
        int next = x + direction * step;

        if ((y >= current && y <= next) || (y <= current && y >= next)) {
            distance = distance + (y - current);
            if (distance < 0) {
                distance = -distance;
            }
            break;
        }

        distance = distance + (next - current);
        if (distance < 0) {
            distance = -distance;
        }

        current = next;
        direction = direction * -1;
        step = step * 2;
    }

    cout << distance << "\n";

    return 0;
}

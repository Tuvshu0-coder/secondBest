#include <bits/stdc++.h>
using namespace std;

int main() {
    int X, Y, M;
    cin >> X >> Y >> M;

    int best = 0;

    for (int a = 0; a * X <= M; a++) {
        for (int b = 0; a * X + b * Y <= M; b++) {
            int total = a * X + b * Y;
            best = max(best, total);
        }
    }

    cout << best << "\n";
    return 0;
}

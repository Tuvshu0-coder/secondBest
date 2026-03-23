#include <bits/stdc++.h>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;

    int road[100];
    int bessie[100];

    int pos = 0;
    for (int i = 0; i < N; i++) {
        int length, speed;
        cin >> length >> speed;
        for (int j = 0; j < length; j++) {
            road[pos] = speed;
            pos++;
        }
    }
    pos = 0;
    for (int i = 0; i < M; i++) {
        int length, speed;
        cin >> length >> speed;
        for (int j = 0; j < length; j++) {
            bessie[pos] = speed;
            pos++;
        }
    }
    int maxOver = 0;
    for (int i = 0; i < 100; i++) {
        if (bessie[i] > road[i]) {
            int diff = bessie[i] - road[i];
            if (diff > maxOver) {
                maxOver = diff;
            }
        }
    }

    cout << maxOver << "\n";

    return 0;
}

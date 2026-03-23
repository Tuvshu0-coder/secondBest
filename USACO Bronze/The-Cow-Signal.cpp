#include <bits/stdc++.h>
using namespace std;

int main() {
    int M, N, K;
    cin >> M >> N >> K;

    vector<string> signal(M);
    for (int i = 0; i < M; i++) {
        cin >> signal[i];
    }

    for (int i = 0; i < M; i++) {
        for (int repeatRow = 0; repeatRow < K; repeatRow++) {
            for (int j = 0; j < N; j++) {
                for (int repeatCol = 0; repeatCol < K; repeatCol++) {
                    cout << signal[i][j];
                }
            }
            cout << "\n";
        }
    }

    return 0;
}

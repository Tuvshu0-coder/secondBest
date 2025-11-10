#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;

    vector<pair<int, int>> shifts(N);
    for (int i = 0; i < N; i++) cin >> shifts[i].first >> shifts[i].second;

    int best = 0;

    for (int i = 0; i < N; i++) {
        vector<int> covered(1000, 0);

        for (int j = 0; j < N; j++) {
            if (i == j) continue;
            for (int t = shifts[j].first; t < shifts[j].second; t++)
                covered[t] = 1;
        }


        int total = 0;
        for (int t = 0; t < 1000; t++)
            total += covered[t];

        best = max(best, total);
    }

    cout << best << "\n";
    return 0;
}

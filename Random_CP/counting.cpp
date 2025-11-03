#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<pair<char, int>> cows(N);

    for (int i = 0; i < N; i++) {
        cin >> cows[i].first >> cows[i].second;
    }


    vector<int> candidates;
    for (auto &c : cows) {
        candidates.push_back(c.second);
        candidates.push_back(c.second + 1);
    }

    int ans = N;

    for (int x : candidates) {
        int lies = 0;
        for (auto &c : cows) {
            if (c.first == 'L' && x > c.second) lies++;
            if (c.first == 'G' && x < c.second) lies++;
        }
        ans = min(ans, lies);
    }

    cout << ans << "\n";
    return 0;
}

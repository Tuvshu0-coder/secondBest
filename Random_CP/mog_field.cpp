#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    // Directions
    map<char, pair<int, int>> dir;
    dir['N'] = make_pair(0, 1);
    dir['S'] = make_pair(0, -1);
    dir['E'] = make_pair(1, 0);
    dir['W'] = make_pair(-1, 0);

    char D;
    int S;
    vector<char> directions;
    vector<int> steps;

    for (int i = 0; i < N; i++) {
        cin >> D >> S;
        directions.push_back(D);
        steps.push_back(S);
    }

    // Starting point and time
    int x = 0, y = 0, t = 0;
    map<pair<int, int>, int> visited;
    visited[make_pair(x, y)] = t;

    int min_delta = INT_MAX;
    bool has_repeat = false;

    // Simulate
    for (int i = 0; i < N; i++) {
        char d = directions[i];
        int s = steps[i];
        int dx = dir[d].first;
        int dy = dir[d].second;

        for (int j = 0; j < s; j++) {
            t++;
            x += dx;
            y += dy;

            pair<int, int> pos = make_pair(x, y);

            if (visited.count(pos)) {
                int delta = t - visited[pos];
                if (delta < min_delta) {
                    min_delta = delta;
                }
                has_repeat = true;
            }
            visited[pos] = t;
        }
    }

    if (has_repeat)
        cout << min_delta << "\n";
    else
        cout << -1 << "\n";

    return 0;
}
#include <bits/stdc++.h>
using namespace std;

struct Event {
    long long x, y, t;
};

long long dist2(long long x1, long long y1, long long x2, long long y2) {
    long long dx = x1 - x2;
    long long dy = y1 - y2;
    return dx*dx + dy*dy;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int G, N;
    cin >> G >> N;

    vector<Event> graze(G);
    for (int i = 0; i < G; i++) {
        cin >> graze[i].x >> graze[i].y >> graze[i].t;
    }

    sort(graze.begin(), graze.end(), [](auto &a, auto &b){
        return a.t < b.t;
    });

    vector<long long> times(G);
    for (int i = 0; i < G; i++) times[i] = graze[i].t;

    int innocent = 0;

    while (N--) {
        long long ax, ay, at;
        cin >> ax >> ay >> at;

        int idx = lower_bound(times.begin(), times.end(), at) - times.begin();

        bool guilty = false;

        for (int d = -2; d <= 2; d++) {
            int i = idx + d;
            if (i < 0 || i >= G) continue;

            long long dt = llabs(times[i] - at);
            long long d2 = dist2(ax, ay, graze[i].x, graze[i].y);

            if (d2 <= dt * dt) {
                guilty = true;
                break;
            }
        }

        if (!guilty) innocent++;
    }

    cout << innocent << "\n";
    return 0;
}

// #include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

static long long floor_div(long long a, long long b) {
    // b is always positive in this solution.
    if (a >= 0) return a / b;
    return -(((-a) + b - 1) / b);
}

void solve() {
    int t;
    cin >> t;
    while (t--) {
        int n;
        cin >> n;
        vector<long long> a(n);
        for (int i = 0; i < n; i++) cin >> a[i];

        vector<int> ans(n, 0);

        for (int i = 0; i < n; i++) {
            vector<pair<long long, int>> events;
            int cur = 0;

            for (int j = i + 1; j < n; j++) {
                if (a[i] == a[j]) continue;

                long long s = a[i] + a[j];
                if (a[i] > a[j]) {
                    // |a[i]-k| > |a[j]-k|  <=>  k < (a[i] + a[j]) / 2
                    // For integer k: k <= floor((s - 1) / 2).
                    long long upper = floor_div(s - 1, 2);
                    cur++;
                    events.push_back({upper + 1, -1});
                } else {
                    // |a[i]-k| > |a[j]-k|  <=>  k > (a[i] + a[j]) / 2
                    // For integer k: k >= floor(s / 2) + 1.
                    long long lower = floor_div(s, 2) + 1;
                    events.push_back({lower, +1});
                }
            }

            int best = cur;
            sort(events.begin(), events.end());

            int p = 0;
            while (p < (int)events.size()) {
                long long x = events[p].first;
                int delta = 0;
                while (p < (int)events.size() && events[p].first == x) {
                    delta += events[p].second;
                    p++;
                }
                cur += delta;
                if (cur > best) best = cur;
            }

            ans[i] = best;
        }

        for (int i = 0; i < n; i++) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << '\n';
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
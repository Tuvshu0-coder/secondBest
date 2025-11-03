#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, T;
    cin >> N >> T;

    vector<long long> arr(N);
    for (auto &x : arr) cin >> x;

    vector<long long> pref(N + 1, 0);
    for (int i = 1; i <= N; i++) {
        pref[i] = pref[i - 1] + arr[i - 1];
    }

    while (T--) {
        int l, r;
        cin >> l >> r;
        cout << pref[r] - pref[l] << '\n';
    }

    return 0;
}

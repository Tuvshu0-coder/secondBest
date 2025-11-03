#include <bits/stdc++.h>
using namespace std;
#define ll long long

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    ll N;
    cin >> N;
    vector<ll> arr(N), manleft(N, LLONG_MAX), manright(N, LLONG_MAX);

    for (ll &t : arr) {
        cin >> t;
    }

    ll ans = LLONG_MAX;
    vector<ll> copy = arr, copy1 = arr;
    manleft[0] = 0;
    manright[N - 1] = 0;

    for (ll i = 1; i < N; i++) {
        copy[i] = max(copy[i], max(copy[i - 1] + 1, arr[i] + manleft[i - 1]));
        manleft[i] = max(manleft[i - 1], copy[i - 1] - arr[i] + 1);
    }

    for (ll i = N - 2; i >= 0; i--) {
        copy1[i] = max(copy1[i], max(copy1[i + 1] + 1, arr[i] + manright[i + 1]));
        manright[i] = max(manright[i + 1], copy1[i + 1] - arr[i] + 1);
    }

    for (ll i = 0; i < N; i++) {
        ans = min(ans, max(manleft[i], manright[i]));
    }

    cout << ans;
    return 0;
}

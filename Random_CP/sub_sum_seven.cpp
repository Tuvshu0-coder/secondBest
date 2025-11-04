#include <iostream>
#include <vector>
using namespace std;

using ll = long long;

vector<ll> prefSum(const vector<ll> &arr) {
    ll n = arr.size();
    vector<ll> prefixSum(n + 1, 0);
    for (ll i = 0; i < n; i++)
        prefixSum[i + 1] = prefixSum[i] + arr[i];
    return prefixSum;
}

int main() {
    freopen("div7.in", "r", stdin);
    freopen("div7.out", "w", stdout);

    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll N;
    cin >> N;

    vector<ll> arr(N);
    for (ll &t : arr)
        cin >> t;

    vector<ll> prefixSum = prefSum(arr);
    ll maxim = 0;

    for (ll i = 1; i <= N; i++) {
        for (ll j = 0; j < i; j++) {
            if ((prefixSum[i] - prefixSum[j]) % 7 == 0)
                maxim = max(maxim, i - j);
        }
    }

    cout << maxim;
    return 0;
}

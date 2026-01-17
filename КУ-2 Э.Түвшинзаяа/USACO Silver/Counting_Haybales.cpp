#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Interval{
    int left;
    int right;
};

signed main(){
    freopen("haybales.in", "r", stdin);
    freopen("haybales.out", "w", stdout);
    int N, Q;
    cin >> N;
    cin >> Q;
    vector<int> location(N);
    for(int &t: location) cin >> t;
    sort(location.begin(), location.end());
    vector<Interval> lalar(Q);
    for(int i=0; i<Q; i++){
        cin >> lalar[i].left  >> lalar[i].right;
        int left, right, delta;
        left = lower_bound(location.begin(), location.end(), lalar[i].left) - location.begin();
        right = upper_bound(location.begin(), location.end(), lalar[i].right) - location.begin();
        delta = right - left;
        cout << delta << '\n';
    }
    



    return 0;
}
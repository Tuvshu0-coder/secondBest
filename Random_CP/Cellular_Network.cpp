#include <bits/stdc++.h>
using namespace std;
#define int long long



signed main(){
    int n, m;
    cin >> n >> m;
    vector<int> city(n), tower(m); 
    for(int &t : city) cin >> t;
    for(int &t : tower) cin >> t;
    sort(city.begin(), city.end());
    sort(tower.begin(), tower.end());
    int maxim = INT_MIN;
    
    for(int i=0; i<n; i++){
        int d = INT_MAX;
        auto it = lower_bound(tower.begin(), tower.end(), city[i]);
        if(it != tower.end()) d = min(d, abs(city[i] - *it));
        if(it != tower.begin()){
            --it;
            d = min(d, abs(city[i] - *it));
        }
        maxim = max(maxim, d);
    }

    cout << maxim;



}
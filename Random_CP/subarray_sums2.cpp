#include <bits/stdc++.h>
using namespace std;
#define int long long


signed main(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int N, target;
    cin >> N >> target;
    vector<int> arr(N);
    for(int &t : arr) cin >> t;
    vector<int> pref(N+1, 0);
    for(int i=0; i<N; i++){
        pref[i+1] = pref[i] + arr[i];
    }
    int count =0;
    for(int i=1; i<=N; i++){
        for(int j=0; j<i; j++){
            if(pref[i]-pref[j]==target) count++;
        }
    }
    cout  << count;


    return 0;
}
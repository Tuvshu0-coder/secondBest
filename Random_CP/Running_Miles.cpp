#include <bits/stdc++.h>
using namespace std;
#define int long long
signed main(){
    int T;
    cin >> T;
    while(T--){
        int N;
        cin >> N;
        vector<int> arr(N+1);
        for(int i = 1; i <= N; i++) cin >> arr[i];
        vector<int> pref(N+1, 0), suff(N+2, -100000000);
        for(int i=1; i<=N; i++){
            pref[i] = max(pref[i-1], arr[i]+i);
        }
        for(int i=N; i>=1; i--){
            suff[i] = max(suff[i+1], arr[i]-i); 
        }
        int maximum=0;
        // for (int i = 1;i <= N; i++) {
        //     cout << pref[i] << ' ';
        // }
        // cout << '\n';
        // for (int i = 1;i <= N; i++) {
        //     cout << suff[i] << ' ';
        // }
        for(int i=1; i<=N; i++){
            maximum = max(maximum, arr[i]+pref[i-1]+suff[i+1]);
        }
        cout << maximum << '\n';

    }


    return 0;
}
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int main(){
    freopen("bcount.in", "r", stdin);
    freopen("bcount.out", "w", stdout);
    int N, Q;
    cin >> N >> Q;
    vector<ll> arr(N);
    for(ll &t : arr) cin >> t;
    vector<array<int , 3>> pref(N+1, {0, 0, 0});
    //vector<tuple<ll, ll, ll>> pref(N+1);


    for(int i=0; i< N; i++){
        pref[i+1] = pref[i];
        if(arr[i]==1){
            pref[i+1][0]++;
        }else if(arr[i]==2){
            pref[i+1][1]++;
        }else if(arr[i]==3){
            pref[i+1][2]++;
        }

    }

    while(Q--){
        int left, right, n1, n2, n3;
        cin >> left >> right;
        n1 = pref[right][0] - pref[left-1][0];
        n2 = pref[right][1] - pref[left-1][1];
        n3 = pref[right][2] - pref[left-1][2];
        cout << n1 << " " << n2 << " " << n3 << '\n';        
    }


}
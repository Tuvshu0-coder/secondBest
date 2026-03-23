#include <bits/stdc++.h>
using namespace std;
#define int long long


signed main(){
    freopen("maxcross.in", "r", stdin);
    freopen("maxcross.out", "w", stdout);
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int N, K, B;
    cin >> N >> K >> B;
    vector<int> arr(N+1, 0);
    for(int i=0; i<B; i++){
        int a;
        cin >> a;
        arr[a]=1;
    }
    vector<int> pref(N+1, 0);
    for(int i=1; i<=N; i++){
        pref[i]=pref[i-1]+arr[i];
    }
    int minim=INT_MAX;
    for(int i=K; i<=N; i++){
        int sum = pref[i]-pref[i-K];
        minim = min(minim, sum);
    }
    cout << minim;


    return 0;
}
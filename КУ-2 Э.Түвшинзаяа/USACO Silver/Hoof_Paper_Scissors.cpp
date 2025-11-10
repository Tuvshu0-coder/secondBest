#include <bits/stdc++.h>
using namespace std;
#define int long long

signed main(){
    freopen("hps.in", "r", stdin);
    freopen("hps.out", "w", stdout);
    ios_base::sync_with_stdio(0);
    cin.tie(0);


    int N;
    cin >> N;
    vector<char> arr(N);
    for(char &t : arr) cin >> t;
    vector<int> hoof(N+1, 0), paper(N+1, 0), sci(N+1, 0); // prefix
    for(int i=1; i<=N; i++){
        paper[i]=paper[i-1];
        hoof[i]=hoof[i-1];
        sci[i]=sci[i-1];
        if(arr[i-1]=='P') paper[i]++;
        if(arr[i-1]=='H') hoof[i]++;
        if(arr[i-1]=='S') sci[i]++;
    }
    int sum=0;
    for(int i=1; i<=N; i++){
        int max1, max2;
        max1 = max({paper[i], hoof[i], sci[i]});
        max2 = max({paper[N]-paper[i], hoof[N]-hoof[i], sci[N]-sci[i]});
        sum = max(sum, max1+max2);
    }
    cout << sum;




}